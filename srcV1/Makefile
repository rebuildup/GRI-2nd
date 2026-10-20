# Objects other than main.o
OBJSA=getint.o led.o motor.o nled.o timer.o hardware_init.o

## Edit H8DIR and CC for windows OS
H8DIR = $(HOME)/.wine/drive_c/H8
# H8DIR = d:/home/tanabe/h8/MES/wingcc
MKLOCAL = $(H8DIR)/local
CBPATH = $(H8DIR)/app
CC = wine $(MKLOCAL)/bin/h8300-elf-gcc.exe $(LDFMINICOM)
# CC = $(MKLOCAL)/bin/h8300-elf-gcc.exe $(LDFMINICOM)

ifndef PROG
PROG=main
endif

ifdef MINICOM
LDFMINICOM=-DMINICOM
endif


CFLAGS = -Wall -B $(MKLOCAL)/h8300-elf -I$(CBPATH)/. -mint32 -mh -fPIC -O2
LDFA = -Wl,--section-start=.stack=0x400 -B $(MKLOCAL)/h8300-elf -L$(CBPATH)/ -mint32 -mh -T $(CBPATH)/h8pic.x -nostartfiles $(CBPATH)/h8piccrt0.S
LDFLAGS = -lmes_h8e

OBJS = $(PROG).o $(OBJSA) 

All : $(PROG).elf

$(PROG).elf : $(OBJS)
	$(CC) $(LDFA) -o $(PROG).elf  $(OBJS) $(LDFLAGS) $(LDF)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm $(PROG).elf $(OBJS)

pre:
	$(CC) $(CFLAGS) -E $(PROG).c -o $(PROG).i
	$(CC) $(CFLAGS) -S $(PROG).i -o $(PROG).s
