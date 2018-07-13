/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

class Z;

static instance i_init(instance self) {
	printf("Z::init\n");

	supersend(Z, self, "init");

	return self;
}

void defclassZ() {
	selector _i_messages[] = {
		"init",		METHOD(i_init),
		0, 0
	};

	extern class Y;

	Z = defclass("Z", Y, 1, 0, 0, 0, _i_messages);
}
