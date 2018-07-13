/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

#include "Application.h"

#include <stdlib.h>

class Echo;

static instance i_echo(instance self, va_list va) {
	char *s = va_arg(va, char *);

	printf( "%s\n", s);

	return self;
}

void defclassEcho() {
	selector _i_messages[] = {
		"echo",	METHOD(i_echo),
		0, 0
	};

	Echo = defclass("Echo", 0, 1, 0, 0, 0, _i_messages);
}

int main( int argc, char *argv[] ) {
	defclassEcho();

	instance echo = (instance)sendmsg(Echo, "new").p;
	sendmsg(echo, "echo", "Adieu !");

	defclassApplication();

	instance app=sendmsg(Application, "new", "Echo", echo).p;
	printf("%s\n", (char *)sendmsg(app, "appName").p);

	sendmsg(app, "echo", "Adieu !");

	sendmsg(app, "hello");

	sendmsg(app, "set", "app", 0);
	sendmsg(app, "echo", "Adieu !");

	exit( 0 );
}
