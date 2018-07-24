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
      ZIP_FILES = $(TAR_FILES)
 
    INSTALL_DIR = /usr/local
        INC_DIR = $(INSTALL_DIR)/include/$(COMPONENT)
        LIB_DIR = $(INSTALL_DIR)/lib

          OWNER = 0
          GROUP = 0
           MODE = 444
          XMODE = 755

        INSTALL = install

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
	rm -f $(COMPONENT).tar.gz $(COMPONENT).zip
	rm -f $(TARGET)
	cd tests; make wipe
	cd poker; make wipe

tar:
	tar -zcf $(COMPONENT).tar.gz $(TAR_FILES)

zip:
	zip $(COMPONENT).zip $(ZIP_FILES)

#

install:	project
install:	install.dir install.inc install.lib

install.dir:
	@for d in $(INC_DIR) $(LIB_DIR); do if [ ! -d $$d ]; then mkdir -p $$d; chown $(OWNER) $$d; chgrp $(GROUP) $$d; chmod $(XMODE) $$d; fi; done

install.inc:
	$(INSTALL) -c -m $(MODE) -o $(OWNER) -g $(GROUP) $(INC_FILES) $(INC_DIR)

install.lib:
	$(INSTALL) -c -m $(MODE) -o $(OWNER) -g $(GROUP) $(LIB_FILES) $(LIB_DIR)

#
OL.c:		OL.h
So-o.c:		So-o.h
Object.c:	Object.h
