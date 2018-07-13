/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

#include "Card.h"
#include "Hand.h"
#include "Deck.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <malloc.h>

static void display_mallinfo( void ) {
	struct mallinfo mi;

	mi = mallinfo();

	printf("\n");
	printf("Total non-mmapped bytes (arena):       %d\n", mi.arena);
	printf("# of free chunks (ordblks):            %d\n", mi.ordblks);
//	printf("# of free fastbin blocks (smblks):     %d\n", mi.smblks);
//	printf("# of mapped regions (hblks):           %d\n", mi.hblks);
//	printf("Bytes in mapped regions (hblkhd):      %d\n", mi.hblkhd);
//	printf("Max. total allocated space (usmblks):  %d\n", mi.usmblks);
//	printf("Free bytes held in fastbins (fsmblks): %d\n", mi.fsmblks);
	printf("Total allocated space (uordblks):      %d\n", mi.uordblks);
	printf("Total free space (fordblks):           %d\n", mi.fordblks);
	printf("Topmost releasable block (keepcost):   %d\n", mi.keepcost);
	printf("\n");
}

int main( int argc, char *argv[] ) {
	defclassCard();
	defclassHand();
	defclassDeck();

	instance deck, hand, card;
	int i, n;

	display_mallinfo();

	deck = sendmsg(Deck, "new", 1).p;
	sendmsg(deck, "print", 1);

	card = sendmsg(deck, "deal").p;
	sendmsg(card, "print", 1);
	card = sendmsg(deck, "deal").p;
	sendmsg(card, "print", 1);

	sendmsg(deck, "shuffle");
	sendmsg(deck, "print", 1);

	for (n = 0; n < 10; n++) {
		hand = sendmsg(deck, "hand").p;
		sendmsg(hand, "print", 1);
		sendmsg(hand, "free");
	}

	int ndeals = 1000000, nstats = 9;
	int stats[nstats];
	for (i = 0;  i < nstats; i++ )
		stats[i] = 0;

	printf("Dealing %d hands", ndeals);

//	hand = sendmsg(deck, "hand").p;

	clock_t begin = clock();

	for (n = 0; n < ndeals; n++ ) {
		if ( n % (ndeals / 10) == 0) {
			printf(".");
			fflush(stdout);
		}
//		sendmsg(deck, "setHand", hand);

		hand = sendmsg(deck, "hand").p;
		stats[ sendmsg(hand, "evaluate").i ]++;
		sendmsg(hand, "free");
	}

	clock_t end = clock();

	double cputime = (double)(end - begin) / CLOCKS_PER_SEC;

	printf(" in %.2lf s!\n", cputime);

	char *handnames[] = { "NOTHING", "ONEPAIR", "TWOPAIRS", "THREEOFKIND", "STRAIGHT", "FLUSH", "FULLHOUSE", "FOUROFKIND", "STRAIGHTFLUSH" };

	for (i = 0; i < nstats; i++ )
		printf("%14s -> %5.2f%% %d\n", handnames[i], stats[i]/(ndeals/100.0), stats[i]);

	sendmsg(deck, "free");

	display_mallinfo();

	exit( 0 );
}
