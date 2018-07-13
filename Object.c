/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "OL.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define InvalidClassProperty "%s::%s Invalid class property"
#define InvalidInstanceProperty "%s::%s Invalid instance property"
#define InvalidClassMessage "%s::%s Invalid class message"
#define InvalidInstanceMessage "%s::%s Invalid instance message"

#define NotImplemented "%s::%s Not implemented"
#define SubclassResponsibility "%s::%s Subclass responsibility"

#define InvalidArgument "%s::%s Invalid argument"

// class methods

static void c_free(class self) {
	class_free(self);
}

static class c_initialize (class self) {
	return self;
}

static class c_class(class self) {
	return self;
}

static const char* c_name(class self) {
	return class_name(self);
}

static unsigned int c_revision(class self) {
	return class_revision(self);
}

static class c_superclass(class self) {
	return class_superclass(self);
}

static value c_get(class self, va_list va) {
	property prop = va_arg(va, property);

	return class_get(self, prop);
}

static class c_set(class self, va_list va) {
	property prop = va_arg(va, property);
	value val = va_arg(va, value);

	return class_set(self, prop, val);
}

static instance c_make(class self) {
	return class_make(self);
}

static instance c_new(class self, va_list va) {
	return (instance)object_send_message_va((instance)class_send_message(self, "make").p, "init", va).p;
}

static char *c_toString(class self) {
	return class_tostring(self);
}

static class c_addClassMessage(class self, va_list va) {
	message msg = va_arg(va, message);
	method func = va_arg(va, method);

	return class_add_class_message(self, msg, func);
}

static class c_removeClassMessage(class self, va_list va) {
	message msg = va_arg(va, message);

	return class_remove_class_message(self, msg);
}

static class c_addInstanceMessage(class self, va_list va) {
	message msg = va_arg(va, message);
	method func = va_arg(va, method);

	return class_add_instance_message(self, msg, func);
}

static class c_removeInstanceMessage(class self, va_list va) {
	message msg = va_arg(va, message);

	return class_remove_instance_message(self, msg);
}

static class c_addClassProperty(class self, va_list va) {
	property prop = va_arg(va, property);

	return class_add_class_property(self, prop);
}

static class c_removeClassProperty(class self, va_list va) {
	property prop = va_arg(va, property);

	return class_remove_class_property(self, prop);
}

static class c_addInstanceProperty(class self, va_list va) {
	property prop = va_arg(va, property);

	return class_add_instance_property(self, prop);
}

static class c_removeInstanceProperty(class self, va_list va) {
	property prop = va_arg(va, property);

	return class_remove_instance_property(self, prop);
}

static class c_error (class self, va_list va) {
	const char *err = va_arg(va, char *);

	vfprintf(stderr, err, va);

	fprintf(stderr, "\n");

	return self;
}

static class c_doesNotContain(class self, va_list va) {
	message msg = va_arg(va, message);

	class_send_message(self, "error", InvalidClassProperty, class_name(self), msg);

	return self;
}

static class c_doesNotRecognize(class self, va_list va) {
	message msg = va_arg(va, message);

	class_send_message(self, "error", InvalidClassMessage, class_name(self), msg);

	return self;
}

static class c_notImplemented(class self, va_list va) {
	message msg = va_arg(va, message);

	class_send_message(self, "error", NotImplemented, class_name(self), msg);

	return self;
}

static class c_subclassResponsibility(class self, va_list va) {
	message msg = va_arg(va, message);

	class_send_message(self, "error", SubclassResponsibility, class_name(self), msg);

	return self;
}

static class c_invalidArgument(class self, va_list va) {
	message msg = va_arg(va, message);

	class_send_message(self, "error", InvalidArgument, class_name(self), msg);

	return self;
}

static method c_classMethodFor(class self, va_list va) {
	message msg = va_arg(va, message);

	return class_find_class_method(self, msg);
}

static method c_instanceMethodFor(class self, va_list va) {
	message msg = va_arg(va, message);

	return class_find_instance_method(self, msg);
}

static message *c_classMessages(class self, va_list va) {
	int inherit = va_arg(va, int);

	alist ml = alist_new();
	class c = self;

	do {
		alist msglist = class_class_messages(c);

		if (msglist)
			alist_merge(ml, msglist);

		if (!inherit)
			break;

		c = class_superclass(c);
	}
	while (c);

	message *msgs = (message *)alist_keys(ml);

	alist_free(ml);

	return msgs;
}

static message *c_instanceMessages(class self, va_list va) {
	int inherit = va_arg(va, int);

	alist ml = alist_new();
	class c = self;

	do {
		alist msglist = class_instance_messages(c);

		if (msglist)
			alist_merge(ml, msglist);

		if (!inherit)
			break;

		c = class_superclass(c);
	}
	while (c);

	message *msgs = (message *)alist_keys(ml);

	alist_free(ml);

	return msgs;
}

static property *c_classProperties(class self, va_list va) {
	int inherit = va_arg(va, int);

	list pl = list_new();
	class c = self;

	do {
		list proplist = class_class_properties(c);

		if (proplist)
			list_merge(pl, proplist);

		if (!inherit)
			break;

		c = class_superclass(c);
	}
	while (c);

	property *props = (property *)list_values(pl);

	list_free(pl);

	return props;
}

static property *c_instanceProperties(class self, va_list va) {
	int inherit = va_arg(va, int);

	list pl = list_new();
	class c = self;

	do {
		list proplist = class_instance_properties(c);

		if (proplist)
			list_merge(pl, proplist);

		if (!inherit)
			break;

		c = class_superclass(c);
	}
	while (c);

	property *props = (property *)list_values(pl);

	list_free(pl);

	return props;
}

// instance methods

static void i_free(instance self) {
	object_free(self);
}

static instance i_init(instance self, va_list va) {
	return self;
}

static class i_class(instance self) {
	return object_class(self);
}

static class i_superclass(instance self) {
	return class_superclass(object_class(self));
}

static instance i_assume(instance self, va_list va) {
	class c = va_arg(va, class);

	return object_assume(self, c);
}

static int i_isKindOf(instance self, va_list va) {
	class c = va_arg(va, class);

	return class_is_kind_of(object_class(self), c);
}

static method i_methodFor(instance self, va_list va) {
	message msg = va_arg(va, message);

	return class_find_instance_method(object_class(self), msg);
}

static int i_respondsTo(instance self, va_list va) {
	return i_methodFor(self, va) ? 1 : 0;
}

static value i_get(instance self, va_list va) {
	property prop = va_arg(va, property);

	return object_get(self, prop);
}

static instance i_set(instance self, va_list va) {
	property prop = va_arg(va, property);
	value val = va_arg(va, value);

	return object_set(self, prop, val);
}

static instance i_copy(instance self) {
	return object_copy(self);
}

static char *i_toString(instance self) {
	return object_tostring(self);
}

static instance i_print(instance self, va_list va) {
	int eol = va_arg(va, int);

	printf("%s", (char *)object_send_message(self, "toString").p);

	if (eol)
		printf("\n");

	return self;
}

static value i_delegate(instance self, va_list va) {
	instance delegate=object_get(self, "delegate").p;

	if (!delegate) {
		return (value)0;
	}

	message msg = va_arg(va, message);

	if (!object_send_message(delegate, "respondsTo", msg).i) {
		return (value)0;
	}

	return object_send_message_va(delegate, msg, va);
}

static instance i_setDelegate(instance self, va_list va) {
	instance delegate = va_arg(va, instance);

	return object_set(self, "delegate", (value)((void *)delegate));
}

static instance i_getDelegate(instance self) {
	return object_get(self, "delegate").p;
}

static instance i_error (instance self, va_list va) {
	char *err = va_arg(va, char *);

	vfprintf(stderr, err, va);

	fprintf(stderr, "\n");

	return self;
}

static instance i_doesNotContain(instance self, va_list va) {
	message msg = va_arg(va, message);

	object_send_message(self, "error", InvalidInstanceProperty, class_name(object_class(self)), msg);

	return self;
}

static instance i_doesNotRecognize(instance self, va_list va) {
	message msg = va_arg(va, message);

	object_send_message(self, "error", InvalidInstanceMessage, class_name(object_class(self)), msg);

	return self;
}

static instance i_notImplemented(instance self, va_list va) {
	message msg = va_arg(va, message);

	object_send_message(self, "error", NotImplemented, class_name(object_class(self)), msg);

	return self;
}

static instance i_subclassResponsibility(instance self, va_list va) {
	message msg = va_arg(va, message);

	object_send_message(self, "error", SubclassResponsibility, class_name(object_class(self)), msg);

	return self;
}

static instance i_invalidArgument(instance self, va_list va) {
	message msg = va_arg(va, message);

	object_send_message(self, "error", InvalidArgument, class_name(object_class(self)), msg);

	return self;
}

static message *i_messages(instance self, va_list va) {
	return class_send_message_va(object_class(self), "instanceMessages", va).p;
}

static property *i_properties(instance self, va_list va) {
	return class_send_message_va(object_class(self), "instanceProperties", va).p;
}

// interface

class Object;

void defclassObject() {
	selector _c_messages[] = {
		"get",						METHOD(c_get),
		"set",						METHOD(c_set),
		"make",						METHOD(c_make),
		"new",						METHOD(c_new),
		"initialize",				METHOD(c_initialize),
		"free",						METHOD(c_free),
		"class",					METHOD(c_class),
		"name",						METHOD(c_name),
		"revision",					METHOD(c_revision),
		"superclass",				METHOD(c_superclass),
		"toString",					METHOD(c_toString),
		"error",					METHOD(c_error),
		"doesNotContain",			METHOD(c_doesNotContain),
		"doesNotRecognize",			METHOD(c_doesNotRecognize),
		"notImplemented",			METHOD(c_notImplemented),
		"subclassResponsibility",	METHOD(c_subclassResponsibility),
		"invalidArgument",			METHOD(c_invalidArgument),
		"classMethodFor",			METHOD(c_classMethodFor),
		"instanceMethodFor",		METHOD(c_instanceMethodFor),
		"classMessages",			METHOD(c_classMessages),
		"instanceMessages",			METHOD(c_instanceMessages),
		"classProperties",			METHOD(c_classProperties),
		"instanceProperties",		METHOD(c_instanceProperties),
		"addClassMessage",			METHOD(c_addClassMessage),
		"removeClassMessage",		METHOD(c_removeClassMessage),
		"addInstanceMessage",		METHOD(c_addInstanceMessage),
		"removeInstanceMessage",	METHOD(c_removeInstanceMessage),
		"addClassProperty",			METHOD(c_addClassProperty),
		"removeClassProperty",		METHOD(c_removeClassProperty),
		"addInstanceProperty",		METHOD(c_addInstanceProperty),
		"removeInstanceProperty",	METHOD(c_removeInstanceProperty),
		0, 0
	};

	selector _i_messages[] = {
		"get",						METHOD(i_get),
		"set",						METHOD(i_set),
		"init",						METHOD(i_init),
		"free",						METHOD(i_free),
		"class",					METHOD(i_class),
		"superclass",				METHOD(i_superclass),
		"toString",					METHOD(i_toString),
		"isKindOf",					METHOD(i_isKindOf),
		"respondsTo",				METHOD(i_respondsTo),
		"methodFor",				METHOD(i_methodFor),
		"delegate",					METHOD(i_delegate),
		"setDelegate",				METHOD(i_setDelegate),
		"getDelegate",				METHOD(i_getDelegate),
		"copy",						METHOD(i_copy),
		"assume",					METHOD(i_assume),
		"print",					METHOD(i_print),
		"error",					METHOD(i_error),
		"doesNotContain",			METHOD(i_doesNotContain),
		"doesNotRecognize",			METHOD(i_doesNotRecognize),
		"notImplemented",			METHOD(i_notImplemented),
		"subclassResponsibility",	METHOD(i_subclassResponsibility),
		"subclassResponsibility",	METHOD(i_subclassResponsibility),
		"invalidArgument",			METHOD(i_invalidArgument),
		"messages",					METHOD(i_messages),
		"properties",				METHOD(i_properties),
		0, 0
	};

	Object = class_new("Object", 0, 1, 0, 0, _c_messages, _i_messages);
}
