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

void print_proplist(list l) {
	void **p;

	if ( !l || !l->l) {
		printf("(nil)\n");
		return;
	}

	for ( p = l->v; p < l->v + l->l; p++ )
		printf("%s\n", (char *)*p);
}

void print_msglist(alist l) {
	void **p;

	if ( !l || !l->l) {
		printf("(nil)\n");
		return;
	}

	for ( p = l->v; p < l->v + l->l * 2; p += 2 )
		printf("%s\n", (char *)*p);
}

int main( int argc, char *argv[] ) {
	value v;

	defclassObject();

	defclassX();
	defclassY();
	defclassZ();

	printf("%s\n", class_tostring(Object));
	printf("%s\n", class_tostring(X));
	printf("%s\n", class_tostring(Y));
	printf("%s\n", class_tostring(Z));

	printf("%s\n", class_name(Object));
	printf("%d\n", class_revision(Object));
	printf("%p\n", class_superclass(Object));
	printf("%s\n", class_name(X));
	printf("%s\n", class_name(class_superclass(X)));
	printf("%s\n", class_name(Y));
	printf("%s\n", class_name(class_superclass(Y)));

	printf(class_is_kind_of(Object, Object) ? "true\n" : "false\n");
	printf(class_is_kind_of(X, Object) ? "true\n" : "false\n");
	printf(class_is_kind_of(Object, X) ? "true\n" : "false\n");
	printf(class_is_kind_of(Y, X) ? "true\n" : "false\n");
	printf(class_is_kind_of(Y, Object) ? "true\n" : "false\n");

	print_proplist( class_class_properties(Object) );
	print_proplist( class_instance_properties(Object) );
	print_msglist( class_class_messages(Object) );
	print_msglist( class_instance_messages(Object) );

	print_proplist( class_instance_properties(X) );
	print_msglist( class_instance_messages(X) );

	class_add_class_property(Object, "foobar");
	print_proplist( class_class_properties(Object) );
	class_remove_class_property(Object, "foobar");
	print_proplist( class_class_properties(Object) );
	class_add_class_message(Object, "foobar", 0);
	class_remove_class_message(Object, "foobar");

	class_add_instance_property(Object, "foobar");
	print_proplist( class_instance_properties(Object) );
	class_remove_instance_property(Object, "foobar");
	print_proplist( class_instance_properties(Object) );
	class_add_instance_message(Object, "foobar", 0);
	class_remove_instance_message(Object, "foobar");

	print_msglist( class_attributes(Object) );

	class_add_class_property(Object, "debug");
	printf("%p\n", class_find_class_property(Object, "debug"));

	v = class_get(Object, "debug");
	printf("%d\n", v.i);
	class_set(Object, "debug", (value)1);
	v = class_get(Object, "debug");
	printf("%d\n", v.i);
	class_set(Object, "debug", (value)0);
	v = class_get(Object, "debug");
	printf("%d\n", v.i);

	instance obj, x, y,z;

	obj = class_make(Object);
	printf("%s\n", class_name(object_class(obj)));

	x = class_make(X);
	printf("%s\n", class_name(object_class(x)));
	printf("%s\n", class_name(object_superclass(x)));

	y = class_make(Y);
	printf("%s\n", class_name(object_class(y)));
	printf("%s\n", class_name(object_superclass(y)));

	object_assume(obj, X);
	printf("%s\n", class_name(object_class(obj)));

	object_free(obj);

	obj = class_make(Object);

	v = object_get(x, "value");
	printf("%d\n", v.i);
	v.i = 1;
	object_set(x, "value", v);
	v = object_get(x, "value");
	printf("%d\n", v.i);
	v.i = 2;
	object_set(y, "value", v);
	v = object_get(y, "value");
	printf("%d\n", v.i);

	object_free(obj);
	obj = object_copy(x);
	object_free(x);
	v = object_get(obj, "value");
	printf("%d\n", v.i);

	class_send_message(Y, "foobar");
	object_send_message(y, "foobar");

	class_send_message(Y, "get", "foobar");
	object_send_message(y, "get", "foobar");

	v = object_send_message(y, "get", "value");
	printf("%d\n", v.i);
	object_send_message(y, "set", "value", -1);
	v = object_send_message(y, "get", "value");
	printf("%d\n", v.i);

	exit( 0 );
}
