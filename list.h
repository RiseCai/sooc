#ifndef _LIST_H
#define _LIST_H

#include <stdio.h>

typedef struct _list {
	int l;
	void **v;
} *list;

#ifdef __STDC__
extern list list_alloc( void );
extern list list_init( list this );
extern list list_new( void );
extern void list_free( list this );
extern int list_length( list this );
extern void *list_get( list this, int key );
extern void *list_put( list this, int key, void *val );
extern void *list_delete( list this, int key );
extern void *list_insert( list this, int key, void *val );
extern int list_find( list this, void *val, int (*fcmp)(void *, void *) );
extern list list_copy( list this );
extern list list_merge( list this, list m );
extern void **list_values( list this );
extern list list_fill( list this, void *val );
extern list list_sort( list this, int (*fcmp)(void *, void *), int dir );
extern list list_shuffle( list this );
#else
extern list list_alloc();
extern list list_init();
extern list list_new();
extern void list_free();
extern int list_length();
extern void *list_get();
extern void *list_put();
extern void *list_delete();
extern void *list_insert();
extern int list_find();
extern list list_copy();
extern list list_merge();
extern void **list_values();
extern list list_fill();
extern list list_sort();
extern list list_shuffle();
#endif
#endif
