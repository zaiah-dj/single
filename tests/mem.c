#include "../single.h"
#include "harness.h"


//Write some data
#define memwrite(c,p) \
	write(2, "'", 1); write(2, c, p); write(2, "'\n", 2)

#define MEM_STRING_MAX 10
#define MEM_CMP_MAX 10


//Another test
typedef struct
{
	char *str;
	int  expected;
	int  actual;
} Cmp;

/*A test for this datatype*/
typedef struct 
{ 
	/*We just keep adding new rows to this struct to create additional tests*/
	Cmp chr[ MEM_CMP_MAX ];
	Cmp str[ MEM_CMP_MAX ];
	Cmp lchr[ MEM_CMP_MAX ];
	Cmp lstr[ MEM_CMP_MAX ];
	const char *test; 
	int sentinel; 
} MemTest;


/*Here are some tests for the memwalk algorithm*/
MemTest mem_tests[] = 
{
	{ 
		.chr  = { { " "   , 3 }, { "/"  , 0  }, { 0 } },
		.str  = { { "Face", 1 }, { "son", 1  }, { 0 } },
		.lchr = { { "s",   19 }, { "R",   12 }, { 0 } },
		.lstr = { { "Face", 7 }, { "son", 19 }, { 0 } },
		.test = "Senior Face Ritual son."
	},

	{ 
		.chr  = { { " "   , 9 }, { "o"  , 5 }, { 0 } },
		.str  = { { "quick",1 }, { "fox", 2 }, { 0 } },
		.lchr = { { "s",   24 }, { "R",   -1 }, { "f", 16 }, { 0 } },
		.lstr = { { "Face",-1 }, { "jump", 20 }, { 0 } },
		.test = "The quick brown fox jumps over the fox's lazy dog."
	},

	{ 
		.chr  = { { "/"   , 4 }, { "/"  ,  6 }, { 0 } },
		.str  = { { "team", 13 }, { "D"  , 1  }, { 0 } },
		.lchr = { { "s",    5 }, { "D",   -1  }, { "f", 16 }, { 0 } },
		.lstr = { { "nccu", 8 }, { "jump", -1 }, { 0 } },
		.test = "https://nccu.teamdynamix.com/TDNext/Home/Desktop/Default.aspx"
	},

	{ .sentinel = 1 }
};



/*memwalk is just about done*/
TEST( mem )
{
	// Cycle through some data. 
	// Test all the functions of the library with strings and uint8_t's
	MemTest *mt = mem_tests;

	while ( !mt->sentinel )
	{
		struct Test 
		{
			Cmp *cmp;
			int type;
			const char *fmt;
			int32_t ( *chrfp )( const void *a, const char  b, int32_t size );
			int32_t ( *strfp )( const void *a, const void *b, int32_t size );
			int actual;
		} tests[] = 
		{
			{ mt->chr, 0, "memchrocc", memchrocc },
			{ mt->str, 0, "memstrocc", NULL, memstrocc },
			{ mt->lchr, 1, "memchrat", memchrat },
			{ mt->lstr, 0,"memstrat",  NULL, memstrat },
		};

		//Tell me what we're about to work on?
		fprintf ( stderr, "Working with string: %s\n",  mt->test );

		for ( int i = 0; i < sizeof( tests ) / sizeof( struct Test ); i ++ )
		{
			//I used to do that thing
			struct Test *t = &tests[ i ];
			char buf[ 2048 ];
			int l = strlen( mt->test );

			//Move through each
			while ( t->cmp->str )
			{
				LPRINTF( "%s: Find '%s'", t->fmt, t->cmp->str );

				if ( t->chrfp )
					t->actual = t->chrfp( mt->test, *t->cmp->str, l ); 
				else {
					t->actual = t->strfp( mt->test, t->cmp->str, l );
				}
			
				if ( t->actual == t->cmp->expected )
					{ RPRINTF( "SUCCESS, Expected %d, got %d",  t->cmp->expected, t->actual ); }
				else
					{ RPRINTF( "FAILED , Expected %d, got %d",  t->cmp->expected, t->actual ); }
			
				t->cmp ++;
			}
		}

		fprintf( stderr, "\n" );
		mt ++;
	}

	return 0;
}
