/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

class Once;

static instance c_new(class self, va_list va) {
	instance i = sendmsg(self, "get", "instance").p;

	if (!i) {
		i = superapply(Once, self, "new", va).p;
		sendmsg(self, "set", "instance", i);
	}

	return i;
}

void defclassOnce() {
	property _c_properties[] = {
		"instance",
		0
	};
	selector _c_messages[] = {
		"new",	METHOD(c_new),
		0, 0
	};

	Once = defclass("Once", 0, 1, _c_properties, 0, _c_messages, 0);
}
