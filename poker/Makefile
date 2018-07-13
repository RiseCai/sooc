      COMPONENT = so-o

          TESTS = test-Card test-Hand test-Deck

             CC = gcc -g

         CFLAGS = -I.. -Wno-missing-braces -O -fstrength-reduce -finline-functions -fomit-frame-pointer
        
        SOO_LIB = ../lib$(COMPONENT).a

      LIB_FILES = $(SOO_LIB)

#.SILENT:

project:	poker

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TESTS) core *.o *.exe *~

wipe:	clean
	rm -f poker

poker:	poker.o Deck.o Hand.o Card.o $(SOO_LIB)
	$(CC) $^ -o $@

tests:		$(TESTS)

test-Card:		test-Card.o Card.o $(SOO_LIB)
	$(CC) $^ -o $@

test-Hand:		test-Hand.o Hand.o Card.o $(SOO_LIB)
	$(CC) $^ -o $@

test-Deck:		test-Deck.o Deck.o Hand.o Card.o $(SOO_LIB)
	$(CC) $^ -o $@
