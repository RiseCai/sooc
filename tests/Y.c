/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

class Y;

static instance i_init(instance self) {
	printf("Y::init\n");

	supersend(Y, self, "init");

	return self;
}

static instance i_f(instance self, va_list va) {
	int n = va_arg(va, int);

	printf("%d\n", n*2);

	supersend(Y, self, "f", n);

	return self;
}

static instance i_g(instance self) {
	printf("Y::g\n");

	return self;
}

static instance i_echo(instance self, va_list va) {
	const char *s = va_arg(va, char *);

	printf("%s\n", s);

	return self;
}

void defclassY() {
	selector _i_messages[] = {
		"init",		METHOD(i_init),
		"f",		METHOD(i_f),
		"g",		METHOD(i_g),
		"echo",		METHOD(i_echo),
		0, 0
	};

	extern class X;

	Y = defclass("Y", X, 1, 0, 0, 0, _i_messages);
}
