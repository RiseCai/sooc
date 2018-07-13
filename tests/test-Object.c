/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

#include "X.h"
#include "Y.h"
#include "Z.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

static void display_mallinfo( void ) {
	struct mallinfo mi;

	mi = mallinfo();

	printf("\n");
	printf("Total non-mmapped bytes (arena):       %d\n", mi.arena);
	printf("# of free chunks (ordblks):            %d\n", mi.ordblks);
//	printf("# of free fastbin blocks (smblks):     %d\n", mi.smblks);
//	printf("# of mapped regions (hblks):           %d\n", mi.hblks);
//	printf("Bytes in mapped regions (hblkhd):      %d\n", mi.hblkhd);
//	printf("Max. total allocated space (usmblks):  %d\n", mi.usmblks);
//	printf("Free bytes held in fastbins (fsmblks): %d\n", mi.fsmblks);
	printf("Total allocated space (uordblks):      %d\n", mi.uordblks);
	printf("Total free space (fordblks):           %d\n", mi.fordblks);
	printf("Topmost releasable block (keepcost):   %d\n", mi.keepcost);
	printf("\n");
}

static int c_supercount(class self) {
	return supersend(Y, self, "count").i;
}

static instance i_foobar(instance self) {
	printf("%s::foobar\n", (char *)sendmsg(sendmsg(self, "class").p, "name").p);

	return self;
}

static void print_strings(char **strings) {
	for ( char **p = strings; *p; p++ )
		printf("%s\n", *p);
}

int main( int argc, char *argv[] ) {
	char **p;
	value v;

	instance x, y,z;

	defclassObject();

	defclassX();
	defclassY();
	defclassZ();

	display_mallinfo();

	x = (instance)sendmsg(X, "new").p;
	y = (instance)sendmsg(Y, "new").p;
	z = (instance)sendmsg(Z, "new").p;

	printf("%s\n", (char *)sendmsg(X, "name").p);
	printf("%d\n", sendmsg(X, "revision").i);
	printf("%d\n", sendmsg(X, "count").i);

	printf("%s\n", (char *)sendmsg(X, "toString").p);
	printf("%s\n", (char *)sendmsg(x, "toString").p);

	printf("%s\n", (char *)sendmsg(sendmsg(Y, "superclass").p, "name").p);

	instance x2 = sendmsg(X, "new").p;
	printf("%d\n", sendmsg(X, "count").i);
	sendmsg(x2, "free");
	printf("%d\n", sendmsg(X, "count").i);

	printf("%s\n", (char *)sendmsg(sendmsg(x, "class").p, "name").p);
	sendmsg(x, "f", 4);
	sendmsg(y, "f", 4);

	printf("%s\n", (char *)sendmsg(sendmsg(y, "class").p, "name").p);
	printf("%s\n", (char *)sendmsg(sendmsg(y, "superclass").p, "name").p);
	printf("%d\n", sendmsg(Y, "revision").i);
	printf("%d\n", sendmsg(Y, "count").i);

	printf("%p\n", sendmsg(X, "classMethodFor", "count").p);
	printf("%p\n", sendmsg(Y, "classMethodFor", "count").p);
	printf("%p\n", sendmsg(Y, "classMethodFor", "foobar").p);

	printf("%p\n", sendmsg(X, "instanceMethodFor", "f").p);
	printf("%p\n", sendmsg(x, "methodFor", "f").p);
	printf("%p\n", sendmsg(Y, "instanceMethodFor", "f").p);
	printf("%p\n", sendmsg(y, "methodFor", "f").p);
	printf("%p\n", sendmsg(Y, "instanceMethodFor", "foobar").p);
	printf("%p\n", sendmsg(y, "methodFor", "foobar").p);

	printf(sendmsg(y, "respondsTo", "free").i ? "true\n" : "false\n");
	printf(sendmsg(y, "respondsTo", "foobar").i ? "true\n" : "false\n");

	for (p = (char **)sendmsg(X, "classMessages").p; *p; p++)
		printf("%s\n", *p);
	for (p = (char **)sendmsg(X, "instanceMessages").p; *p; p++)
		printf("%s\n", *p);

	sendmsg(X, "addInstanceProperty", "delegate");
	printf("%p\n", sendmsg(x, "getDelegate").p);
	sendmsg(x, "setDelegate", y);
	printf("%p\n", sendmsg(x, "getDelegate").p);
	sendmsg(y, "echo", "Hello, world!");
	sendmsg(x, "delegate", "echo", "Hello, world!");
	sendmsg(x, "delegate", "foobar");

	sendmsg(Y, "addClassMessage", "supercount", c_supercount);
	printf("%d\n", sendmsg(Y, "supercount").i);
	sendmsg(Y, "removeClassMessage", "supercount");
	sendmsg(Y, "supercount");

	sendmsg(Y, "addInstanceMessage", "foobar", i_foobar);
	sendmsg(y, "foobar");
	sendmsg(Y, "removeInstanceMessage", "foobar");
	sendmsg(y, "foobar");

	p = sendmsg(X, "classProperties", 0).p;
	print_strings(p);
	free(p);

	p = sendmsg(X, "classProperties", 1).p;
	print_strings(p);
	free(p);

	sendmsg(X, "addClassProperty", "supercount");
	p = sendmsg(Y, "classProperties", 1).p;
	print_strings(p);
	free(p);

	p = sendmsg(X, "classMessages", 0).p;
	print_strings(p);
	free(p);

	p = sendmsg(X, "classMessages", 1).p;
	print_strings(p);
	free(p);

	p = sendmsg(y, "properties", 1).p;
	print_strings(p);
	free(p);

	p = sendmsg(y, "messages", 1).p;
	print_strings(p);
	free(p);

	sendmsg(x, "free");
	sendmsg(y, "free");
	sendmsg(z, "free");

	printf("%d\n", sendmsg(X, "count").i);

	display_mallinfo();

	exit( 0 );
}

