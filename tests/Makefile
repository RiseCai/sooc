      COMPONENT = so-o

          TESTS = test-Object test-OL test-list test-alist test-Hello test-Application test-Responder

             CC = gcc

         CFLAGS = -I.. -Wno-missing-braces -O -fstrength-reduce -finline-functions -fomit-frame-pointer
        
        SOO_LIB = ../lib$(COMPONENT).a

      LIB_FILES = $(SOO_LIB)

#.SILENT:

project:	tests

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TESTS) core *.o *.exe *~

wipe:	clean

tests:		$(TESTS)

test-Application:		test-Application.o $(SOO_LIB)
	$(CC) $^ -o $@

test-Responder:		test-Responder.o $(SOO_LIB)
	$(CC) $^ -o $@

test-Hello:		test-Hello.o $(SOO_LIB)
	$(CC) $^ -o $@

test-Object:	test-Object.o X.o Y.o Z.o $(SOO_LIB)
	$(CC) $^ -o $@

test-OL:		test-OL.o X.o Y.o Z.o $(SOO_LIB)
	$(CC) $^ -o $@

test-list:	../list.c
	$(CC) $(CFLAGS) -DTEST $^ -o $@
test-alist:	../alist.c
	$(CC) $(CFLAGS) -DTEST $^ -o $@

