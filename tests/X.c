/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

class X;

static class c_initialize(class self) {
	return sendmsg(self, "set", "count", 0).p;
}

static instance c_new(class self) {
	instance i = supersend(X, self, "new").p;

	sendmsg(self, "incr");

	return i;
}

static int c_count(class self) {
	return sendmsg(self, "get", "count").i;
}

static class c_incr(class self) {
	return sendmsg(self, "set", "count", sendmsg(self, "get", "count").i + 1).p;
}

static class c_decr(class self) {
	return sendmsg(self, "set", "count", sendmsg(self, "get", "count").i - 1).p;
}

static void i_free(instance self) {
	sendmsg(sendmsg(self, "class").p, "decr");

	supersend(X, self, "free");
}

static instance i_init(instance self) {
	printf("X::init\n");

	supersend(X, self, "init");

	sendmsg(self, "set", "value", 0);

	return self;
}

static instance i_f(instance self, va_list va) {
	int n = va_arg(va, int);

	printf("%d\n", n);

	if (n > 0)
		sendmsg(self, "f", n-1);

	return self;
}

static instance i_g(instance self) {
	printf("X::g\n");

	return self;
}

static value i_value(instance self) {
	return sendmsg(self, "get", "value");
}

void defclassX() {
	property _c_properties[] = {
		"count",
		0
	};
	property _i_properties[] = {
		"value",
		0
	};
	selector _c_messages[] = {
		"initialize",	METHOD(c_initialize),
		"new",			METHOD(c_new),
		"incr",			METHOD(c_incr),
		"decr",			METHOD(c_decr),
		"count",		METHOD(c_count),
		0, 0
	};
	selector _i_messages[] = {
		"free",		METHOD(i_free),
		"init",		METHOD(i_init),
		"value",	METHOD(i_value),
		"f",		METHOD(i_f),
		"g",		METHOD(i_g),
		0, 0
	};

	X = defclass("X", 0, 1, _c_properties, _i_properties, _c_messages, _i_messages);
}
