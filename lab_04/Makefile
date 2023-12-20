CC := gcc
MAIN_APP := a.exe
.PHONY : clean

CFLAGS := -std=c99 -Wall -pedantic -pipe -g3

all : $(MAIN_APP)

$(MAIN_APP) : $(patsubst %.c,%.o, $(wildcard *.c))
	$(CC) $(CFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c -MD $<

include $(wildcard *.d)

clean :
	rm -f *.d *.o *.exe
