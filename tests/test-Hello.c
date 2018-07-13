/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

#include <stdlib.h>

extern class Hello;

extern void defclassHello();

int main( int argc, char *argv[] ) {
	instance hello;

	defclassHello();

	hello = (instance)sendmsg(Hello, "new").p;

	sendmsg(hello, "hello");

	sendmsg(hello, "free");

	exit( 0 );
}
