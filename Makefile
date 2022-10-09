-include ../config.mk

LIB_STATIC = wlib.a

# ls *.c | sed "s%\.c%.o%g" | grep -v _test | tr "\n" " " >> Makefile
OBJS = w_fileutils.o w_inet.o w_strfuncs.o w_utils.o 

#CPPFLAGS += -I..
CFLAGS += -I.. -Wall -g -O2 `pkg-config glib-2.0 --cflags`
LIBS   += `pkg-config glib-2.0 --libs`

all: $(LIB_STATIC) tests

$(LIB_STATIC): $(OBJS)
	$(AR) rcs $(LIB_STATIC) $(OBJS)

tests: tests.o wlib.a
	$(CC) $(CFLAGS) -o tests tests.c `pkg-config glib-2.0 --cflags --libs` wlib.a

strip:

distclean: clean

clean:
	rm -f *.o *~ *.a *.so tests

install:

install-strip: install

uninstall:

