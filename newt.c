#include "single.h"
#include "simpletables.h"

#define MARKDUMPS

#define SIG(term) \
	int term( uint8_t *b, int *a, int *z, Marker **p, Table *t, Marker **my )

#define COPYHASH(src, ssize, p, psize, v, vsize, cnt) \
	memcpy( &src[ ssize ] , p, psize );  \
	ssize += psize; \
	if (cnt > -1) { \
		char misc[10] = {0}; \
		snprintf( misc, 9, ".%d", cnt ); \
		memcpy( &src[ ssize ], misc, strlen(misc) ); ssize+=strlen(misc); \
	} \
	memcpy( &src[ ss ] , v, vsize ); \
	ssize += vsize;
	

#if 1
 #define SIGGO()
#else
 #define SIGGO() \
	proc_block( b, a, z, t, my )
#endif

#if 0
 #define SHOW_MARKER(m) \
	fprintf( stderr, "{ " ); \
	fprintf( stderr, "action = %c,", m->action ); \
	fprintf( stderr, "parent = " ); \
	( !m->parent ) ? write(2, "NULL", 4) : write(2, m->parent, m->psize); \
	fprintf( stderr, "blob   = " ); \
	( !m->blob ) ? write( 2, "NULL", 4) : write(2, m->blob, m->bsize); \
	fprintf( stderr, "sentinel = %d,", m->sentinel ); \
	fprintf( stderr, "}\n" )
#else
 #define SHOW_MARKER(m) \
	fprintf( stderr, "{ " ); \
	fprintf( stderr, "action = %c, ", m->action ); \
	fprintf( stderr,  "index = %3d, ", m->index ); \
	fprintf( stderr, "parent = " ); \
	( !m->parent ) ? write(2, "NULL", 4) : write(2, m->parent, m->psize); \
	fprintf( stderr, ", blob = " ); \
	( !m->blob ) ? write( 2, "NULL", 4) : write(2, "FILL", 4); \
	fprintf( stderr, ", sentinel = %d,", m->sentinel ); \
	fprintf( stderr, "}\n" )
#endif

#ifndef SIGDYN
 #define REALLOC_M(src) \
	(*src)++; \
	(*src)->sentinel = 1

 #define SETBLOBMOVE(b, sz) \
	(*(*my)->bsb)->blob = b; \
	(*(*my)->bsb)->size = sz; \
	(*(*my)->bsb)->sentinel = 0; \
	(*(*my)->bsb)++; \
	(*(*my)->bsb)->sentinel = 1

#else
 #define REALLOC_M(src) \
	Marker *dest = realloc( src, ++( *z ) * sizeof(Marker)); \
	if ( !dest ) \
		return 0; \
	else  \
	{ \
		src = dest; \
		memset( &src[ (*z - 1)], 0, sizeof(Marker) ); \
		src = &src[ (*z - 1) ]; \
		src->sentinel = 1; \
	}

 #define SETBLOBMOVE( blob, size ) \
	(*(*my)->bsb)->blob = 0; \
	(*(*my)->bsb)->size = 0; \
	(*(*my)->bsb)->sentinel = 0; \
	(*(*my)->bsb)++; \
	(*(*my)->bsb)->sentinel = 1;
#endif


const LiteKv MT[] = {
	{ INT_KEY( 1 )    , TEXT_VALUE( "Wash the dog." )  },
	{ TEXT_KEY( "ashor" )  , TABLE_VALUE( )         },
		{ INT_KEY( 7043 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog." )  },
		{ INT_KEY( 7002 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog another time." )  },
		{ TRM() },
	{ TEXT_KEY( "head" )       , TABLE_VALUE( )         },
		/*Database records look a lot like this*/
		{ INT_KEY( 0 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "number" ), INT_VALUE ( 1 ) },
			{ TRM() },

		{ INT_KEY( 1 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "body" ), TEXT_VALUE( "Choo choo cachoo." ) },
			{ TEXT_KEY( "number" ), INT_VALUE ( 2 ) },
			{ TRM() },

		{ INT_KEY( 2 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "body" ), TEXT_VALUE( "Choo choo cachoo." ) },
			{ TEXT_KEY( "number" ), INT_VALUE ( 3 ) },
			{ TRM() },

		{ INT_KEY( 3 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "body" ), TEXT_VALUE( "Choo choo cachoo." ) },
			{ TEXT_KEY( "number" ), INT_VALUE ( 4 ) },
			{ TRM() },
		{ TRM() },
	{ TEXT_KEY( "michael" )       , TABLE_VALUE( )         },
		{ TEXT_KEY( "val" ), BLOB_VALUE( "MySQL makes me giddy." ) },
		{ TEXT_KEY( "rec" ), TEXT_VALUE( "Choo choo cachoo." ) },
		{ TRM() },
	{ TEXT_KEY( "jackson" )       , TABLE_VALUE( )         },
		{ TEXT_KEY( "val" ), BLOB_VALUE( "MySQL makes me ecstatic." ) },
		{ TEXT_KEY( "rec" ), TEXT_VALUE( "Choo choo cachoo." ) },
		{ TRM() },
	{ LKV_LAST } 
};


typedef struct Shim
{
	uint8_t *blob;
	int      size,
					 sentinel;
} Shim;


typedef struct Marker 
{
	uint8_t *blob,    // Block between active terms.
					*parent;  // Parent block of text, if I'm within some kind of table
	struct   Marker
					*pmarker; // An easier way to do the non-recursive parent thing...
	int      bsize,   // Size of blob / block
					 psize,   // Size of parent block
					 action,  // Actions are no longer predefined, but they go here
					 type,    // ?
					 index,   // Where in the table is the value found
					 sentinel;// The last item in the set
	Shim    *bsb[100];// Blobs and sizes, less space...
} Marker;


//This is the signature for text processing blocks.
SIG( proc_block )
{
	fprintf( stderr, "Got a " );
	write( 2, b, 1 );
	fprintf( stderr, "\n" );
	return 0;
	//Should return false, true, and continue?
}


SIG( left_curly_brace_block )
{
	SIGGO();
	if ( !*a ) {
		*a = 1;
		return 0;
	}
	else {
		*a = 2;
		(*my)->action = *b;
		(*my)->blob = NULL;
		(*my)->parent = NULL;
		(*my)->sentinel = 0;
		REALLOC_M( my );
		return 1;
	}
	return 0;
}


SIG( right_curly_brace_block )
{
	SIGGO();
	if ( *a == 2 ) {
		*a = 1;
		return 0;
	}
	else if ( *a == 1 ) {
		*a = 0;
		(*my)->action = *b;
		(*my)->blob = b + 1;
		(*my)->bsize = 2;
		(*my)->sentinel = 0;
		REALLOC_M( my );
		return 0;
	}
	return 0;
}


SIG( pound_block )
{
	SIGGO();
	if ( *a == 2 ) {
		(*my)->action = *b; // Mark the action as a loop.
		uint8_t *c = b;
		int ind = 0, cc = 0; 
		while ( *c && *c != '}' ) c++, cc++;
		c = trim( b, " #\t", cc, &cc );
		(*my)->index = lt_get_long_i( t, c, cc );
		
		//I can differentiate between parents here.
		(*my)->parent = c;
		(*my)->psize = cc;
		(*my)->type = lt_rettype( t, 0, (*my)->index );
		(*my)->sentinel = 0;
		*p = *my;

		#ifdef MARKDUMPS
		fprintf( stderr, "pound_block: " );
		write(2,"'",1);write( 2, c, cc );write(2,"'",1);
		fprintf( stderr, " index: %d\n", (*my)->index );
		#endif

		REALLOC_M( my );
	}
	return 0;
}


SIG( dollar_block )
{
	if ( *a == 2 ) {
		(*my)->action = *b;
		(*my)->parent = (*p)->parent;
		(*my)->psize = (*p)->psize;

		int pind = (*p)->index;
		uint8_t *c = b;
		uint8_t s[ 2048 ] = { 0 };
		int ind = 0, cc = 0, ss = 0; 
		while ( *c && *c != '}' ) c++, cc++;
		c = trim( b, " $\t", cc, &cc );

		#if 0
		int isKey = 0;
		if ( (isKey = memcmp("$key", c, cc)) == 0 ) || memcmp("$val", c, cc) == 0 )
		{
			int side = (isKey) ? 0 : 1;
			//Loop through table entry
			while ( lt_get( side, ... ) != 0 ) {
				SETBLOBMOVE( c, cc );
			}
		}
		#endif
	}
}

SIG( period_block )
{
	SIGGO();
	if ( *a == 2 ) {
		(*my)->action = *b;
		(*my)->parent = (*p)->parent;
		(*my)->psize = (*p)->psize;

		int pind = (*p)->index;
		uint8_t *c = b;
		uint8_t s[ 2048 ] = { 0 };
		int ind = 0, cc = 0, ss = 0; 
		while ( *c && *c != '}' ) c++, cc++;
		c = trim( b, " #\t", cc, &cc );

		//A parent should always exist
		if ( (*my)->parent ) {
			COPYHASH( s, ss, (*my)->parent, (*my)->psize, c, cc, -1 );
			fprintf( stderr, "hash : " );write( 2, s, ss );fprintf( stderr, "\n" );

			//This depends on type...
			(*my)->index = lt_get_long_i( t, s, ss );

			if ( (*my)->index == -1 ) {
				fprintf( stderr, "hash2 amount of values: %d\n", lt_counti( t, (*p)->index ));
				for ( int x = 0; x < lt_counti( t, (*p)->index); x++ ) {
					ss = 0;
					memset( s, 0, sizeof( s ));
					COPYHASH( s, ss, (*my)->parent, (*my)->psize, c, cc, x );
					fprintf( stderr, "hash2: " );write( 2, s, ss );fprintf( stderr, "\n" );
					//(*my)->index = lt_get_long_i( t, s, ss );
					//SETBLOBMOVE();
				}
				//optionally loop through until reaching the end...
			}
			else {
				//I only found one value, save the reference to it 
				SETBLOBMOVE(NULL, 0);
			}
		}

		//If the parent element is a table, then I most likely need to loop
		(*my)->blob = NULL;
		(*my)->bsize = 0;	
		(*my)->sentinel = 0;

		REALLOC_M( my );
	}
	return 0;
}

SIG( slash_block ) 
{
	SIGGO();
	if ( *a == 2 ) {
		(*my)->action = *b;
		uint8_t *c = b;
		int cc = 0;
		while ( *c && *c != '}' ) c++, cc++;
		c = trim( b, " /\t", cc, &cc );
		(*my)->sentinel = 0;
		(*my)->index = -1;
		(*my)->blob = NULL;
		REALLOC_M(my);
	}
	return 0;
}

SIG( exclamation_block )
{
	SIGGO();
	(*my)->sentinel = 0;
	(*my)->action = *b;
	REALLOC_M(my);
	return 0;
}

SIG( squiggly_block ) 
{
	SIGGO();
	(*my)->sentinel = 0;
	(*my)->action = *b;
	REALLOC_M(my);
	return 0;
}

SIG( quote_block )
{
	SIGGO();
	(*my)->sentinel = 0;
	(*my)->action = *b;
	REALLOC_M(my);
	return 0;
}


int main (int argc, char *argv[])
{
	//Array of unsigned character data
	#if 0
	uint8_t block[] = 
		{ 255, 1, 0 }
	#else
	char block[] = 
	//Anything can go here....
		"{{ #head }}\n"
		"	Chapter {{ .number }}\n"
		"	<p>This is a full paragraph</p>\n"
		"	\"This is a string, and should be skipped\"\n"
		"{{ /head }}\n"
	#endif
	;

	fprintf( stderr, "\nBlock before pre-rendering looks like:\n" );
	fprintf( stderr, "=======================================\n" );
	write( 2, block, sizeof(block));

	//Text processing functions
	SIG((*pbArr[256])) = {
	#if 1
		 ['{' ] = left_curly_brace_block
		,['}' ] = right_curly_brace_block
		,['#' ] = pound_block
		,['/' ] = slash_block	
		,['!' ] = exclamation_block
		,['~' ] = squiggly_block	
		,['.' ] = period_block	
		,['"' ] = quote_block	
	#endif
	};

	//Table
	Table *t = convert_lkv( (LiteKv *)MT );
	fprintf( stderr, "\nTable to render from looks like:\n" );
	fprintf( stderr, "=======================================\n" );
	lt_dump( t );

	//Marker
	#ifndef SIGDYN
	Marker m[ 100 ] = { 0 };
	Marker *maf = m;
	#else
	Marker *maf = malloc( sizeof(Marker) );
	memset( maf, 0, sizeof(Marker) ); 
	#endif

	if ( !maf ) {
		fprintf( stderr, "Trouble reallocating markers...\n" );
		return 0;
	}

	//Move through the unsigned character data	
	uint8_t *b = (uint8_t *)block;
	Marker *parent = NULL;
	int act = 0, bct = 0;
	while ( *b ) {
		if ( pbArr[ *b ] ) {
			pbArr[ *b ]( b, &act, &bct, &parent, t, &maf );
		}
		b++;	
	}

#if 1
	//Let's see what the marker looks like...
	maf = m;
	fprintf( stderr, "\nContent pointers look like:\n" );
	fprintf( stderr, "=======================================\n" );
	while ( !maf->sentinel ) {
		SHOW_MARKER( maf );
		maf++;
	}	
#endif


	fprintf( stderr, "\nBlock after rendering looks like:\n" );
	fprintf( stderr, "=======================================\n" );
}

/*Compile me with: gcc -Wall -DSQROOGE_H single.c newt.c -o newt*/
