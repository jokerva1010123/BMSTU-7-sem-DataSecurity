CC := gcc
MAIN_APP := a.out
.PHONY : clean

CFLAGS := -std=c99 -Wall -Werror -pedantic -pipe -g3

all : $(MAIN_APP)

$(MAIN_APP) : $(patsubst %.c,%.o, $(wildcard *.c))
	$(CC) $(CFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c -MD $<

include $(wildcard *.d)

clean :
	rm -f *.d *.o *.out *.enc decrypted_*
