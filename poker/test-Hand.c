/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

#include "Card.h"
#include "Hand.h"

#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] ) {
	defclassCard();
	defclassHand();

	instance hand;

	instance card1 = sendmsg(Card, "new", ACE, SPADES).p;
	instance card2 = sendmsg(Card, "new", THREE, CLUBS).p;
	instance card3 = sendmsg(Card, "new", ACE, DIAMONDS).p;
	instance card4 = sendmsg(Card, "new", JACK, HEARTS).p;
	instance card5 = sendmsg(Card, "new", SIX, SPADES).p;

	hand = sendmsg(Hand, "new", card1, card2, card3, card4, card5).p;

	sendmsg(hand, "print", 1);
	sendmsg(hand, "reorder");
	sendmsg(hand, "print", 1);

	sendmsg(sendmsg(hand, "card", 1).p, "print", 1);
	sendmsg(hand, "setCard", 2, sendmsg(Card, "new", ACE, HEARTS));
	sendmsg(hand, "print", 1);

	sendmsg(hand, "free");

	int ntesthands = 9, testhands[9][5][2] = {
		{ JACK, SPADES, KING, DIAMONDS, EIGHT, DIAMONDS, FOUR, HEARTS, TEN, CLUBS },
		{ ACE, SPADES, THREE, CLUBS, FOUR, DIAMONDS, THREE, HEARTS, SIX, SPADES },
		{ SEVEN, SPADES, KING, HEARTS, SEVEN, DIAMONDS, JACK, CLUBS, JACK, SPADES },
		{ FOUR, SPADES, NINE, HEARTS, NINE, DIAMONDS, EIGHT, CLUBS, NINE, SPADES },
		{ KING, HEARTS, JACK, DIAMONDS, QUEEN, CLUBS, TEN, SPADES, ACE, DIAMONDS },
		{ FOUR, HEARTS, NINE, HEARTS, ACE, HEARTS, SEVEN, HEARTS, QUEEN, HEARTS },
		{ FOUR, SPADES, TEN, HEARTS, TEN, DIAMONDS, FOUR, CLUBS, TEN, SPADES },
		{ FIVE, SPADES, FIVE, HEARTS, FIVE, DIAMONDS, QUEEN, CLUBS, FIVE, CLUBS },
		{ KING, DIAMONDS, JACK, DIAMONDS, QUEEN, DIAMONDS, TEN, DIAMONDS, ACE, DIAMONDS },
	};

	for (int h = 0; h < ntesthands; h++) {
		card1 = sendmsg(Card, "new", testhands[h][0][0], testhands[h][0][1]).p;
		card2 = sendmsg(Card, "new", testhands[h][1][0], testhands[h][1][1]).p;
		card3 = sendmsg(Card, "new", testhands[h][2][0], testhands[h][2][1]).p;
		card4 = sendmsg(Card, "new", testhands[h][3][0], testhands[h][3][1]).p;
		card5 = sendmsg(Card, "new", testhands[h][4][0], testhands[h][4][1]).p;

		hand = sendmsg(Hand, "new", card1, card2, card3, card4, card5).p;

		sendmsg(hand, "print", 1);
		sendmsg(hand, "free");
	}

	exit( 0 );
}

