#include "../single.h"
#include "harness.h"

/* -------------------------------------- *
 * TODO:
 *
 * - Keys that don't exist don't seem to 
 * work (i.e. when not there, the rest of
 * the render src does not show)
 *
 * - Test other types of data
 *
 * - Test function pointers....
 *
 * - Test inner loops...
 *
 * - Test key / value iteration
 * 
 * - Test all the syntax:
 *   {{   x }}  = Replace x with the value of x in the table
 *   {{#  x }},
 *   	...
 *   {{/  x }}  = Replace every target in ... with values
 *   							from the source table.
 *   {{$  k }}  = Replace k with the key of the current 
 *   							key-value pair in the current table 
 *   							(whether numeric or not)
 *   {{$  v }}  = Replace v with the value of the current 
 *   							key-value pair in the current table 
 *   {{!  x }}  = ???
 *   {{+  x }}  = ???
 *   {{=  x }}  = ???
 *   {{@  x }}  = ???
 *   							
 * -------------------------------------- */



//Pull random lines from an array of const char *s
#define RND_TEXT() \
	LOREMIPSUM[ random() % ( sizeof( LOREMIPSUM ) / sizeof( const char *) ) ]

#define RND_NUM() \
	random()

#define RND_FUNCT()


const char rs2[] =
 "<html>\n"
 "<head>\n"
 "</head>\n"
 "<body>\n"
 "	<h2>{{ durham }}</h2>\n"
 "	<p>\n"
 "  	<table>\n"
 "		{{# artillery }}\n"
 "    \t<tr>\n"
 "  	\t<td>{{ .val }}</td>\n"
 "  	\t<td>{{ .rec }}</td>\n"
 "    \t</tr>\n"
 "		{{/ artillery }}\n"
 "  	</table>\n"
 "	</p>	\n"
 "</body>\n"
 "</html>\n"
;


const char rs25[] =
 "<html>\n"
 "<head>\n"
 "</head>\n"
 "<body>\n"
 "	<h2>{{ durham }}</h2>\n"
 "	<p>\n"
 "  	<table>\n"
 "		{{# markteka }}\n"
 "    \t<tr>\n"
 "  	\t<td>{{ .val }}</td>\n"
 "  	\t<td>{{ .rec }}</td>\n"
 "    \t</tr>\n"
 "		{{/ markteka }}\n"
 "  	</table>\n"
 "	</p>	\n"
 "</body>\n"
 "</html>\n"
;

const char rs3[] =
 "<html>\n"
 "<head>\n"
 "</head>\n"
 "<body>\n"
 "	<p>\n"
 "  	<table>\n"
 "		{{# root.data }}\n"
 "    \t<tr>\n"
 "  	\t<td>{{ .id }}</td>\n"
 "  	\t<td>{{ .message }}</td>\n"
 "  	\t<td>{{ .actions }}</td>\n"
 "    \t</tr>\n"
 "		{{/ root.data }}\n"
 "  	</table>\n"
 "	</p>	\n"
 "</body>\n"
 "</html>\n"
;


typedef struct 
{
	const char *name  ,
						 *desc  ,
						 *renSrc,
						 *renCmp;
	LiteKv values[100];
} RenderTest;



RenderTest r[] = 
{
	//These should be pretty easy to read:
	//.name   = Name of the test
	//.desc   = A quick description of the test
	//.renSrc = the input that the test will use for find and replace
	//.renCmp = the constant to compare against to make sure that rendering worked
	//.values = the Table to use for values (these tests do not test any parsing)

#if 0	
	{ // 1 
		.name   = "Integers",
		.desc   = "Integers",

		.renSrc = 	
			 "<html>\n"
			 "<head>\n"
			 "</head>\n"
			 "<body>\n"
			 "	<h2>{{ ghi }}</h2>\n"
			 "	<p>\n"
			 "		{{ abc }}\n"
			 "	</p>	\n"
			 "</body>\n"
			 "</html>\n"
		,

		.renCmp = 
			""
		,

		.values = 
		{
			{ TEXT_KEY( "abc" ), TEXT_VALUE( "def" ) },
			{ TEXT_KEY( "def" ), FLOAT_VALUE( 342.32 ) },
			{ TEXT_KEY( "ghi" ), TEXT_VALUE( "Indomitable" ) },
			{ TEXT_KEY( "abc" ), TEXT_VALUE( "Large angry deer are following me." ) },
			{ LKV_LAST } 
		},
	},

	{
		.name   = "Integers",
		.desc   = "Integers",
		.values = 
		{
			{ INT_KEY( 33 ), TEXT_VALUE( "def" ) },
			{ INT_KEY( 53 ), FLOAT_VALUE( 342.32 ) },
			{ INT_KEY( 1  ), BLOB_VALUE( dat4 ) },
			{ INT_KEY( 32 ),  INT_VALUE( 3222 ) },
			{ INT_KEY( 2  ), TEXT_VALUE( "Large angry deer are following me." ) },
			{ LKV_LAST } 
		},

		.renSrc =
		 "<html>\n"
		 "<head>\n"
		 "</head>\n"
		 "<body>\n"
		 "	<h2>{{ ghi }}</h2>\n"
		 "	<p>\n"
		 "		{{ abc }}\n"
		 "	</p>	\n"
		 "</body>\n"
		 "</html>\n"
		,

		.renCmp = 
			""

	},
#endif

	{
		.name = "mixed_keys",
		.desc = "Mixed keys",
		.values = {
			{ TEXT_KEY( "zxy" ), TEXT_VALUE( "def" ) },
			{ TEXT_KEY( "def" ), FLOAT_VALUE( 342.32 ) },
			{ TEXT_KEY( "ghi" ), FLOAT_VALUE( 245.01 ) },
			{  INT_KEY( 553   ), FLOAT_VALUE( 455.37 ) },
			{ TEXT_KEY( "jkl" ), FLOAT_VALUE( 981.68 ) },
			{  INT_KEY( 8234  ), FLOAT_VALUE( 477.99 ) },
			{  INT_KEY( 11    ), FLOAT_VALUE( 343.01 ) },
			{ TEXT_KEY( "bin" ), BLOB_VALUE( dat4 ) },
			{  INT_KEY( 32    ),  INT_VALUE( 3222 ) },
			{ TEXT_KEY( "abc" ), TEXT_VALUE( "Large angry deer are following me." ) },
			{ LKV_LAST } 
		},

		.renSrc =
		 "<html>\n"
		 "<head>\n"
		 "</head>\n"
		 "<body>\n"
		 "	<h2>{{ ghi }}</h2>\n"
		 "	<p>\n"
		 "		{{ abc }}\n"
		 "	</p>	\n"
		 "</body>\n"
		 "</html>\n"
		,

		.renCmp = 
		 "<html>\n"
		 "<head>\n"
		 "</head>\n"
		 "<body>\n"
		 "	<h2>245.01</h2>\n"
		 "	<p>\n"
		 "		Large angry deer are following me.\n"
		 "	</p>	\n"
		 "</body>\n"
		 "</html>\n"
	},


#if 0
	{

.name = "",
.desc = "",
.values = {
	{ TEXT_KEY( "baltimore" ), TEXT_VALUE( "City of Dreams" )  },
	{ TEXT_KEY( "durham" ), BLOB_VALUE( "City of Chicken and Guns" )    },

	//This is much easier to see
	//{ START_TABLE( "artillery" ) }
	{ TEXT_KEY( "artillery" )       , TABLE_VALUE( )         },
		/*Database records look a lot like this*/
		{ INT_KEY( 0 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "val" ), BLOB_VALUE( "MySQL makes me tired." ) },
			{ TEXT_KEY( "rec" ), TEXT_VALUE( "Choo choo cachoo." ) },
			{ TRM() },
		{ TRM() },
	#if 0
			{ INT_KEY( 0 )     , TRM_VALUE( )           },	
		{ INT_KEY( 0 )     , TRM_VALUE( )           },	
	#endif

	{ TEXT_KEY( "ashor" )  , TABLE_VALUE( )         },
		{ INT_KEY( 7043 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog." )  },
		{ INT_KEY( 7002 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog another time." )  },
		{ INT_KEY( 7003 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the 3rd time." )  },
		{ INT_KEY( 7004 )    , USR_VALUE ( NULL ) },
		{ INT_KEY( 7008 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog again." )  },
		{ INT_KEY( 7009 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the last time." )  },
		{ TRM() },
	#if 0
		{ INT_KEY( 0 )       , TRM_VALUE( )           },	
	#endif
	{ LKV_LAST } 
},

.renSrc = 
	"",

.renCmp =
	"",

	},
#endif
};




LiteKv level_1_table_alpha[] = 
{
	{ TEXT_KEY( "abc" ), TEXT_VALUE( "def" ) },
	{ TEXT_KEY( "def" ), FLOAT_VALUE( 342.32 ) },
	{ TEXT_KEY( "ghi" ), TEXT_VALUE( "Indomitable" ) },
	{ TEXT_KEY( "abc" ), TEXT_VALUE( "Large angry deer are following me." ) },
	{ LKV_LAST } 
};


const char rs1[] =
 "<html>\n"
 "<head>\n"
 "</head>\n"
 "<body>\n"
 "	<h2>{{ ghi }}</h2>\n"
 "	<p>\n"
 "		{{ abc }}\n"
 "	</p>	\n"
 "</body>\n"
 "</html>\n"
;



LiteKv level_1_table_numeric [] = 
{
	{ INT_KEY( 33 ), TEXT_VALUE( "def" ) },
	{ INT_KEY( 53 ), FLOAT_VALUE( 342.32 ) },
	{ INT_KEY( 1  ), BLOB_VALUE( dat4 ) },
	{ INT_KEY( 32 ),  INT_VALUE( 3222 ) },
	{ INT_KEY( 2  ), TEXT_VALUE( "Large angry deer are following me." ) },
	{ LKV_LAST } 
};


LiteKv level_1_table_mixed [] = 
{
	{ TEXT_KEY( "abc" ), TEXT_VALUE( "def" ) },
	{ TEXT_KEY( "def" ), FLOAT_VALUE( 342.32 ) },
	{ TEXT_KEY( "def" ), FLOAT_VALUE( 342.32 ) },
	{  INT_KEY( 553   ), FLOAT_VALUE( 342.32 ) },
	{ TEXT_KEY( "def" ), FLOAT_VALUE( 342.32 ) },
	{  INT_KEY( 8234  ), FLOAT_VALUE( 342.32 ) },
	{  INT_KEY( 11    ), FLOAT_VALUE( 342.32 ) },
	{ TEXT_KEY( "bin" ), BLOB_VALUE( dat4 ) },
	{  INT_KEY( 32    ),  INT_VALUE( 3222 ) },
	{ TEXT_KEY( "abc" ), TEXT_VALUE( "Large angry deer are following me." ) },
	{ LKV_LAST } 
};


LiteKv rn3[] = 
{
	{ TEXT_KEY( "abc" ), FLOAT_VALUE( 3212.0001 ) },
	{ TEXT_KEY( "def" ), FLOAT_VALUE( 342.32 ) },
	{ TEXT_KEY( "ghi" ), INT_VALUE( 32042384 ) },
	{ TEXT_KEY( "abc" ), TEXT_VALUE( "Large angry deer are following me." ) },
	{ LKV_LAST } 
};


/*One table with one value*/
LiteKv rn4[] =
{
	{ TEXT_KEY( "baltimore" ), TEXT_VALUE( "City of Dreams" )  },
	{ TEXT_KEY( "durham" ), BLOB_VALUE( "City of Chicken and Guns" )    },

	//This is much easier to see
	//{ START_TABLE( "artillery" ) }
	{ TEXT_KEY( "artillery" )       , TABLE_VALUE( )         },
		/*Database records look a lot like this*/
		{ INT_KEY( 0 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "val" ), BLOB_VALUE( "MySQL makes me tired." ) },
			{ TEXT_KEY( "rec" ), TEXT_VALUE( "Choo choo cachoo." ) },
			{ TRM() },
		{ TRM() },
	#if 0
			{ INT_KEY( 0 )     , TRM_VALUE( )           },	
		{ INT_KEY( 0 )     , TRM_VALUE( )           },	
	#endif

	{ TEXT_KEY( "ashor" )  , TABLE_VALUE( )         },
		{ INT_KEY( 7043 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog." )  },
		{ INT_KEY( 7002 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog another time." )  },
		{ INT_KEY( 7003 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the 3rd time." )  },
		{ INT_KEY( 7004 )    , USR_VALUE ( NULL ) },
		{ INT_KEY( 7008 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog again." )  },
		{ INT_KEY( 7009 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the last time." )  },
		{ TRM() },
	#if 0
		{ INT_KEY( 0 )       , TRM_VALUE( )           },	
	#endif
	{ LKV_LAST } 
};

/*Two seperate tables*/
LiteKv rn5[] =
{
	{ TEXT_KEY( "baltimore" ), TEXT_VALUE( "City of Dreams" )  },
	{ TEXT_KEY( "durham" ), BLOB_VALUE( "City of Chicken and Guns" )    },
	{ TEXT_KEY( "markteka" )       , TABLE_VALUE( )         },
		/*Database records look a lot like this*/
		{ INT_KEY( 0 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "val" ), BLOB_VALUE( "MySQL makes me tired." ) },
			{ TEXT_KEY( "rec" ), TEXT_VALUE( "Choo choo cachoo." ) },
			{ TRM() },

			//{ INT_KEY( 0 )     , TRM_VALUE( )           },	

		{ INT_KEY( 1 )    , TABLE_VALUE( )         },
			{ TEXT_KEY( "val" ), BLOB_VALUE( "SQLite makes me tired." ) },
			{ TEXT_KEY( "rec" ), TEXT_VALUE( "Choo choo cachoo." ) },
			//{ INT_KEY( 0 )     , TRM_VALUE( )           },	
			{ TRM() },

		{ INT_KEY( 2 )    , TABLE_VALUE( )         },
			{ TEXT_KEY( "val" ), BLOB_VALUE( "PostgreSQL makes me tired." ) },
			{ TEXT_KEY( "rec" ), TEXT_VALUE( "Choo choo cachoo." ) },
			{ TRM() },
			//{ INT_KEY( 0 )     , TRM_VALUE( )           },	
		//{ INT_KEY( 0 )       , TRM_VALUE( )           },	
		{ TRM() },

	{ TEXT_KEY( "ashor" )  , TABLE_VALUE( )         },
		{ INT_KEY( 7043 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog." )  },
		{ INT_KEY( 7002 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog another time." )  },
		{ INT_KEY( 7003 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the 3rd time." )  },
		{ INT_KEY( 7004 )    , USR_VALUE ( NULL ) },
		{ INT_KEY( 7008 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog again." )  },
		{ INT_KEY( 7009 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the last time." )  },
		{ TRM() },
		//{ INT_KEY( 0 )       , TRM_VALUE( )           },	
	{ LKV_LAST } 
};


/*JSON test string*/
const char json_text[] = 
"{ \"data\": ["
"      {"
"         \"id\": \"X999_Y999\","
"         \"from\": {"
"            \"name\": \"Tom Brady\", \"id\": \"X12\""
"         },"
"         \"message\": \"Looking forward to 2010!\","
"         \"actions\": ["
"            {"
"               \"name\": \"Comment\","
"               \"link\": \"http://www.facebook.com/X999/posts/Y999\""
"            },"
"            {"
"               \"name\": \"Like\","
"               \"link\": \"http://www.facebook.com/X999/posts/Y999\""
"            }"
"         ],"
"         \"type\": \"status\","
"         \"created_time\": \"2010-08-02T21:27:44+0000\","
"         \"updated_time\": \"2010-08-02T21:27:44+0000\""
"      },"
"      {"
"         \"id\": \"X998_Y998\","
"         \"from\": {"
"            \"name\": \"Peyton Manning\", \"id\": \"X18\""
"         },"
"         \"message\": \"Where's my contract?\","
"         \"actions\": ["
"            {"
"               \"name\": \"Comment\","
"               \"link\": \"http://www.facebook.com/X998/posts/Y998\""
"            },"
"            {"
"               \"name\": \"Like\","
"               \"link\": \"http://www.facebook.com/X998/posts/Y998\""
"            }"
"         ],"
"         \"type\": \"status\","
"         \"created_time\": \"2010-08-02T21:27:44+0000\","
"         \"updated_time\": \"2010-08-02T21:27:44+0000\""
"      }"
"   ]"
"}"
;


/*All tests in one table*/
/*Use the same tables from tab.c, but use embedded text for each render*/
typedef struct RTest RTest;
struct RTest
{
	LiteKv     *kv;     /*Source Table*/
	const char *render; /*Source tags*/
	const char *fmt   ; /*A message*/
	enum RTestTypes 
	{ 
		R_NONE, 
		R_JSON, 
		R_SQL 
	}           type  ;
	const char *str   ;
	uint8_t    *dest  ; /*Destination*/
	int         destlen;
	int         sentinel;
};




RTest rtests[] =
{
 #if 0
	{ level_1_deep_table, rs1, "Single result with long words" },
	{ level_2_deep_table, rs1, "Single result with non existent values and long words" },
	{ level_3_deep_table, rs1, "Ints and floats" },
	{ level_4_deep_table, rs2, "Tabular values" },  /*Non-existent loops*/
 #endif
	{ rn4, rs2, "Tabular values" },  /*All values exist*/
	{ rn5, rs25, "Tabular values" }, 
 #if 0
 #ifndef JSON_H
	/*Tests JSON conversions*/
	{ NULL, rs3, "Tabular values", R_JSON, json_text }, /*One row table*/
	{ NULL, rs3, "Tabular values", R_JSON, json_text }, /*Multi row table*/
 #endif
 #endif
 #ifndef SQROOGE_H
	/*Tests SQL result conversions*/
 #endif
	{ .sentinel = 1 }
};




TEST( render )
{
	int set = 1;
	RTest *rt = rtests;
	char buf[ 2048 ];

	//Make a random seed for the random number generator.
	srandom( );

	//Loop through each RTest
	while ( !rt->sentinel )
	{
		//Define
		Render R;
		Table *t = NULL; 

		//Print the results
		fprintf( stderr, "%s\n", rt->fmt );

		if ( rt->type == R_NONE )
		{
			//Convert the values to a real table
			if ( !( t = convert_lkv( rt->kv )) )
			{
				EPRINTF( "Failed to convert #%d set of key values in %s", set, __FILE__ );
			}
		}
		else if ( rt->type == R_JSON )
		{
			/*....*/
			t = malloc( sizeof( Table ) );
			memset( t, 0, sizeof( Table ) );
			if ( !json_json( t, (uint8_t *)rt->str, strlen( rt->str ) ) )
			{
				EPRINTF( "Failed to convert JSON at #%d set of key values in %s", set, __FILE__ );
			}
		}

		lt_dump( t );
		if ( !render_init( &R, t ) )
		{
			EPRINTF( "Failed to iniitalize render module at test #%d", set );
		}

		if ( !render_map( &R, (uint8_t *)rt->render, strlen( rt->render ) ) )
		{
			EPRINTF( "Failed to set render source correctly at test #%d", set );
		}

		if ( !render_render( &R ) )
		{
			EPRINTF( "Failed to set render source correctly at test #%d", set );
		}


		//Retrieve rendered source and do something with it
		rt->dest = bf_data( render_rendered( &R ) );
		rt->destlen = bf_written ( render_rendered( &R ) );
		write( 2, rt->dest, rt->destlen );
		

		//Free everything
		render_free( &R );
		lt_free( t );
		if ( rt->type == R_JSON || rt->type == R_SQL )
		{
			free( t );
		}
		set ++, rt ++;
	}

#if 0
	//Split the files received into two
	uint8_t *b1 = NULL,
					*b2 = NULL;
          *src= NULL:
	int b1_len  = 0,
      b2_len  = 0,
      srcpos  = 0;
 #if 0
	//Find the sequence
	if ( !memchr( TEST_BYTES, *seq, TEST_BYTES_LENGTH ) )
		return xerr( 1, "Failed to split file. Test failed." );
 #else
	b2 = b1 = TEST_BYTES;
	b2_len = TEST_BYTES_LENGTH;
	b1_len = 0;

	while ( 1 )
	{
		if ( *b2 == *seq && memcmp( b2, seq, 8 ) )
		{
			b2 += 8;
			b2_len -= 8;	
		}
		b2++, b1_len++, b2_len--;
	}
 #endif

	//Serialize JSON, b/c I'm using that as my model	
	if ( !json_json( &tt, TEST_BYTES, TEST_BYTES_LENGTH ) )
		return perr( 1, "You're going to die...\n" );

	//Memset/clean this structure
	if ( !render_init( &R, &tt ) )
		return xerr( 1, "Failed to initialize...\n" );	

	//
	if ( !render_map( &R, f[1].src, f[1].len ) )
		return xerr( 1, "Failed to render view: %s\n", vfile );

	//Render something
	if ( !render_render( &R ) ) //, f[1].src, f[1].len ) )
		return xerr( 1, "Failed to render view: %s\n", vfile );

	//See what was rendered 
	if ( !(rr = render_rendered( &R )) )
		return xerr( 1, "Other error occurred... Can't render\n" );
#endif
	return 0;
}
