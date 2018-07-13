/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

#include "Deck.h"
#include "Hand.h"
#include "Card.h"

#include <string.h>

class Deck;

static instance i_init(instance self, va_list va) {
	int swe = va_arg(va, int);

	supersend(Deck, self, "init");

	sendmsg(self, "set", "shuffleWhenEmpty", swe ? 1 : 0);

	list cards = list_new();

	for (int s = 0; s < 4; s++)
		for (int r = 0; r < 13; r++)
			list_put(cards, 13*s+r, sendmsg(Card, "new", r, s).p);

	sendmsg(self, "set", "cards", cards);
	sendmsg(self, "set", "top", 0);

	return self;
}

static void i_free(instance self) {
	list cards = sendmsg(self, "get", "cards").p;

	for (int n = 0; n < 52; n++)
		sendmsg(list_get(cards, n), "free");

	list_free(cards);

	supersend(Deck, self, "free");
}

static instance i_shuffle(instance self) {
	list_shuffle(sendmsg(self, "get", "cards").p);

	sendmsg(self, "set", "top", 0);

	return self;
}

static instance i_deal(instance self) {
	list cards = sendmsg(self, "get", "cards").p;
	int top = sendmsg(self, "get", "top").i;

	instance c = list_get(cards, top);

	if (++top >= 52) {
		if (sendmsg(self, "get", "shuffleWhenEmpty").i)
			sendmsg(self, "shuffle");
		top = 0;
	}

	sendmsg(self, "set", "top", top);

	return c;
}

static instance i_hand(instance self) {
	instance card1 = sendmsg(self, "deal").p;
	instance card2 = sendmsg(self, "deal").p;
	instance card3 = sendmsg(self, "deal").p;
	instance card4 = sendmsg(self, "deal").p;
	instance card5 = sendmsg(self, "deal").p;

	return sendmsg(Hand, "new", card1, card2, card3, card4, card5).p;
}

static instance i_setHand(instance self, va_list va) {
	instance hand = va_arg(va, instance);

	instance card1 = sendmsg(self, "deal").p;
	instance card2 = sendmsg(self, "deal").p;
	instance card3 = sendmsg(self, "deal").p;
	instance card4 = sendmsg(self, "deal").p;
	instance card5 = sendmsg(self, "deal").p;

	return sendmsg(hand, "reset", card1, card2, card3, card4, card5).p;
}

static char *i_toString(instance self) {
	static char s[52*2+51+3+1];

	list cards = sendmsg(self, "get", "cards").p;
	int top = sendmsg(self, "get", "top").i;

	char *p = s;

	sprintf(s, "%s -> ", (char *)sendmsg(list_get(cards, top), "toString").p);

	p += strlen(s);

	for (int i = 0; i < list_length(cards); i++) {
		if (i > 0)
			*p++ = ',';
		strcpy(p, sendmsg(list_get(cards, i), "toString").p);
		p += 2;
	}

	return s;
}

void defclassDeck() {
	property _i_properties[] = {
		"cards",
		"top",
		"shuffleWhenEmpty",
		0, 0
	};
	selector _i_messages[] = {
		"init",		METHOD(i_init),
		"free",		METHOD(i_free),
		"shuffle",	METHOD(i_shuffle),
		"deal",		METHOD(i_deal),
		"hand",		METHOD(i_hand),
		"setHand",	METHOD(i_setHand),
		"toString",	METHOD(i_toString),
		0, 0
	};

	Deck = defclass("Deck", 0, 1, 0, _i_properties, 0, _i_messages);
}
