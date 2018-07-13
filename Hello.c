/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

class Hello;

static instance i_hello(instance self) {
	printf( "Hello from So-o!\n" );

	return self;
}

void defclassHello() {
	selector _i_messages[] = {
		"hello",	METHOD(i_hello),
		0, 0
	};

	Hello = defclass("Hello", 0, 1, 0, 0, 0, _i_messages);
}
