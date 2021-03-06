GCC ?= riscv32-unknown-elf/bin/riscv32-unknown-elf

# The intermediate directory for compiled object files.
BUILD = build/

DRIVER = driver/
SOURCE = carbon1/

INCLUDE = -I include/ -I ${SOURCE}

TARGET = ${BUILD}/kernel.img
LIST = ${BUILD}/kernel.list
MAP = ${BUILD}/kernel.map

LINKER = ${SOURCE}/kernel.ld

AFLAGS = -march=rv32i -mabi=ilp32 -Wall
CFLAGS = -march=rv32i -mabi=ilp32 -Wall
LFLAGS = --no-undefined 

OBJECTS := $(patsubst $(SOURCE)%.s,$(BUILD)%.o,$(wildcard $(SOURCE)*.s))
OBJECTS += $(patsubst $(SOURCE)%.c,$(BUILD)%.o,$(wildcard $(SOURCE)*.c))
OBJECTS += $(patsubst $(DRIVER)%.c,$(BUILD)%.o,$(wildcard $(DRIVER)*.c))

# Rule to make everything.
all: $(TARGET) $(LIST)

# Rule to remake everything. Does not include clean.
rebuild: all

# Rule to make the listing file.
$(LIST) : $(BUILD)output.elf
	$(GCC)-objdump -d $(BUILD)output.elf > $(LIST)

# Rule to make the image file.
$(TARGET) : $(BUILD)output.elf
	$(GCC)-objcopy --input-target=elf32-littleriscv $(BUILD)output.elf -O binary $(TARGET) 

# Rule to make the elf file.
$(BUILD)output.elf : $(OBJECTS) $(LINKER)
	$(GCC)-ld $(LFLAGS) $(OBJECTS) -Map $(MAP) -o $(BUILD)output.elf -T $(LINKER) --print-memory-usage

# Rule to make the object files.
$(BUILD)%.o: $(SOURCE)%.s $(BUILD)
	$(GCC)-as $(AFLAGS) -I $(SOURCE) $< -o $@

$(BUILD)%.o: $(SOURCE)%.c $(BUILD)
	$(GCC)-gcc $(CFLAGS) $(INCLUDE) -c $< -o $@

$(BUILD)%.o: $(DRIVER)%.c $(BUILD)
	$(GCC)-gcc $(CFLAGS) $(INCLUDE) -c $< -o $@

# Rule to create build directory.
$(BUILD):
	mkdir $@

# Rule to clean files.
clean:
	-rm -rf $(BUILD)

prepare_image:
	dd if=/dev/zero of=build/flash.bin bs=64M count=1
	dd if=build/kernel.img of=build/flash.bin conv=notrunc

flash_buspirate: prepare_image
	date
	time flashrom -p buspirate_spi:dev=/dev/ttyUSB0,spispeed=2M -c MT25QL512 -l spi.layout -i flash -n -w build/flash.bin
