NAME=eggsloader
SRCS=eggsloader.c

BUILDDIR		= ./build

CC				= arm-none-eabi-gcc
AS				= arm-none-eabi-as
CFLAGS			= -mcpu=arm946e-s -mthumb -Os -nostartfiles -nodefaultlibs -fno-builtin
LDFLAGS			= --specs=nosys.specs -T linker.ld
INC				= -I../../include
LIB				=

TOOLS			= ../../tools

all: clean $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) $(LDFLAGS) $(SRCS) -o $(BUILDDIR)/$(NAME)
	arm-none-eabi-objcopy -O binary $(BUILDDIR)/$(NAME) $(BUILDDIR)/$(NAME).bin
	arm-none-eabi-objdump -d $(BUILDDIR)/$(NAME) > $(BUILDDIR)/$(NAME)_dump.txt
	python3 make_egg.py $(BUILDDIR)/$(NAME).bin $(BUILDDIR)/$(NAME)_egg.bin
	$(TOOLS)/converter.py -i $(BUILDDIR)/${NAME}_egg.bin -o $(BUILDDIR)/${NAME}_egg_par.txt --pointer -a 0xC318 -p 0x02108818

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)/*