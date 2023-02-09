NAME=tetris
SRCS=tetris.c

SYMBOLFILE		= ADAJN0J08.sym

BUILDDIR		= ./build

CC				= arm-none-eabi-gcc
AS				= arm-none-eabi-as
CFLAGS			= -mcpu=arm946e-s -mthumb -Os -nostartfiles -nodefaultlibs -fno-builtin
LDFLAGS			= --specs=nosys.specs -T linker.ld
INC				= -I../../include
#LIB				= -L../../lib -laeabi

LDFLAGS			+= -Wl,--just-symbols=$(SYMBOLFILE)
LIB				=

TOOLS			= ../../tools

all: clean $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) $(LDFLAGS) $(SRCS) -o $(BUILDDIR)/$(NAME) ${LIB}
	arm-none-eabi-objcopy -O binary $(BUILDDIR)/$(NAME) $(BUILDDIR)/$(NAME).bin
	arm-none-eabi-objdump -d $(BUILDDIR)/$(NAME) > $(BUILDDIR)/$(NAME)_dump.txt
	$(TOOLS)/toEggs.sh $(BUILDDIR)/$(NAME).bin

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)/*