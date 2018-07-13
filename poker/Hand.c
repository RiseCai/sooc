/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

#include "Hand.h"

#include <string.h>

class Hand;

static int cardcmp(instance card1, instance card2) {
	return sendmsg(card1, "compare", card2).i;
}

static instance i_init(instance self, va_list va) {
	instance card1 = va_arg(va, instance);
	instance card2 = va_arg(va, instance);
	instance card3 = va_arg(va, instance);
	instance card4 = va_arg(va, instance);
	instance card5 = va_arg(va, instance);

	supersend(Hand, self, "init");

	list cards = list_new();

	list_put(cards, 0, card1);
	list_put(cards, 1, card2);
	list_put(cards, 2, card3);
	list_put(cards, 3, card4);
	list_put(cards, 4, card5);

	sendmsg(self, "set", "cards", cards);

	return self;
}

static void i_free(instance self) {
	list cards = sendmsg(self, "get", "cards").p;

	list_free(cards);

	supersend(Hand, self, "free");
}

static instance i_copy(instance self) {
	instance copy = supersend(Hand, self, "copy").p;

	sendmsg(copy, "set", "cards", list_copy(sendmsg(self, "get", "cards").p));

	return copy;
}

static instance i_reorder(instance self) {
	list cards = sendmsg(self, "get", "cards").p;

	list_sort(cards, (int (*)(void *, void *))cardcmp, 1);

	return self;
}

static instance i_reset(instance self, va_list va) {
	instance card1 = va_arg(va, instance);
	instance card2 = va_arg(va, instance);
	instance card3 = va_arg(va, instance);
	instance card4 = va_arg(va, instance);
	instance card5 = va_arg(va, instance);

	list cards = sendmsg(self, "get", "cards").p;

	list_put(cards, 0, card1);
	list_put(cards, 1, card2);
	list_put(cards, 2, card3);
	list_put(cards, 3, card4);
	list_put(cards, 4, card5);

	return self;
}

static instance i_card(instance self, va_list va) {
	int n = va_arg(va, int);

	if (n < 1 || n > 5) {
		return sendmsg(self, "invalidArgument", "card").p;
	}

	list cards = sendmsg(self, "get", "cards").p;

	return (instance)list_get(cards, n-1);
}

static instance i_setCard(instance self, va_list va) {
	int n = va_arg(va, int);
	instance card = va_arg(va, instance);

	if (n < 1 || n > 5) {
		return sendmsg(self, "invalidArgument", "setCard").p;
	}

	list cards = sendmsg(self, "get", "cards").p;

	list_put(cards, n-1, card);

	return self;
}

static char *i_toString(instance self) {
	static char *handnames[] = { "NOTHING", "ONEPAIR", "TWOPAIRS", "THREEOFKIND", "STRAIGHT", "FLUSH", "FULLHOUSE", "FOUROFKIND", "STRAIGHTFLUSH" };

	static char s[5*2+4+4+13+1];

	char *p = s;

	int eval = sendmsg(self, "evaluate").i;

	list cards = sendmsg(self, "get", "cards").p;

	for (int i = 0; i < list_length(cards); i++) {
		if (i > 0)
			*p++ = ',';
		strcpy(p, (char *)sendmsg((instance)list_get(cards, i), "toString").p);
		p += 2;
	}

	sprintf(p, " -> %s", handnames[eval]);

	return s;
}

static int i_evaluate(instance self) {
	// sort or nothing works but don't reoder the hand!
	list cards = list_sort(list_copy(sendmsg(self, "get", "cards").p), (int (*)(void *, void *))cardcmp, 1);

	instance card1 = list_get(cards, 0);
	instance card2 = list_get(cards, 1);
	instance card3 = list_get(cards, 2);
	instance card4 = list_get(cards, 3);
	instance card5 = list_get(cards, 4);

	int r1 = sendmsg(card1, "rank").i;
	int r2 = sendmsg(card2, "rank").i;
	int r3 = sendmsg(card3, "rank").i;
	int r4 = sendmsg(card4, "rank").i;
	int r5 = sendmsg(card5, "rank").i;

	int s1 = sendmsg(card1, "suit").i;
	int s2 = sendmsg(card2, "suit").i;
	int s3 = sendmsg(card3, "suit").i;
	int s4 = sendmsg(card4, "suit").i;
	int s5 = sendmsg(card5, "suit").i;

	int ret = NOTHING;

	// STRAIGHTFLUSH - a(a+1)(a+2)(a+3)(a+4)

	if (r5 == r4+1 && r4 == r3+1 && r3 == r2+1 && r2 == r1+1 &&
		s1 == s2 && s2 == s3 && s3 == s4 && s4 == s5)
		ret = STRAIGHTFLUSH;

	// FOUROFKIND - aaaab abbbb

	else if (r1 == r2 && r2 == r3 && r3 == r4)
		ret = FOUROFKIND;

	else if (r2 == r3 && r3 == r4 && r4 == r5)
		ret = FOUROFKIND;

	// FULLHOUSE - aaabb aabbb

	else if (r1 == r2 && r2 == r3 && r3 != r4 && r4 == r5)
		ret = FULLHOUSE;

	else if (r1 == r2 && r2 != r3 && r3 == r4 && r4 == r5)
		ret = FULLHOUSE;

	// FLUSH

	else if (s1 == s2 && s2 == s3 && s3 == s4 && s4 == s5)
		ret = FLUSH;		// could be a straight flush

	// STRAIGHT - a(a+1)(a+2)(a+3)(a+4)

	else if (r5 == r4+1 && r4 == r3+1 && r3 == r2+1 && r2 == r1+1)
		ret = STRAIGHT;	// could be a straight flush

	// THREEOFKIND - aaabc abbbc abccc

	else if (r1 == r2 && r2 == r3 && r3 != r4 && r4 != r5)
		ret = THREEOFKIND;

	else if (r1 != r2 && r2 == r3 && r3 == r4 && r4 != r5)
		ret = THREEOFKIND;

	else if (r1 != r2 && r2 != r3 && r3 == r4 && r4 == r5)
		ret = THREEOFKIND;

	// TWOPAIRS - aabbc aabcc abbcc

	else if (r1 == r2 && r2 != r3 && r3 == r4 && r4 != r5)
		ret = TWOPAIRS;

	else if (r1 == r2 && r2 != r3 && r3 != r4 && r4 == r5)
		ret = TWOPAIRS;

	else if (r1 != r2 && r2 == r3 && r3 != r4 && r4 == r5)
		ret = TWOPAIRS;

	// ONEPAIR - aabcd abbcd abccd abcdd

	else if (r1 == r2 && r2 != r3 && r3 != r4 && r4 != r5)
		ret = ONEPAIR;

	else if (r1 != r2 && r2 == r3 && r3 != r4 && r4 != r5)
		ret = ONEPAIR;

	else if (r1 != r2 && r2 != r3 && r3 == r4 && r4 != r5)
		ret = ONEPAIR;

	else if (r1 != r2 && r2 != r3 && r3 != r4 && r4 == r5)
		ret = ONEPAIR;

	list_free(cards);

	return ret;
}

void defclassHand() {
	property _i_properties[] = {
		"cards",
		0, 0
	};
	selector _i_messages[] = {
		"init",				METHOD(i_init),
		"free",				METHOD(i_free),
		"copy",				METHOD(i_copy),
		"reorder",			METHOD(i_reorder),
		"reset",			METHOD(i_reset),
		"card",				METHOD(i_card),
		"setCard",			METHOD(i_setCard),
		"evaluate",			METHOD(i_evaluate),
		"toString",			METHOD(i_toString),
		0, 0
	};

	Hand = defclass("Hand", 0, 1, 0, _i_properties, 0, _i_messages);
}
