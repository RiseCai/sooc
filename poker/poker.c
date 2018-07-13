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
#include <regex.h>

int main( int argc, char *argv[] ) {
	instance deck, hand;
	list keep = list_new();

	regex_t regexp;
	regmatch_t pmatch[2];

	regcomp(&regexp, "([1-5])", REG_EXTENDED );

	char line[16], num[2];
	char *p, *beg, *end;
	int i;

	defclassCard();
	defclassHand();
	defclassDeck();

	deck = sendmsg(Deck, "new", 1).p;

	for (;;) {
		sendmsg(deck, "shuffle");
		hand = sendmsg(deck, "hand").p;
		sendmsg(hand, "print", 1);

		printf("Keep (1-5...)? ");

		if (! fgets(line, sizeof (line), stdin))
			break;

		for (i = 0; i < 5; i++ )
			list_put(keep, i, 0);

		p = line;
		while (*p) {
			if (regexec(&regexp, p, 2, pmatch, 0) != 0 )
				break;

			if (pmatch[1].rm_so == -1)
				break;

			beg = p+pmatch[1].rm_so;
			end = p+pmatch[1].rm_eo;

			strncpy(num, beg, end-beg);

			i = atoi(num);

			list_put(keep, i-1, (void *)1);

			p = end;
		}

		for (i = 0; i < 5; i++ ) {
			if (!list_get(keep, i))
				sendmsg(hand, "setCard", i+1, sendmsg(deck, "deal").p);
		}

		sendmsg(hand, "print", 1);

		printf("Play or (q)uit? ");

		if (! fgets(line, sizeof (line), stdin))
			break;

		for (p = line; *p == ' '; p++)
			;

		if (*p == 'q' || *p == 'Q')
			break;
	}

	exit( 0 );
}
