#ifndef _ALIST_H
#define _ALIST_H

typedef struct _alist {
	int l;
	void **v;
} *alist;

#ifdef __STDC__
extern alist alist_alloc( void );
extern alist alist_init( alist this );
extern alist alist_new( void );
extern void alist_free( alist this );
extern int alist_length( alist this );
extern void *alist_get( alist this, const char *key );
extern void *alist_put( alist this, const char *key, void *val );
extern void *alist_delete( alist this, const char *key );
extern alist alist_copy( alist this );
extern alist alist_merge( alist this, alist m );
extern void **alist_keys( alist this );
extern void **alist_values( alist this );
#else
extern alist alist_alloc();
extern alist alist_init();
extern alist alist_new();
extern void alist_free();
extern int alist_length();
extern void *alist_get();
extern void *alist_put();
extern void *alist_delete();
extern alist alist_copy();
extern alist alist_merge();
extern void **alist_keys();
extern void **alist_values();
#endif
#endif
