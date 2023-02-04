NAME=eggsloader
SRCS=eggsloader.c

CC				= arm-none-eabi-gcc
AS				= arm-none-eabi-as
CFLAGS			= -mcpu=arm946e-s -mthumb -Os -nostartfiles -nodefaultlibs -fno-builtin
LDFLAGS			= --specs=nosys.specs -T linker.ld
INC				= -I../../include
LIB				=

TOOLS			= ../../tools

all: clean
	$(CC) $(CFLAGS) $(INC) $(LDFLAGS) $(SRCS) -o $(NAME)
	arm-none-eabi-objcopy -O binary $(NAME) $(NAME).bin
	arm-none-eabi-objdump -d $(NAME) > $(NAME)_dump.txt
	python3 make_egg.py
	$(TOOLS)/converter.py -i ${NAME}_egg.bin -o ${NAME}_egg_par.txt --pointer -a 0xC318 -p 0x02108818

clean:
	rm -rf $(NAME) *.txt *.bin