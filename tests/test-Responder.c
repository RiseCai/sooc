/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

#include "Responder.h"

#include <stdlib.h>

class Button;

static instance i_click(instance self) {
    printf("%s click\n", (char *)sendmsg(self, "toString").p);

    sendmsg(self, "respondTo", "clicked", self);

    return self;
}

void defclassButton() {
	selector _i_messages[] = {
		"click",	METHOD(i_click),
		0, 0
	};

	Button = defclass("Button", Responder, 1, 0, 0, 0, _i_messages);
}

class Action;

static int i_clicked0(instance self, va_list va) {
	instance sender = va_arg(va, instance);

	printf("%s", (char *)sendmsg(self, "toString").p);
	printf(" clicked from ");
	printf("%s", (char *)sendmsg(sender, "toString").p);
	printf("\n");

    return 0;
}

void defclassAction() {
	selector _i_messages[] = {
		"clicked",	METHOD(i_clicked0),
		0, 0
	};

	Action = defclass("Action", Responder, 1, 0, 0, 0, _i_messages);
}

class AnotherAction;

static int i_clicked1(instance self, va_list va) {
	instance sender = va_arg(va, instance);

	printf("%s", (char *)sendmsg(self, "toString").p);
	printf(" clicked from ");
	printf("%s", (char *)sendmsg(sender, "toString").p);
	printf("\n");

    return 1;
}

void defclassAnotherAction() {
	selector _i_messages[] = {
		"clicked",	METHOD(i_clicked1),
		0, 0
	};

	AnotherAction = defclass("AnotherAction", Responder, 1, 0, 0, 0, _i_messages);
}

int main( int argc, char *argv[] ) {
	defclassResponder();

	defclassButton();
	defclassAction();

	instance button = (instance)sendmsg(Button, "new").p;

	sendmsg(button, "click");

	instance action = (instance)sendmsg(Action, "new").p;

	sendmsg(button, "addNextResponder", action);

	sendmsg(button, "click");

	defclassAnotherAction();

	instance anotheraction = (instance)sendmsg(AnotherAction, "new").p;

	sendmsg(button, "addNextResponder", anotheraction);

	sendmsg(button, "click");

	sendmsg(button, "removeNextResponder", anotheraction);

	sendmsg(button, "click");

	sendmsg(action, "addNextResponder", anotheraction);

	sendmsg(button, "click");

	sendmsg(button, "removeNextResponder", action);
	sendmsg(action, "removeNextResponder", anotheraction);

	sendmsg(button, "addNextResponder", anotheraction);
	sendmsg(anotheraction, "addNextResponder", action);

	sendmsg(button, "click");

	exit( 0 );
}
