#include "single.h"
#include "simpletables.h"
#define MARKDUMPS

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

typedef struct Marker 
{
	uint8_t *blob,    // Block between active terms.
					*parent;  // Parent block of text, if I'm within some kind of table
	int      bsize,   // Size of blob / block
					 psize,   // Size of parent block
					 action,  // Actions are no longer predefined, but they go here
					 type,    // ?
					 index;   // Where in the table is the value found
	struct   Marker
					*pmarker; // An easier way to do the non-recursive parent thing...
} Marker;



//This is the signature for text processing blocks.
int proc_block( uint8_t *b, int *a, Marker *my )
{
	fprintf( stderr, "Got a " );
	write( 2, b, 1 );
	fprintf( stderr, "\n" );
	return 0;
	//Should return false, true, and continue?
}

int left_curly_brace_block( uint8_t *b, int *a, Table *t, Marker *my )
{
	proc_block( b, a, my );
	if ( !*a ) {
		*a = 1;
		return 0;
	}
	else {
		*a = 2;
		return 1;
	}
	return 0;
}

int right_curly_brace_block( uint8_t *b, int *a, Table *t, Marker *my )
{
	proc_block( b, a, my );
	if ( *a == 2 ) {
		*a = 1;
		return 0;
	}
	else ( *a == 1 ) {
		*a = 0;
		return 0;
	}
	return 0;
}

int pound_block( uint8_t *b, int *a, Table *t, Marker *my )
{
	proc_block( b, a, my );

	if ( *a == 2 ) {
		my->action = 1; // Mark the action as a loop.
		uint8_t *c = b;
		int ind = 0, cc = 0; 
		while ( *c && *c != '}' ) c++, cc++;
		c = trim( b, " #\t", cc, &cc );
		my->index = lt_get_long_i( t, c, cc );
		my->parent = c;

		#ifdef MARKDUMPS
		fprintf( stderr, "pound_block: " );
		write(2,"'",1);write( 2, c, cc );write(2,"'",1);
		fprintf( stderr, " index: %d\n", my->index );
		#endif
	}
	return 0;
}

int period_block( uint8_t *b, int *a, Table *t, Marker *my )
{
	proc_block( b, a, my );
	if ( *a == 2 ) {
		my->action = 2;
		uint8_t *c = b;
		int ind = 0, cc = 0; 
		while ( *c && *c != '}' ) c++, cc++;
		c = trim( b, " #\t", cc, &cc );
		my->index = lt_get_long_i( t, c, cc );
	}
	return 0;
}

int slash_block( uint8_t *b, int *a, Table *t, Marker *my )
{
	proc_block( b, a, my );
	return 0;
}

int exclamation_block( uint8_t *b, int *a, Table *t, Marker *my )
{
	proc_block( b, a, my );
	return 0;
}

int squiggly_block( uint8_t *b, int *a, Table *t, Marker *my )
{
	proc_block( b, a, my );
	return 0;
}

int quote_block( uint8_t *b, int *a, Table *t, Marker *my )
{
	proc_block( b, a, my );
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

	//Text processing functions
	int (*pbArr[256])( uint8_t *, int *, Table *, Marker * ) = {
	#if 1
		 ['{' ] = left_curly_brace_block
		,['}' ] = right_curly_brace_block
		,['#' ] = pound_block
		,['/' ] = slash_block	
		,['!' ] = exclamation_block
		,['~' ] = squiggly_block	
		,['.' ] = period_block	
		,['"' ] = quote_block	
	#else
	#endif
	};

	//Table
	Table *t = convert_lkv( MT );
	lt_dump( t );

	//Marker
	Marker *maf = malloc( sizeof(Marker) );
	memset( maf, 0, sizeof(Marker) ); 

	//Move through the unsigned character data	
	uint8_t *b = (uint8_t *)block;
	int act = 0;
	while ( *b ) {
		if ( pbArr[ *b ] ) {
			pbArr[ *b ]( b, &act, t, maf );
		}
		b++;	
	}
}

/*Compile me with:
 * gcc -Wall -DSQROOGE_H single.c newt.c -o newt
 */
