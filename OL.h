/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#ifndef _OL_H
#define _OL_H

#include <stdarg.h>

#include "list.h"
#include "alist.h"

typedef char *property;
typedef char *message;

typedef union _value {
	int i;
	long l;
	float f;
	double d;
	void *p;
} value;

typedef value (*method)();

#define METHOD(f) (value (*)())f

typedef struct _selector {
	message msg;
	method func;
} selector;

typedef struct _attribute {
	property name;
	value value;
} attribute;

#define CLASS		'C'
#define INSTANCE	'I'

typedef struct _class {
	char type;
	const char *name;
	unsigned int revision;
	struct _class *superclass;
	list c_properties;
	list i_properties;
	alist c_messages;
	alist i_messages;
	alist attributes;
} *class;

typedef struct _instance {
	char type;
	class class;
	alist attributes;
} *instance;

#ifdef __STDC__
extern class class_alloc( void );
extern class class_init( class self, const char *cname, class superclass, unsigned int rev, property *c_props, property *i_props, selector *c_msgs, selector *i_msgs );
extern class class_new( const char *name, class superclass, unsigned revision, property *class_properties, property *instance_properties, selector *class_messages, selector *instance_messages );
extern void class_free( class self );
extern char *class_tostring( class self );
extern const char *class_name(class self);
extern unsigned int class_revision(class self);
extern class class_superclass(class self);
extern list class_class_properties(class self);
extern list class_instance_properties(class self);
extern alist class_class_messages(class self);
extern alist class_instance_messages(class self);
extern class class_set_class_messages(class self, selector *msgs);
extern class class_set_instance_messages(class self, selector *msgs);
extern class class_set_class_properties(class self, property *props);
extern class class_set_instance_properties(class self, property *props);
extern class class_add_class_message(class self, message msg, method func);
extern class class_remove_class_message(class self, message msg);
extern class class_add_instance_message(class self, message msg, method func);
extern class class_remove_instance_message(class self, message msg);
extern class class_add_class_property(class self, property prop);
extern class class_remove_class_property(class self, property prop);
extern class class_add_instance_property(class self, property prop);
extern class class_remove_instance_property(class self, property prop);
extern alist class_attributes(class self);
extern class class_set_attributes(class self, alist data);
extern int class_is_kind_of(class self, class c);
extern value class_get(class self, property prop);
extern class class_set(class self, property prop, value val);
extern instance class_make(class self);
extern property class_find_class_property(class self, property prop);
extern property class_find_instance_property(class self, property prop);
extern method class_find_class_method(class self, message msg);
extern method class_find_instance_method(class self, message msg);
extern value class_send_message(class self, message msg, ...);
extern value class_send_message_va(class self, message msg, va_list va);
extern value class_super_send_message(class fromclass, class self, message msg, ...);
extern value class_super_send_message_va(class fromclass, class self, message msg, va_list va);
extern instance object_alloc( void );
extern instance object_init( instance self, class c );
extern instance object_new( class c );
extern void object_free( instance self );
extern char *object_tostring( instance self );
extern class object_class(instance self);
extern class object_superclass(instance self);
extern instance object_assume(instance self, class c);
extern alist object_attributes(instance self);
extern instance object_set_attributes(instance self, alist data);
extern value object_get(instance self, property prop);
extern instance object_set(instance self, property prop, value val);
extern instance object_copy(instance self);
extern value object_send_message(instance self, message msg, ...);
extern value object_send_message_va(instance self, message msg, va_list va);
extern value object_super_send_message(class fromclass, instance self, message msg, ...);
extern value object_super_send_message_va(class fromclass, instance self, message msg, va_list va);
#else
extern class class_alloc();
extern class class_init();
extern class class_new();
extern void class_free();
extern char *class_tostring();
extern const char *class_name();
extern unsigned int class_revision();
extern class class_superclass();
extern list class_class_properties();
extern list class_instance_properties();
extern alist class_class_messages();
extern alist class_instance_messages();
extern class class_set_class_messages();
extern class class_set_instance_messages();
extern class class_set_class_properties();
extern class class_set_instance_properties();
extern class class_add_class_message();
extern class class_remove_class_message();
extern class class_add_instance_message();
extern class class_remove_instance_message();
extern class class_add_class_property();
extern class class_remove_class_property();
extern class class_add_instance_property();
extern class class_remove_instance_property();
extern alist class_attributes();
extern class class_set_attributes();
extern int class_is_kind_of();
extern value class_get();
extern class class_set();
extern instance class_make();
extern property class_find_class_property();
extern property class_find_instance_property();
extern method class_find_class_method();
extern method class_find_instance_method();
extern value class_send_message();
extern value class_send_message_va();
extern value class_super_send_message();
extern value class_super_send_message_va();
extern instance object_alloc();
extern instance object_init();
extern instance object_new();
extern void object_free();
extern char *object_tostring();
extern class object_class();
extern class object_superclass();
extern instance object_assume();
extern alist object_attributes();
extern instance object_set_attributes();
extern value object_get();
extern instance object_set();
extern instance object_copy();
extern value object_send_message();
extern value object_send_message_va();
extern value object_super_send_message();
extern value object_super_send_message_va();
#endif
#endif
