/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

class Responder;

static instance i_addNextResponder(instance self, va_list va) {
	instance r = va_arg(va, instance);

	list responders = sendmsg(self, "get", "nextResponders").p;

	if (!responders)  {
		responders = list_new();

		sendmsg(self, "set", "nextResponders", responders);

		list_put(responders, -1, r);
	}
	else if (list_find(responders, r, 0) == -1)
		list_put(responders, -1, r);

	return self;
}

static instance i_removeNextResponder(instance self, va_list va) {
	instance r = va_arg(va, instance);

	list responders = sendmsg(self, "get", "nextResponders").p;

	if (responders) {
		int i = list_find(responders, r, 0);

		if (i != -1)
			list_delete(responders, i);
	}

	return self;
}

static instance i_respondTo(instance self, va_list va) {
    va_list vac;

    va_copy(vac, va);
    message msg = va_arg(vac, message);

	if (sendmsg(self, "respondsTo", msg).i && applymsg(self, msg, vac).i) {
		va_end(vac);
		return self;
	}

	va_end(vac);

	list responders = sendmsg(self, "get", "nextResponders").p;

	if (responders) {
		for (int i = 0; i < list_length(responders); i++) {
		    va_copy(vac, va);
			applymsg(list_get(responders, i), "respondTo", vac);
			va_end(vac);
		}
	}

	return self;
}

void defclassResponder() {
	property _i_properties[] = {
		"nextResponders",
		0
	};
	selector _i_messages[] = {
		"addNextResponder",		METHOD(i_addNextResponder),
		"removeNextResponder",	METHOD(i_removeNextResponder),
		"respondTo",			METHOD(i_respondTo),
		0, 0
	};

	Responder = defclass("Responder", 0, 1, 0, _i_properties, 0, _i_messages);
}

