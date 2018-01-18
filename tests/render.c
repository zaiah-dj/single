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
	LOREMIPSUM[ (rand_number() % (sizeof( LOREMIPSUM ) / sizeof( const char *))) ]

#define RND_NUM() \
	random()

#define RND_FUNCT()

#if 0
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
#endif


typedef struct 
{
	const char *name  ,
						 *desc  ,
						 *renSrc,
						 *renCmp;
	LiteKv values[100];
	int    sentinel   ;
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
			{ TEXT_KEY( "def" ), INT_VALUE( 342 ) },
			{ TEXT_KEY( "ghi" ), INT_VALUE( 245 ) },
			{  INT_KEY( 553   ), INT_VALUE( 455 ) },
			{ TEXT_KEY( "jkl" ), INT_VALUE( 981 ) },
			{  INT_KEY( 8234  ), INT_VALUE( 477 ) },
			{  INT_KEY( 11    ), INT_VALUE( 343 ) },
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
		 "	<h2>245</h2>\n"
		 "	<p>\n"
		 "		Large angry deer are following me.\n"
		 "	</p>	\n"
		 "</body>\n"
		 "</html>\n"
	},


	//execution
	//userdata (and custom, close to execution)
	//one table
	//two tables
	//multi-level tables
	//key and value vs actual values
	{
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
		 "	<h2>245</h2>\n"
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

	{ .sentinel = 1 }
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




TEST( render )
{
	int set = 1;
	RenderTest *rt = r;
	char buf[ 2048 ];
	int destlen = 0;
	uint8_t *dest = NULL;

	//Loop through each RTest
	while ( !rt->sentinel )
	{
		//Define
		Render R;
		Table *t = NULL; 

		//Print the name and description 
		fprintf( stderr, "Name: %s\n", rt->name );
		fprintf( stderr, "Description: %s\n", rt->desc );

		//Convert the table
		if ((t = convert_lkv( rt->values )) == NULL ) {
			EPRINTF( "Failed to convert values to table.\n" );
		}

		//Print the table (if it crashes here, you have bigger problems)
		//lt_dump( t ); 
	
		//Run the test via the render thing
		if ( !render_init( &R, t ) ) {
			EPRINTF( "Failed to iniitalize render module at test #%d", set );
		}

		//Map
		if ( !render_map( &R, (uint8_t *)rt->renSrc, strlen( rt->renSrc ) ) ) {
			EPRINTF( "Failed to set render source correctly at test #%d", set );
		}

		//Dump the map? (is useful to see what's what)
		//render_dump_mark( &R );

		//Start replacing things
		if ( !render_render( &R ) ) {
			EPRINTF( "Failed to set render source correctly at test #%d", set );
		}

		//Retrieve rendered source and do something with it
		dest = bf_data( render_rendered( &R ) ); 
		destlen = bf_written ( render_rendered( &R )); 

		//We know something went wrong if destlen is zero.
		if ( !destlen ) {
			EPRINTF( "Templating failed somewhere.  Not sure where...\n" );
		}


		//Check rendered result against what should be
		if ( memcmp( rt->renCmp, dest, destlen ) == 0 )
			fprintf( stderr, "SUCCESS: Final matches expected!\n" );
		else {
			fprintf( stderr, "FAILED:  Final and expected do not match.\n" );
			nlprintf( strlen( rt->renCmp ) );
			niprintf( destlen );
			fprintf( stderr, "source:\n" );
			write( 2, rt->renCmp, strlen( rt->renCmp ));
			fprintf( stderr, "rendered:\n" );
			write( 2, dest, destlen );
			//EPRINTF( "Finished result did not match expected result" );
		}

		//Free everything
		//render_free( &R );
		lt_free( t );
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
