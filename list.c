#include "list.h"

#include <stdlib.h>
#include <time.h>

list list_alloc( void ) {
	return (list)calloc( 1, sizeof ( struct _list ));
}

list list_init( list this ) {
	return this;
}

list list_new( void ) {
	return list_init( list_alloc() );
}

void list_free( list this ) {
	if ( this->v )
		free( this->v );
	free( this );
}

int list_length( list this ) {
	return this->l;
}

void *list_get( list this, int key ) {
	if ( key < 0 )
		return this->l ? this->v[ this->l - 1 ] : 0;

	if ( key < this->l )
		return this->v[ key ];

	return 0;
}

void *list_put( list this, int key, void *val ) {
	if ( key < 0 )
		key = this->l;

	if ( key < this->l ) {
		this->v[ key ] = val;
	}
	else {
		this->l = key + 1;
		this->v = (void **)realloc( this->v, (key + 1) * sizeof ( void * ));
		this->v[ key ] = val;
	}

	return val;
}

void *list_delete( list this, int key ) {
	void **p, *val;

	if ( key < 0 )
		key = this->l ? this->l - 1 : 0;

	if ( key < this->l ) {
		val = this->v[ key ];
		this->l--;

		for ( p = this->v + key; p < this->v + this->l; p++ ) {
			*p = *(p+1);
		}

		return val;
	}

	return 0;
}

void *list_insert( list this, int key, void *val ) {
	void **p;

	if ( key < 0 || key >= this->l )
		return list_put( this, key, val );

	this->v = (void **)realloc( this->v, ++this->l * sizeof ( void * ));

	for ( p = this->v + (this->l - 1); p > this->v + key; p-- ) {
		*p = *(p-1);
	}

	this->v[ key ] = val;

	return val;
}

int list_find( list this, void *val, int (*fcmp)(void *, void *) ) {
	void **p;

	for ( p = this->v; p < this->v + this->l; p++ ) {
		if ( (fcmp && fcmp(*p, val) == 0) || (!fcmp && *p == val ) )
			return p - this->v;
	}

	return -1;
}

list list_copy( list this ) {
	list nl = list_new();

	nl->v = (void **)realloc( nl->v, this->l * sizeof ( void * ) );
	nl->l = this->l;

	void **p, **np;

	for ( p = this->v, np = nl->v; p < this->v + this->l; p++, np++ ) {
		*np = *p;
	}

	return nl;
}

list list_merge( list this, list m ) {
	this->v = (void **)realloc( this->v, (this->l + m->l) * sizeof ( void * ) );

	void **p, **mp;

	for ( mp = m->v, p = this->v+this->l; mp < m->v + m->l; mp++, p++ ) {
		*p = *mp;
	}

	this->l = this->l + m->l;

	return this;
}

void **list_values( list this ) {
	void *values = malloc((this->l + 1) * sizeof (void *));

	void **p, **vp;

	for ( p = this->v, vp = values; p < this->v + this->l; vp++, p++ ) {
		*vp = *p;
	}
	*vp = 0;	/* convenient */

	return values;
}

list list_fill( list this, void *val ) {
	void **p;

	for ( p = this->v; p < this->v + this->l; p++ )
		*p = val;

	return this;
}

list list_sort( list this, int (*fcmp)(void *, void *), int dir ) {
	/* bubble sort */
	for ( int i = list_length(this)-1; i > 0; i-- ) {
		for ( int j = 0; j < i; j++ ) {
			void *v1 = list_get(this, j);
			void *v2 = list_get(this, j+1);

			if ((dir > 0 && ((fcmp && fcmp(v1, v2) > 0) || (!fcmp && v1 > v2))) ||
				(dir < 0 && ((fcmp && fcmp(v1, v2) < 0) || (!fcmp && v1 < v2)))) {
				list_put(this, j, v2);
				list_put(this, j+1, v1);
			}
		}
	}

	return this;
}

list list_shuffle( list this ) {
	int len = list_length(this);

	if (len > 1) {
		srand(time(0));

		for ( int i = 0; i < len-1; i++ ) {
			int j = i + rand() / (RAND_MAX / (len - i) + 1);
			void *v = list_get(this, j);

	        list_put(this, j, list_get(this, i));
	        list_put(this, i, v);
		}
	}

	return this;
}

#if	defined( TEST )

#include <stdio.h>
#include <string.h>

void print_list(list l) {
	int len = list_length(l);

	printf("l=%d\n", len);

	for (int i = 0; i < len; i++)
		printf("%s\n", (char *)list_get(l, i));
}

int main( int argc, char *argv[] ) {
	char *s;
	int i;
	void **p;

	list numbers = list_new();

	printf("l=%d\n", list_length(numbers));

	list_put(numbers, 0, "zero");
	list_put(numbers, 1, "one");
	list_put(numbers, 2, "two");

	printf("l=%d\n", list_length(numbers));

	s=(char *)list_get(numbers, 0);
	printf("%s\n", s ? s : "0?");
	s=(char *)list_get(numbers, 1);
	printf("%s\n", s ? s : "1?");
	s=(char *)list_get(numbers, 2);
	printf("%s\n", s ? s : "2?");

	list_put(numbers, -1, "three");

	s=(char *)list_get(numbers, 3);
	printf("%s\n", s ? s : "3?");

	list_put(numbers, 3, "four");
	list_insert(numbers, 3, "three");

	printf("l=%d\n", list_length(numbers));

	s=(char *)list_get(numbers, 3);
	printf("%s\n", s ? s : "3?");
	s=(char *)list_get(numbers, 4);
	printf("%s\n", s ? s : "4?");

	list_delete(numbers, 0);

	printf("l=%d\n", list_length(numbers));

	s=(char *)list_get(numbers, 0);
	printf("%s\n", s ? s : "0?");

	i=list_find(numbers, "four", (int (*)(void *, void *))strcmp);
	printf("%d\n", i);
	if ( i >= 0 )
		printf("%s\n", (char *)list_get(numbers, i));

	list newnumbers = list_copy(numbers);
	print_list(newnumbers);

	list_put(numbers, 0, "five");
	list_put(numbers, 1, "six");
	list_put(numbers, 2, "seven");
	list_put(numbers, 3, "eight");
	list_put(numbers, 4, "nine");

	printf("l=%d\n", list_length(numbers));

	p = list_values(numbers);
	for (i = 0; i < list_length(numbers); i++)
		printf("%s\n", (char *)p[i]);
	free(p);

	list_merge(newnumbers, numbers);
	print_list(newnumbers);

	list_shuffle(newnumbers);
	print_list(newnumbers);

	list_sort(newnumbers, (int (*)(void *, void *))strcmp, 1);
	print_list(newnumbers);

	list_free(newnumbers);
	list_free(numbers);

 	exit( 0 );
}

#endif
