# ===================================================================================================
NAME			= npcase

# The name of .s file
TARGET			= markingACE_init_full

# The address where you want to extract the binary
VMA             = 0x0206DAB0

# ===================================================================================================
TARGET_VMA     = $(TARGET)_$(VMA)
BIN             = $(TARGET_VMA).bin

CC				= arm-none-eabi-gcc
CFLAGS		    = -nostartfiles -nostdlib #-ffreestanding
LDFLAGS			= -Wl,--section-start=.text=$(VMA)

TOOLS	= ../../tools

all: clean egg

$(BIN):
	$(CC) $(TARGET).s -o $(TARGET_VMA) $(CFLAGS) $(LDFLAGS)
	arm-none-eabi-objcopy -O binary $(TARGET_VMA) $(BIN)
	arm-none-eabi-objdump -d $(TARGET_VMA) > $(TARGET_VMA)_dump.txt
	rm $(TARGET_VMA)

egg: $(BIN)
	python3 make_egg.py $(BIN)
	python3 ${TOOLS}/converter.py -i ${TARGET_VMA}_eggs.bin -o ${NAME}_eggs_par.txt --pointer -a 0x1E060 -p 0x02108818

clean:
	rm -rf $(NAME) *.txt *.bin