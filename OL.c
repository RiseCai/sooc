/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#include "OL.h"

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <regex.h>

#include <stdarg.h>

#define CNAMEMAXLEN 64
#define CNAMEREGEXP "^[:alpha:]?[[:alnum:]_-]{0,62}[:alnum:]?$"

class class_alloc( void ) {
	return (class)calloc( 1, sizeof ( struct _class ));
}

class class_init( class self, const char *cname, class superclass, unsigned int rev, property *c_props, property *i_props, selector *c_msgs, selector *i_msgs ) {
	static regex_t regexp;

	regcomp(&regexp, CNAMEREGEXP, REG_EXTENDED | REG_NOSUB);

	if ( regexec(&regexp, cname, 0, 0, 0) != 0 )
		return 0;

	extern class Object;

	self->type = CLASS;
	self->name = cname;
	self->revision = rev;

	self->superclass = strcmp("Object", cname) != 0 ? (superclass ? superclass : Object) : 0;

	if ( c_msgs )
		class_set_class_messages(self, c_msgs);

	if ( i_msgs )
		class_set_instance_messages(self, i_msgs);

	if ( c_props )
		class_set_class_properties(self, c_props);

	if ( i_props )
		class_set_instance_properties(self, i_props);

	return self;
}

class class_new( const char *name, class superclass, unsigned revision, property *class_properties, property *instance_properties, selector *class_messages, selector *instance_messages ) {
	return class_init( class_alloc(), name, superclass, revision, class_properties, instance_properties, class_messages, instance_messages );
}

void class_free( class self ) {
	if (self->c_properties)
		list_free( self->c_properties );
	if (self->i_properties)
		list_free( self->i_properties );
	if (self->c_messages)
		alist_free( self->c_messages );
	if (self->i_messages)
		alist_free( self->i_messages );
	if (self->attributes)
		alist_free( self->attributes );
	free( self );
}

char *class_tostring( class self ) {
	static char s[CNAMEMAXLEN+7+1];

	sprintf(s, "class(%s)", self->name);

	return s;
}

const char *class_name(class self) {
	return self->name;
}

unsigned int class_revision(class self) {
	return self->revision;
}

class class_superclass(class self) {
	return self->superclass;
}

list class_class_properties(class self) {
	return self->c_properties;
}

list class_instance_properties(class self) {
	return self->i_properties;
}

alist class_class_messages(class self) {
	return self->c_messages;
}

alist class_instance_messages(class self) {
	return self->i_messages;
}

class class_set_class_messages(class self, selector *msgs) {
	if ( self->c_messages )
		alist_free(self->c_messages);

	alist l = alist_new();
	selector *p;

	for ( p = msgs; p->msg; p++ )
		alist_put(l, p->msg, p->func);

	self->c_messages = l;

	return self;
}

class class_set_instance_messages(class self, selector *msgs) {
	if ( self->i_messages )
		alist_free(self->i_messages);

	alist l = alist_new();
	selector *p;

	for ( p = msgs; p->msg; p++ )
		alist_put(l, p->msg, p->func);

	self->i_messages = l;

	return self;
}

class class_set_class_properties(class self, property *props) {
	if ( self->c_properties )
		list_free(self->c_properties);

	list l = list_new();
	property *p;

	for ( p = props; *p; p++ )
		list_put(l, -1, *p);

	self->c_properties = l;

	return self;
}

class class_set_instance_properties(class self, property *props) {
	if ( self->i_properties )
		list_free(self->i_properties);

	list l = list_new();
	property *p;

	for ( p = props; *p; p++ )
		list_put(l, -1, *p);

	self->i_properties = l;

	return self;
}

class class_add_class_message(class self, message msg, method func) {
	alist msglist = class_class_messages(self);

	if (!msglist)
		self->c_messages = msglist = alist_new();

	alist_put(msglist, msg, func);

	return self;
}

class class_remove_class_message(class self, message msg) {
	alist msglist = class_class_messages(self);

	if (msglist)
		alist_delete(msglist, msg);

	return self;
}

class class_add_instance_message(class self, message msg, method func) {
	alist msglist = class_instance_messages(self);

	if (!msglist)
		self->i_messages = msglist = alist_new();

	alist_put(msglist, msg, func);

	return self;
}

class class_remove_instance_message(class self, message msg) {
	alist msglist = class_instance_messages(self);

	if (msglist)
		alist_delete(msglist, msg);

	return self;
}

class class_add_class_property(class self, property prop) {
	list proplist = class_class_properties(self);

	if (!proplist) {
		self->c_properties = proplist = list_new();
		list_put(proplist, -1, prop);
	}
	else if (list_find(proplist, prop, (int (*)(void *, void *))strcmp) == -1) {
		list_put(proplist, -1, prop);
	}

	return self;
}

class class_remove_class_property(class self, property prop) {
	list proplist = class_class_properties(self);

	if (proplist) {
		int i = list_find(proplist, prop, (int (*)(void *, void *))strcmp);

		if (i != -1)
			list_delete(proplist, i);
	}

	return self;
}

class class_add_instance_property(class self, property prop) {
	list proplist = class_instance_properties(self);

	if (!proplist) {
		self->i_properties = proplist = list_new();
		list_put(proplist, -1, prop);
	}
	else if (list_find(proplist, prop, (int (*)(void *, void *))strcmp) == -1) {
		list_put(proplist, -1, prop);
	}

	return self;
}

class class_remove_instance_property(class self, property prop) {
	list proplist = class_instance_properties(self);

	if (proplist) {
		int i = list_find(proplist, prop, (int (*)(void *, void *))strcmp);

		if (i != -1)
			list_delete(proplist, i);
	}

	return self;
}

alist class_attributes(class self) {
	return self->attributes;
}

class class_set_attributes(class self, alist data) {
	if (self->attributes)
		alist_free(self->attributes);

	self->attributes = data;

	return self;
}

int class_is_kind_of(class self, class c) {
	if (self == c)
		return 1;

	class superclass = class_superclass(self);

	return superclass ? class_is_kind_of(superclass, c) : 0;
}

value class_get(class self, property prop) {
	if (!class_find_class_property(self, prop))
		return class_send_message(self, "doesNotContain", prop);

	return (value)(self->attributes ? alist_get(self->attributes, prop) : 0);
}

class class_set(class self, property prop, value val) {
	if (!class_find_class_property(self, prop)) {
		class_send_message(self, "doesNotContain", prop);
	}
	else {
		if (!self->attributes)
			self->attributes = alist_new();

		alist_put(self->attributes, prop, val.p);
	}

	return self;
}

instance class_make(class self) {
	return object_new( self );
}

property class_find_class_property(class self, property prop) {
	list properties = class_class_properties(self);

	if (properties && list_find(properties, prop, (int (*)(void *, void *))strcmp) != -1)
		return prop;

	class superclass = class_superclass(self);

	return superclass ? class_find_class_property(superclass, prop) : 0;
}

property class_find_instance_property(class self, property prop) {
	list properties = class_instance_properties(self);

	if (properties && list_find(properties, prop, (int (*)(void *, void *))strcmp) != -1)
		return prop;

	class superclass = class_superclass(self);

	return superclass ? class_find_instance_property(superclass, prop) : 0;
}

method class_find_class_method(class self, message msg) {
	alist msglist = class_class_messages(self);
	method f = msglist ? alist_get(msglist, msg) : 0;

	if (f) {
		return f;
	}

	class superclass=class_superclass(self);

	return superclass ? class_find_class_method(superclass, msg) : 0;
}

method class_find_instance_method(class self, message msg) {
	alist msglist = class_instance_messages(self);
	method f = msglist ? alist_get(msglist, msg) : 0;

	if (f) {
		return f;
	}

	class superclass = class_superclass(self);

	return superclass ? class_find_instance_method(superclass, msg) : 0;
}

value class_send_message(class self, message msg, ...) {
	va_list va;

	va_start(va, msg);

	value ret = class_send_message_va(self, msg, va);

	va_end(va);

    return ret;
}

value class_send_message_va(class self, message msg, va_list va) {
	method f = class_find_class_method(self, msg);

	if (f) {
	    return f(self, va);
	}

	va_list vac;

	va_copy(vac, va);

	value ret = class_send_message(self, "doesNotRecognize", msg, vac);

	va_end(vac);

	return ret;
}

value class_super_send_message(class fromclass, class self, message msg, ...) {
	va_list va;

	va_start(va, msg);

	value ret = class_super_send_message_va(fromclass, self, msg, va);

	va_end(va);

    return ret;
}

value class_super_send_message_va(class fromclass, class self, message msg, va_list va) {
	class superclass = class_superclass(fromclass);

	method f = class_find_class_method(superclass, msg);

	if (f) {
	    return f(self, va);
	}

	va_list vac;

	va_copy(vac, va);

	value ret = class_super_send_message(fromclass, self, "doesNotRecognize", msg, vac);

	va_end(vac);

	return ret;
}

instance object_alloc( void ) {
	return (instance)calloc( 1, sizeof ( struct _instance ));
}

instance object_init( instance self, class c ) {
	self->type = INSTANCE;
	self->class = c;

	return self;
}

instance object_new( class c ) {
	return object_init(object_alloc(), c);
}

void object_free( instance self ) {
	if (self->attributes)
		alist_free( self->attributes );
	free( self );
}

char *object_tostring( instance self ) {
	static char s[CNAMEMAXLEN+8+1];

	sprintf(s, "object(%s)", self->class->name);

	return s;
}

class object_class(instance self) {
	return self->class;
}

class object_superclass(instance self) {
	return class_superclass(object_class(self));
}

instance object_assume(instance self, class c) {
	self->class=c;

	return self;
}

alist object_attributes(instance self) {
	return self->attributes;
}

instance object_set_attributes(instance self, alist data) {
	if (self->attributes)
		alist_free(self->attributes);

	self->attributes = data;

	return self;
}

value object_get(instance self, property prop) {
	if (!class_find_instance_property(object_class(self), prop))
		return object_send_message(self, "doesNotContain", prop);

	return (value)(self->attributes ? alist_get(self->attributes, prop) : 0);
}

instance object_set(instance self, property prop, value val) {
	if (!class_find_instance_property(object_class(self), prop)) {
		object_send_message(self, "doesNotContain", prop);
	}
	else {
		if (!self->attributes)
			self->attributes = alist_new();

		alist_put(self->attributes, prop, val.p);
	}

	return self;
}

instance object_copy(instance self) {
	instance newself = object_new(object_class(self));

	if (self->attributes)
		newself->attributes = alist_copy(self->attributes);

	return newself;
}

value object_send_message(instance self, message msg, ...) {
	va_list va;
	va_start(va, msg);

	value ret = object_send_message_va(self, msg, va);

	va_end(va);

    return ret;
}

value object_send_message_va(instance self, message msg, va_list va) {
	method f = class_find_instance_method(object_class(self), msg);

	if (f) {
	    return f(self, va);
	}

	va_list vac;

	va_copy(vac, va);

	value ret = object_send_message(self, "doesNotRecognize", msg, va);

	va_end(vac);

	return ret;
}

value object_super_send_message(class fromclass, instance self, message msg, ...) {
	va_list va;

	va_start(va, msg);

	value ret = object_super_send_message_va(fromclass, self, msg, va);

	va_end(va);

    return ret;
}

value object_super_send_message_va(class fromclass, instance self, message msg, va_list va) {
	class superclass = class_superclass(fromclass);

	method f = class_find_instance_method(superclass, msg);

	if (f) {
	    return f(self, va);
	}

	va_list vac;

	va_copy(vac, va);

	value ret = object_super_send_message(fromclass, self, "doesNotRecognize", msg, vac);

	va_end(vac);

	return ret;
}
