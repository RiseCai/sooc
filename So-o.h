/**
 *
 * @copyright  2018 so-o.org
 * @version    1
 * @link       http://www.so-o.org
 */

#ifndef _SOO_H
#define _SOO_H

#include "Object.h"

#ifdef __STDC__
extern class defclass( const char *name, class superclass, unsigned revision, property *class_properties, property *instance_properties, selector *class_messages, selector *instance_messages );
extern value sendmsg( void *rcv, message msg, ... );
extern value applymsg( void *rcv, message msg, va_list va);
extern value supersend( class c, void *rcv, message msg, ... );
extern value superapply( class c, void *rcv, message msg, va_list va );
#else
extern class defclass();
extern value sendmsg();
extern value applymsg();
extern value supersend();
extern value superapply();
#endif
#endif
