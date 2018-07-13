      COMPONENT = so-o
        VERSION = 1.0
       REVISION = 1

         TARGET = $(SOO_LIB)

             CC = gcc# -g -DTRYCATCH

         CFLAGS = -Wno-missing-braces -O -fstrength-reduce -finline-functions -fomit-frame-pointer
        
       SOO_SRCS = So-o.c Object.c OL.c list.c alist.c Hello.c Once.c Application.c Responder.c
       SOO_OBJS = $(SOO_SRCS:.c=.o)

        SOO_LIB = lib$(COMPONENT).a

      INC_FILES = So-o.h Object.h OL.h list.h alist.h Hello.h Once.h Application.h Responder.h
      LIB_FILES = $(SOO_LIB)

   LICENSE_FILE = LICENSE

      TAR_FILES = $(LICENSE_FILE) $(INC_FILES) $(SOO_SRCS) prototype Makefile
 
#.SILENT:

project:	$(TARGET)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(SOO_LIB):	$(SOO_OBJS)
	ar rc $@ $?

proto:
	./prototype So-o.c OL.c list.c alist.c
	
clean:
	rm -f core *.o *.exe *~

wipe:	clean
	rm -f $(TARGET)
	cd tests; make wipe
	cd poker; make wipe

tar:
	tar -zcf $(COMPONENT).tar.gz $(TAR_FILES)

#
OL.c:		OL.h
So-o.c:		So-o.h
Object.c:	Object.h
