/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

#include "Card.h"

class Card;

static instance i_init(instance self, va_list va) {
	int rank = va_arg(va, int);
	int suit = va_arg(va, int);

	supersend(Card, self, "init");

	sendmsg(self, "set", "rank", rank);
	sendmsg(self, "set", "suit", suit);

	return self;
}

static int i_rank(instance self) {
	return sendmsg(self, "get", "rank").i;
}

static int i_suit(instance self) {
	return sendmsg(self, "get", "suit").i;
}

static int i_compare(instance self, va_list va) {
	instance card = va_arg(va, instance);

	int rank1 = sendmsg(self, "get", "rank").i;
	int rank2 = sendmsg(card, "get", "rank").i;

	return rank1 == rank2 ? 0 : rank1 > rank2 ? 1 : -1;
}

static char *i_toString(instance self) {
	static char srank[] = { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' };
	static char ssuit[] = { 'c', 'd', 'h', 's' };

	static char s[2+1];

	int rank = sendmsg(self, "get", "rank").i;
	int suit = sendmsg(self, "get", "suit").i;

	sprintf(s, "%c%c", srank[rank], ssuit[suit]);

	return s;
}

void defclassCard() {
	property _i_properties[] = {
		"rank",
		"suit",
		0, 0
	};
	selector _i_messages[] = {
		"init",		METHOD(i_init),
		"rank",		METHOD(i_rank),
		"suit",		METHOD(i_suit),
		"compare",	METHOD(i_compare),
		"toString",	METHOD(i_toString),
		0, 0
	};

	Card = defclass("Card", 0, 1, 0, _i_properties, 0, _i_messages);
}
