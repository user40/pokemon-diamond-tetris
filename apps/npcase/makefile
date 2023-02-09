# ===================================================================================================
NAME			= npcase

# The name of .s file
TARGET			= markingACE_init_full

# The address where you want to extract the binary
VMA             = 0x0206DAB0

# ===================================================================================================
BUILDDIR		= ./build
TARGET_VMA      = $(TARGET)_$(VMA)
BIN             = $(TARGET_VMA).bin

CC				= arm-none-eabi-gcc
CFLAGS		    = -nostartfiles -nostdlib #-ffreestanding
LDFLAGS			= -Wl,--section-start=.text=$(VMA)

TOOLS	= ../../tools

all: clean $(BUILDDIR) egg

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(BIN):
	$(CC) $(TARGET).s -o $(BUILDDIR)/$(TARGET_VMA) $(CFLAGS) $(LDFLAGS)
	arm-none-eabi-objcopy -O binary $(BUILDDIR)/$(TARGET_VMA) $(BUILDDIR)/$(BIN)
	arm-none-eabi-objdump -d $(BUILDDIR)/$(TARGET_VMA) > $(BUILDDIR)/$(TARGET_VMA)_dump.txt
	rm $(BUILDDIR)/$(TARGET_VMA)

egg: $(BUILDDIR)/$(BIN)
	python3 make_egg.py $(BUILDDIR)/$(BIN) $(BUILDDIR)/$(NAME)
	python3 ${TOOLS}/converter.py -i $(BUILDDIR)/${NAME}_eggs.bin -o $(BUILDDIR)/${NAME}_eggs_par.txt --pointer -a 0x1E060 -p 0x02108818

clean:
	rm -rf $(BUILDDIR)/*