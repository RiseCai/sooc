#include "alist.h"

#include <stdlib.h>
#include <string.h>

alist alist_alloc( void ) {
	return (alist)calloc( 1, sizeof ( struct _alist ));
}

alist alist_init( alist this ) {
	return this;
}

alist alist_new( void ) {
	return alist_init( alist_alloc() );
}

void alist_free( alist this ) {
	if ( this->v )
		free( this->v );
	free( this );
}

int alist_length( alist this ) {
	return this->l;
}

void *alist_get( alist this, const char *key ) {
	void **p;

	for ( p = this->v; p < this->v + this->l * 2; p += 2 ) {
		if ( strcmp(*(char **)p, key) == 0 ) {
			return *(p+1);
		}
	}

	return 0;
}

void *alist_put( alist this, const char *key, void *val ) {
	void **p;

	for ( p = this->v; p < this->v + this->l * 2; p += 2 ) {
		if ( strcmp(*(char **)p, key) == 0 ) {
			return *(p+1) = val;
		}
	}

	this->v = (void **)realloc( this->v, (this->l * 2 + 2) * sizeof ( void * ));

	p = this->v + this->l * 2;

	*p = (void *)key;
	*(p+1) = val;

	++this->l;

	return val;
}

void *alist_delete( alist this, const char *key ) {
	void **p, *val;

	for ( p = this->v; p < this->v + this->l * 2; p += 2 ) {
		if ( strcmp(*(char **)p, key) == 0 ) {
			val = *(p+1);

			do {
				*p = *(p+2); p++;
				*p = *(p+2); p++;
			}
			while ( p < this->v + this->l * 2 );

			--this->l;

			return val;
		}
	}

	return 0;
}

alist alist_copy( alist this ) {
	alist nl = alist_new();

	nl->v = (void **)realloc( nl->v, (this->l * 2) * sizeof ( void * ) );
	nl->l = this->l;

	void **p, **np;

	for ( p = this->v, np = nl->v; p < this->v + this->l * 2; p++, np++ ) {
		*np = *p;
	}

	return nl;
}

alist alist_merge( alist this, alist m ) {
	void **mp;

	for ( mp = m->v; mp < m->v + m->l * 2; mp += 2 ) {
		alist_put(this, *mp, *(mp+1));
	}

	return this;
}

void **alist_keys( alist this ) {
	void *keys = malloc((this->l + 1) * sizeof (void *));

	void **p, **kp;

	for ( p = this->v, kp = keys; p < this->v + this->l * 2; kp++, p += 2 ) {
		*kp = *p;
	}
	*kp = 0;	/* convenient */

	return keys;
}

void **alist_values( alist this ) {
	void *values = malloc((this->l + 1) * sizeof (void *));

	void **p, **vp;

	for ( p = this->v, vp = values; p < this->v + this->l * 2; vp++, p += 2 ) {
		*vp = *(p+1);
	}
	*vp = 0;	/* convenient */

	return values;
}

#if	defined( TEST )

#include <stdio.h>

int main( int argc, char *argv[] ) {
	char *s;

	alist numbers = alist_new();

	printf("l=%d\n", alist_length(numbers));

	alist_put(numbers, "one", "un");
	alist_put(numbers, "two", "deux");
	alist_put(numbers, "three", "trois");

	printf("l=%d\n", alist_length(numbers));

	s=(char *)alist_get(numbers, "one");
	printf("%s\n", s ? s : "one?");
	s=(char *)alist_get(numbers, "two");
	printf("%s\n", s ? s : "two?");
	s=(char *)alist_get(numbers, "three");
	printf("%s\n", s ? s : "three?");

	alist_put(numbers, "two", "zwei");

	s=(char *)alist_get(numbers, "two");
	printf("%s\n", s ? s : "two?");

	alist_delete(numbers, "two");

	printf("l=%d\n", alist_length(numbers));

	s=(char *)alist_get(numbers, "one");
	printf("%s\n", s ? s : "one?");
	s=(char *)alist_get(numbers, "two");
	printf("%s\n", s ? s : "two?");
	s=(char *)alist_get(numbers, "three");
	printf("%s\n", s ? s : "three?");

	alist_put(numbers, "four", "quatre");
	alist_put(numbers, "five", "cinq");

	printf("l=%d\n", alist_length(numbers));

	s=(char *)alist_get(numbers, "four");
	printf("%s\n", s ? s : "four?");
	s=(char *)alist_get(numbers, "five");
	printf("%s\n", s ? s : "five?");

	alist newnumbers = alist_copy(numbers);

	printf("l=%d\n", alist_length(newnumbers));
	s=(char *)alist_get(newnumbers, "four");
	printf("%s\n", s ? s : "four?");
	s=(char *)alist_get(newnumbers, "five");
	printf("%s\n", s ? s : "five?");

	alist_put(numbers, "six", "six");
	printf("l=%d\n", alist_length(numbers));

	alist_merge(newnumbers, numbers);
	printf("l=%d\n", alist_length(newnumbers));

	s=(char *)alist_get(newnumbers, "one");
	printf("%s\n", s ? s : "one?");
	s=(char *)alist_get(newnumbers, "six");
	printf("%s\n", s ? s : "six?");

	alist_free(newnumbers);
	alist_free(numbers);

 	exit( 0 );
}

#endif

