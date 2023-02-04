NAME=tetris
SRCS=tetris.c

SYMBOLFILE		= ADAJN0J08.sym

CC				= arm-none-eabi-gcc
AS				= arm-none-eabi-as
CFLAGS			= -mcpu=arm946e-s -mthumb -Os -nostartfiles -nodefaultlibs -fno-builtin
LDFLAGS			= --specs=nosys.specs -T linker.ld
INC				= -I../../include
#LIB				= -L../../lib -laeabi

LDFLAGS			+= -Wl,--just-symbols=$(SYMBOLFILE)
LIB				=

TOOLS			= ../../tools

all:
	$(CC) $(CFLAGS) $(INC) $(LDFLAGS) $(SRCS) -o $(NAME) ${LIB}
	arm-none-eabi-objcopy -O binary $(NAME) $(NAME).bin
	arm-none-eabi-objdump -d $(NAME) > $(NAME)_dump.txt
	$(TOOLS)/toEggs.sh $(NAME).bin

clean:
	rm -rf $(NAME) *.txt *.bin