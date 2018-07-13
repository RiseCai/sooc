/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

#include "Once.h"

class Application;

static instance i_init(instance self, va_list va) {
	char *appName = va_arg(va, char *);
	instance app = va_arg(va, instance);

	supersend(Application, self, "init");

	if (appName) {
		sendmsg(self, "set", "appName", appName);
		if (app)
			sendmsg(self, "set", "app", app);
    }

	return self;
}

static char *i_appName(instance self) {
    return sendmsg(self, "get", "appName").p;
}

static value i_doesNotRecognize(instance self, va_list va) {
	message msg = va_arg(va, message);

	instance app = sendmsg(self, "get", "app").p;

	if (!app)
		return supersend(Application, self, "doesNotRecognize", msg);

    return applymsg(app, msg, va_arg(va, va_list));
}

void defclassApplication() {
	property _i_properties[] = {
		"appName",
		"app",
		0
	};
	selector _i_messages[] = {
		"init",				METHOD(i_init),
		"appName",			METHOD(i_appName),
		"doesNotRecognize",	METHOD(i_doesNotRecognize),
		0, 0
	};

	Application = defclass("Application", Once, 1, 0, _i_properties, 0, _i_messages);
}

