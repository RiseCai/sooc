/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

#include "Card.h"

#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] ) {
	defclassCard();

	instance card_2c = (instance)sendmsg(Card, "new", TWO, CLUBS).p;
	instance card_Td = (instance)sendmsg(Card, "new", TEN, DIAMONDS).p;
	instance card_Kh = (instance)sendmsg(Card, "new", KING, HEARTS).p;
	instance card_As = (instance)sendmsg(Card, "new", ACE, SPADES).p;

	printf("2c (two of clubs) -> ");
	sendmsg(card_2c, "print", 1);

	printf("Td (ten of diamonds) -> ");
	sendmsg(card_Td, "print", 1);

	printf("Kh (king of hearts) -> ");
	sendmsg(card_Kh, "print", 1);

	printf("As (ace of spades) -> ");
	sendmsg(card_As, "print", 1);

	printf("-1 -> %2d\n", sendmsg(card_Kh, "compare", card_As).i);
	printf(" 0 -> %2d\n", sendmsg(card_2c, "compare", card_2c).i);
	printf(" 1 -> %2d\n", sendmsg(card_Kh, "compare", card_Td).i);

	exit( 0 );
}

