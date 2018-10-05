#include "../single.h"
#include "harness.h"

//Testing this has really sucked.
//
// - Test that a list of LiteKv's can be read in and converted
// - Test that Models can be converted
// - Test the depth of Tables ( how many deep can you go until you can't read keys)
// - Test the length of table keys ( can things crash? )
// - Test adding bullshit key types ( only int, char*, and unsigned char * should work)
// - Check that you can find keys of different types and lengths
// - Test strdup'd keys

/*Max key searches*/
#define LKV_MAX_SEARCH 10

/*Defines for each key type in a Table*/
#define LKV_TERM -7

#define LKV_LAST \
	.hash = { LKV_TERM }

#define TEXT_KEY( str ) \
	.key = { LITE_TXT, .v.vchar = str } 

#define TEXT_VALUE( str ) \
	.value = { LITE_TXT, .v.vchar = str }

#define BLOB_KEY( b ) \
	.key = { LITE_BLB, .v.vblob = { sizeof( b ), (unsigned char *)b }}

#define BLOB_VALUE( b ) \
	.value = { LITE_BLB, .v.vblob = { sizeof( b ), (unsigned char *)b }}

#define INT_KEY( b ) \
	.key = { LITE_INT, .v.vint = b } 

#define INT_VALUE( b ) \
	.value = { LITE_INT, .v.vint = b } 

#define FLOAT_VALUE( b ) \
	.value = { LITE_FLT, .v.vfloat = b } 

#define USR_VALUE( b ) \
	.value = { LITE_USR, .v.vusrdata = b } 

#define TABLE_VALUE( b ) \
	.value = { LITE_TBL }

#define NULL_VALUE( ) \
	.value = { LITE_NUL }

#define TRM_VALUE( ) \
	.value = { LITE_TRM }

#define TRM( ) \
	.key = { LITE_TRM }

#define START_TABLEs( str ) \
	.key = { LITE_TXT, .v.vchar = str }, .value = { LITE_TBL }

#define START_TABLEi( num ) \
	.key = { LITE_INT, .v.vint = num }, .value = { LITE_TBL }

#define END_TABLE() \
	.key = { LITE_TRM }

//Here is a stream containing seemingly random data
//Here are some items that might be embedded in some table somewhere
const char    eat1[] = "fleeing rainbow";
const int     eat2   = 3243;
const float   eat3   = 532.29234;
const uint8_t eat4[] = { 243, 120, 213, 'a', 'b', 'c', 'd', 'e' };

//Here is a bunch of random data.
const char    dat1[] = "fleeing rainbow";
const int     dat2   = 3243;
const float   dat3   = 532.29234;
const uint8_t dat4[] = { 243, 120, 213, 'a', 'b', 'c', 'd', 'e' };
const void    *dat5  = NULL;

//Define a table like this (but let's leave it linear)
//Depth is done by the index (this is space heavy)

//A test datatype
typedef struct Test Test;
struct Test 
{
	const char  *name;
	void        *data;
	unsigned int size;
	_Bool (*test)(Test *t);
	
	int expected;
	int actual;
};


//Here is an example LiteKv structure
LiteKv kv0[] = 
{
	{ .key = { LITE_TXT, .v.vchar = "abc"}, .value = { LITE_TXT, .v.vchar = "def" } },
	{ .key = { LITE_TXT, .v.vchar = "abc"}, .value = { LITE_FLT, .v.vfloat= 342.32} },
	{ .key = { LITE_TXT, .v.vchar = "abc"}, .value = { LITE_TXT, .v.vchar = "def" } },
	{ .key = { LITE_TXT, .v.vchar = "abc"}, .value = { LITE_TXT, .v.vchar = "fff"} },
	{ .key = { LITE_INT, .v.vint  = 3234 }, .value = { LITE_TXT, .v.vchar = "def" } },
	{ .key = { LITE_INT, .v.vint  = 100  }, .value = { LITE_TXT, .v.vchar = "def" } },
	{ .key = { LITE_TXT, .v.vchar = "bin"}, .value = { LITE_BLB, .v.vblob = { sizeof(dat4), (uint8_t *)dat4}}}, 
	{ .key = { LITE_INT, .v.vint  = 32   }, .value = { LITE_INT, .v.vint  = 3222  } },
	{ .key = { LITE_TXT, .v.vchar = "abc"}, .value = { LITE_TXT, .v.vchar = "def" } },
	{ LKV_LAST } 
};


/*Regular table with no nested tables*/
LiteKv kv1[] = 
{
	{ TEXT_KEY( "abc" ), TEXT_VALUE( "def" ) },
	{ TEXT_KEY( "def" ), FLOAT_VALUE( 342.32 ) },
	{ TEXT_KEY( "ghi" ), TEXT_VALUE( "Indomitable" ) },
	{ TEXT_KEY( "jkl" ), /*NULL_VALUE( )*/ TEXT_VALUE( "orange julius" ) },
	{  INT_KEY( 3234  ), TEXT_VALUE( "def" ) },
	{  INT_KEY( 100   ), TEXT_VALUE( "def" ) },
	{ TEXT_KEY( "bin" ), BLOB_VALUE( dat4 ) },
	{  INT_KEY( 32    ),  INT_VALUE( 3222 ) },
	{ TEXT_KEY( "abc" ), TEXT_VALUE( "Large angry deer are following me." ) },
	{ LKV_LAST } 
};


/*Trivially small table*/
LiteKv kv2[] =
{
	{ TEXT_KEY( "samwise" ), TEXT_VALUE( "frodo" ) },
	{ TEXT_KEY( "gamgee" ), BLOB_VALUE( "abc" ) },
	{ LKV_LAST } 
};


/*One nested table*/
LiteKv kv3[] =
{
	{ TEXT_KEY( "samwise" ), TEXT_VALUE( "frodo" )  },
	{ TEXT_KEY( "gamgee" ), BLOB_VALUE( "mordor" )    },
	{ INT_KEY( 836 )       , TABLE_VALUE( )         },
		{ INT_KEY( 7001 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog." )  },
		{ INT_KEY( 7002 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog another time." )  },
		{ INT_KEY( 7003 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the 3rd time." )  },
		{ INT_KEY( 7004 )    , USR_VALUE ( NULL ) },
		{ INT_KEY( 7008 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog again." )  },
		{ INT_KEY( 7009 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the last time." )  },
	#if 0
		{ INT_KEY( 0 )       , TRM_VALUE( )           },	
	#endif
		{ TRM() },
	{ LKV_LAST } 
};


/*Two seperate tables*/
LiteKv kv4[] =
{
	{ TEXT_KEY( "samwise" ), TEXT_VALUE( "frodo" )  },
	{ TEXT_KEY( "samwise" ), BLOB_VALUE( "abc" )    },
	{ INT_KEY( 836 )       , TABLE_VALUE( )         },
		{ INT_KEY( 7001 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog." )  },
		{ INT_KEY( 7002 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog another time." )  },
		{ INT_KEY( 7003 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the 3rd time." )  },
		{ INT_KEY( 7004 )    , USR_VALUE ( NULL ) },
		{ INT_KEY( 7008 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog again." )  },
		{ INT_KEY( 7009 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the last time." )  },
#if 0
		{ INT_KEY( 0 )       , TRM_VALUE( )           },	
#endif
		{ TRM() },
	{ TEXT_KEY( "ashor" )  , TABLE_VALUE( )         },
		{ INT_KEY( 7043 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog." )  },
		{ INT_KEY( 7002 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog another time." )  },
		{ INT_KEY( 7003 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the 3rd time." )  },
		{ INT_KEY( 7004 )    , USR_VALUE ( NULL ) },
		{ INT_KEY( 7008 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog again." )  },
		{ INT_KEY( 7009 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the last time." )  },
#if 0
		{ INT_KEY( 0 )       , TRM_VALUE( )           },	
#endif
		{ TRM() },
	{ LKV_LAST } 
};


/*Two level deep tables*/
LiteKv kv5[] =
{
	{ TEXT_KEY( "nas" ), TEXT_VALUE( "frodo" )  },
	{ TEXT_KEY( "game" ), BLOB_VALUE( "abc" )    },
	{ INT_KEY( 70071 )       , TABLE_VALUE( )         },
		{ INT_KEY( 7001 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog." )  },
		{ INT_KEY( 7002 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog another time." )  },
		{ INT_KEY( 7003 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the 3rd time." )  },
		{ INT_KEY( 7004 )    , USR_VALUE ( NULL ) },
		{ INT_KEY( 7008 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog again." )  },
		{ INT_KEY( 7009 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the last time." )  },
		{ TEXT_KEY( "ashor" )  , TABLE_VALUE( )         },
			{ INT_KEY( 7043 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog." )  },
			{ INT_KEY( 7002 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog another time." )  },
			{ INT_KEY( 7003 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the 3rd time." )  },
			{ INT_KEY( 7004 )    , USR_VALUE ( NULL ) },
			{ INT_KEY( 7008 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog again." )  },
			{ INT_KEY( 7009 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the last time." )  },
			{ TRM() },
		{ TRM() },
#if 0
			{ INT_KEY( 0 )       , TRM_VALUE( )           },	
		{ INT_KEY( 0 )       , TRM_VALUE( )           },	
#endif
	{ LKV_LAST } 
};



/*Five levels deep*/
LiteKv kv6[] =
{
	{ TEXT_KEY( "nas" ), TEXT_VALUE( "frodo" )  },
	{ INT_KEY( 80800 ), BLOB_VALUE( "abc" )    },
	/*Level 1*/
	{ TEXT_KEY( "abcdef" ) , TABLE_VALUE( )         },
		{ INT_KEY( 7001 )    , TEXT_VALUE( "Terrible." )  },
		{ INT_KEY( 7002 )    , TEXT_VALUE( "Terrible another time." )  },
		{ INT_KEY( 7003 )    , TEXT_VALUE( "Terrible for the 3rd time." )  },
		{ INT_KEY( 7004 )    , USR_VALUE ( NULL ) },
		{ INT_KEY( 7008 )    , TEXT_VALUE( "Terrible again." )  },
		{ INT_KEY( 7009 )    , TEXT_VALUE( "Terrible for the last time." )  },
	/*Level 2*/
		{ TEXT_KEY( "soulblade" )  , TABLE_VALUE( )         },
			{ INT_KEY( 81 )    , TEXT_VALUE( "Terrible again." )  },
			{ INT_KEY( 88 )    , TEXT_VALUE( "Terrible for the last time." )  },
	/*Level 3*/
			{ TEXT_KEY( "seung-mina" )  , TABLE_VALUE( )         },
				{ INT_KEY( 7043 )    , TEXT_VALUE( "Terrible." )  },
				{ INT_KEY( 7044 )    , TEXT_VALUE( "Terrible another time." )  },
				{ INT_KEY( 7045 )    , TEXT_VALUE( "Terrible for the 3rd time." )  },
				{ INT_KEY( 7074 )    , USR_VALUE ( NULL ) },
				{ INT_KEY( 7089 )    , TEXT_VALUE( "Terrible again." )  },
				{ INT_KEY( 7089 )    , TEXT_VALUE( "Terrible for the last time." )  },
				{ TRM() },
			#if 0
				{ INT_KEY( 0 )       , TRM_VALUE( )           },	
			#endif
	/*Level 3*/
			{ TEXT_KEY( "hwang" )  , TABLE_VALUE( )         },
				{ INT_KEY( 7043 )    , TEXT_VALUE( "Terrible." )  },
				{ INT_KEY( 7044 )    , TEXT_VALUE( "Terrible another time." )  },
				{ INT_KEY( 7089 )    , TEXT_VALUE( "Terrible for the last time." )  },
	/*Level 4*/
				{ TEXT_KEY( "xianghua" )  , TABLE_VALUE( )         },
					{ INT_KEY( 7043 )    , TEXT_VALUE( "Terrible." )  },
					{ INT_KEY( 7044 )    , TEXT_VALUE( "Terrible another time." )  },
					{ INT_KEY( 7089 )    , TEXT_VALUE( "Terrible for the last time." )  },
					{ TRM() },
				{ TRM() },
		#if 0
					{ INT_KEY( 0 )       , TRM_VALUE( )           },	
				{ INT_KEY( 0 )       , TRM_VALUE( )           },	
		#endif
	/*Level 3*/
			{ TEXT_KEY( "siegfried" )  , TABLE_VALUE( )         },
				{ INT_KEY( 7043 )    , TEXT_VALUE( "Terrible." )  },
				{ INT_KEY( 7044 )    , TEXT_VALUE( "Terrible another time." )  },
				{ INT_KEY( 7089 )    , TEXT_VALUE( "Terrible for the last time." )  },
	/*Level 4*/
				{ TEXT_KEY( "blackknight" )  , TABLE_VALUE( )         },
					{ INT_KEY( 7043 )    , TEXT_VALUE( "Terrible." )  },
					{ INT_KEY( 7044 )    , TEXT_VALUE( "Terrible another time." )  },
					{ INT_KEY( 7089 )    , TEXT_VALUE( "Terrible for the last time." )  },
	/*Level 5*/
					{ TEXT_KEY( "nightmare" )  , TABLE_VALUE( )         },
						{ INT_KEY( 7043 )    , TEXT_VALUE( "Terrible." )  },
						{ INT_KEY( 7044 )    , TEXT_VALUE( "Terrible another time." )  },
						{ INT_KEY( 7089 )    , TEXT_VALUE( "Terrible for the last time." )  },
						{ TRM() },
					{ TRM() },
				{ TRM() },
			{ TRM() },
		{ TRM() },
#if 0
						{ INT_KEY( 0 )       , TRM_VALUE( )           },	
					{ INT_KEY( 0 )       , TRM_VALUE( )           },	
				{ INT_KEY( 0 )       , TRM_VALUE( )           },	
			{ INT_KEY( 0 )       , TRM_VALUE( )           },	
		{ INT_KEY( 0 )       , TRM_VALUE( )           },	
#endif
	{ LKV_LAST } 
};


/*Here's a list of these that can be easily iterated through*/
typedef struct TestKv 
{
	LiteKv *lkv;	
	const char *search[ LKV_MAX_SEARCH ];
} TestKv;


/*Each test should find all the keys we ask for*/
TestKv tests_kv[] = 
{
	{ kv1, { "abc", "3234", "jkl", NULL } },
	{ kv2, { "samwise", "gamgee", NULL } },
	{ kv3, { "836.7008", "836.7009", NULL } },
	{ kv6, { "abcdef.soulblade.seung-mina", "abcdef.soulblade.siegfried.blackknight.nightmare", NULL } },
#if 0
	{ kv4, { "a", "b", "c", NULL } },
#endif
	{ NULL }
};



//Get count of kv, since for these tests, that's what I'll use
int get_count ( LiteKv *kv )
{
	int a = 0;
	while ( *kv->hash != LKV_TERM ) 
		a ++, kv ++;
	return a;	
}


//convert
Table *convert_lkv ( LiteKv *kv )
{
	char buf[ BUFLEN ];
	Table *t = malloc( sizeof( Table ) );
	lt_init( t, NULL, get_count( kv ) );

	while ( *kv->hash != LKV_TERM )
	{
		if ( kv->key.type == LITE_TXT )
			lt_addtextkey( t, kv->key.v.vchar );
		else if ( kv->key.type == LITE_BLB )
			lt_addblobkey( t, kv->key.v.vblob.blob, kv->key.v.vblob.size );
		else if ( kv->key.type == LITE_INT )
			lt_addintkey( t, kv->key.v.vint );
		else if ( kv->key.type == LITE_TRM )
		{
			lt_ascend( t );
			kv ++;
			continue;
		}
		else 
		{
			//Abort immediately b/c this is an error
			EPRINTF( "Attempted to add wrong key type!  Bailing!" );
			exit( 0 );	
		}

		if ( kv->value.type == LITE_TXT )
			lt_addtextvalue( t, kv->value.v.vchar );
		else if ( kv->value.type == LITE_BLB )
			lt_addblobvalue( t, kv->value.v.vblob.blob, kv->value.v.vblob.size );
		else if ( kv->value.type == LITE_INT )
			lt_addintvalue( t, kv->value.v.vint );
		else if ( kv->value.type == LITE_FLT )
			lt_addfloatvalue( t, kv->value.v.vfloat );
		else if ( kv->value.type == LITE_USR )
			lt_addudvalue( t, kv->value.v.vusrdata );
		else if ( kv->value.type == LITE_NUL )
			0;//lt_ascend has already been called, thus we should never reach this
		else { 
			if ( kv->value.type != LITE_TBL ) {
				//Abort immediately b/c this is an error
				EPRINTF( "Got unknown or invalid key type!  Bailing!" );
				return NULL;
			}
		}

		if ( kv->value.type != LITE_TBL )
			lt_finalize( t );
		else {
			lt_descend( t );
			kv ++;
			continue;
		}
		kv ++;
	}
	lt_lock( t );
	return t;
}



//int main (int argc, char *argv[]) 
TEST( tab )
{
	int set = 1;
	char buf [ BUFLEN ];
	TestKv *j = tests_kv;

	while ( j->lkv ) 
	{
		//Convert the key values to Table 
		Table *t = NULL;
		if ( (t = convert_lkv( j->lkv )) == NULL ) 
		{
			EPRINTF( "Failed to convert #%d set of key values", set );
		}

		//Always dump (you could theoretically use memcmp/strcmp to check this)
		lt_dump( t );

		//Simply find each key requested (for now)
		//This can be used to check for overflow or faulty hash tabling
		const char **search = j->search;
		while ( *search ) 
		{
			int index = lt_geti( t, *search );
		#if 0
			fprintf( stderr, "key to search: %s\n", *search );
			fprintf( stderr, "key index at:  %d\n", index );
		#endif
			LPRINTF( "Found key %s in table at %p?", *search, ( void * )t );  
			if ( index == -1 )
			{
				RPRINTF( "FAILED" );
			}
			else 
			{
				RPRINTF( "SUCCESS ( Hash resolved to %d ) ", index );
			}
			search++;
		}
		
		//Free the values and the table
		lt_free( t );
		free( t );

		//Next
		j++;
	}

	return 0;
}
