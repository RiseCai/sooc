/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "So-o.h"

#include <stdarg.h>

#ifdef TRYCATCH

#include <stdio.h>
#include <setjmp.h>

#define BadReceiver "%s::%s Bad receiver %p"

#endif

class defclass( const char *name, class superclass, unsigned revision, property *class_properties, property *instance_properties, selector *class_messages, selector *instance_messages ) {
	if (!Object)
		defclassObject();

	class c = class_new(name, superclass, revision, class_properties, instance_properties, class_messages, instance_messages);

	class_send_message(c, "initialize");

	return c;
}

value sendmsg( void *rcv, message msg, ... ) {
	va_list va;

	va_start(va, msg);

	value ret = applymsg(rcv, msg, va);

	va_end(va);

	return ret;
}

value applymsg( void *rcv, message msg, va_list va ) {
#ifdef TRYCATCH
	static jmp_buf toplevel;
	static unsigned calldepth = 0;

	if (calldepth == 0) {
		if (setjmp(toplevel) != 0) {
			calldepth = 0;

			fprintf(stderr, BadReceiver, "sendmsg", msg, rcv);
			fprintf(stderr, "\n");

			return (value)0;
		}
	}

	if (!rcv)
		longjmp(toplevel, 1);

	++calldepth;
#endif

	value ret;

	switch (((instance)rcv)->type) {
		case INSTANCE:
			ret = object_send_message_va(rcv, msg, va);
			break;
		case CLASS:
			ret = class_send_message_va(rcv, msg, va);
			break;
		default:
#ifdef TRYCATCH
			longjmp(toplevel, 1);
#else
			ret = (value)0;
			break;
#endif
	}

#ifdef TRYCATCH
	--calldepth;
#endif

	return ret;
}

value supersend( class c, void *rcv, message msg, ... ) {
	va_list va;

	va_start(va, msg);

	value ret = superapply( c, rcv, msg, va );

	va_end(va);

	return ret;
}

value superapply( class c, void *rcv, message msg, va_list va ) {
#ifdef TRYCATCH
	static jmp_buf toplevel;
	static unsigned calldepth = 0;

	if (calldepth == 0) {
		if (setjmp(toplevel) != 0) {
			calldepth = 0;

			fprintf(stderr, BadReceiver, "supersend", msg, rcv);
			fprintf(stderr, "\n");

			return (value)0;
		}
	}

	if (!c)
		longjmp(toplevel, 1);

	if (!rcv)
		longjmp(toplevel, 1);

	++calldepth;
#endif

	value ret;

	switch (((instance)rcv)->type) {
		case INSTANCE:
			ret = object_super_send_message_va(c, rcv, msg, va);
			break;
		case CLASS:
			ret = class_super_send_message_va(c, rcv, msg, va);
			break;
		default:
#ifdef TRYCATCH
			longjmp(toplevel, 1);
#else
			ret = (value)0;
			break;
#endif
	}

#ifdef TRYCATCH
	--calldepth;
#endif

	return ret;
}

