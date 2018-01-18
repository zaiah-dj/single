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
 * - Add a function:
 *   render_???( Render *r, char *, (void *)( ... ))
 *   This should allow a dev to setup some kind of custom action for a new term 
 *
 * PROBLEMS:
 * - FLOAT_VALUE looks to be using a printf modifier (it's too many places out)
 * - Looping only goes from max to min
 * - Still getting that extra row
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
				{ INT_KEY( 1 )       , TABLE_VALUE( )         },
					{ TEXT_KEY( "val" ), BLOB_VALUE( "MySQL makes me giddy." ) },
					{ TEXT_KEY( "rec" ), TEXT_VALUE( "Choo choo cachoo." ) },
					{ TRM() },
				{ INT_KEY( 2 )       , TABLE_VALUE( )         },
					{ TEXT_KEY( "val" ), BLOB_VALUE( "MySQL makes me ecstatic." ) },
					{ TEXT_KEY( "rec" ), TEXT_VALUE( "Choo choo cachoo." ) },
					{ TRM() },
				{ INT_KEY( 3 )       , TABLE_VALUE( )         },
					{ TEXT_KEY( "val" ), BLOB_VALUE( "MySQL makes me a bother." ) },
					{ TEXT_KEY( "rec" ), TEXT_VALUE( "Choo choo cachoo." ) },
					{ TRM() },
				{ TRM() },

			{ TEXT_KEY( "ashor" )  , TABLE_VALUE( )         },
				{ INT_KEY( 7043 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog." )  },
				{ INT_KEY( 7002 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog another time." )  },
				{ INT_KEY( 7003 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the 3rd time." )  },
				{ INT_KEY( 7004 )    , USR_VALUE ( NULL ) },
				{ INT_KEY( 7008 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog again." )  },
				{ INT_KEY( 7009 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the last time." )  },
				{ TRM() },
			{ LKV_LAST } 
		},

		.renSrc =
		 "<html>\n"
		 "<head>\n"
		 "</head>\n"
		 "<body>\n"
		 "{{# artillery }}\n"
		 "	<h2>{{ .rec }}</h2>\n"
		 "	<p>\n"
		 "		{{ .val }}\n"
		 "	</p>	\n"
		 "{{/ artillery }}\n"
		 "</body>\n"
		 "</html>\n"
		,

		.renCmp = 
		 "<html>\n"
		 "<head>\n"
		 "</head>\n"
		 "<body>\n"
		 "	<h2>Choo choo cachoo</h2>\n"
		 "	<p>\n"
		 "		MySQL makes me tired.\n"
		 "	</p>	\n"
		 "	<h2>Choo choo cachoo</h2>\n"
		 "	<p>\n"
		 "		MySQL makes me giddy.\n"
		 "	</p>	\n"
		 "	<h2>Choo choo cachoo</h2>\n"
		 "	<p>\n"
		 "		MySQL makes me ecstatic.\n"
		 "	</p>	\n"
		 "	<h2>Choo choo cachoo</h2>\n"
		 "	<p>\n"
		 "		MySQL makes me a bother.\n"
		 "	</p>	\n"
		 "</body>\n"
		 "</html>\n"
	},

	{
		.values = {
			{ TEXT_KEY( "baltimore" ), TEXT_VALUE( "City of Dreams" )  },
			{ TEXT_KEY( "winston-salem" ), BLOB_VALUE( "City of Chicken and Guns" )    },

			//This is much easier to see
			//{ START_TABLE( "artillery" ) }
			{ TEXT_KEY( "artillery" )       , TABLE_VALUE( )         },
				/*Database records look a lot like this*/
				{ TEXT_KEY( "arbutus" )       , TABLE_VALUE( )         },
					{ TEXT_KEY( "city" ), BLOB_VALUE( "San Francisco, CA" ) },
					{ TEXT_KEY( "population" ), INT_VALUE( 332420 ) },
					{ TEXT_KEY( "favorite spots" ), TEXT_VALUE( "China, Dacao, Maharantz" ) },
					{ TRM() },
				{ TEXT_KEY( "dreamer" )       , TABLE_VALUE( )         },
					{ TEXT_KEY( "city" ), BLOB_VALUE( "Winston-Salem, NC" ) },
					{ TEXT_KEY( "population" ), INT_VALUE( 33242 ) },
					{ TEXT_KEY( "favorite spots" ), TEXT_VALUE( "China, Dacao, Maharantz" ) },
					{ TRM() },
				{ TEXT_KEY( "daca" )       , TABLE_VALUE( )         },
					{ TEXT_KEY( "city" ), BLOB_VALUE( "Modesto, CA" ) },
					{ TEXT_KEY( "population" ), INT_VALUE( 777000 ) },
					{ TEXT_KEY( "favorite spots" ), TEXT_VALUE( "Get out now..." ) },
					{ TRM() },
				{ TEXT_KEY( "real-global" )       , TABLE_VALUE( )         },
					{ TEXT_KEY( "city" ), BLOB_VALUE( "New York, NY" ) },
					{ TEXT_KEY( "population" ), INT_VALUE( 8312101 ) },
					{ TEXT_KEY( "favorite spots" ), TEXT_VALUE( "Brooklyn, Manhattan" ) },
					{ TRM() },
				{ TRM() },

			{ TEXT_KEY( "ashor" )  , TABLE_VALUE( )         },
				{ INT_KEY( 7043 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog." )  },
				{ INT_KEY( 7002 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog another time." )  },
				{ INT_KEY( 7003 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the 3rd time." )  },
				{ INT_KEY( 7004 )    , USR_VALUE ( NULL ) },
				{ INT_KEY( 7008 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog again." )  },
				{ INT_KEY( 7009 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the last time." )  },
				{ TRM() },
			{ LKV_LAST } 
		},

		.renSrc =
		 "<html>\n"
		 "<head>\n"
		 "</head>\n"
		 "<body>\n"
		 "{{# artillery }}\n"
		 "	<h2>{{$ key }}</h2>\n"
		 "	<p>\n"
		 "		{{$ val }}\n"
		 "	</p>	\n"
		 "{{/ artillery }}\n"
		 "</body>\n"
		 "</html>\n"
		,

		.renCmp = 
		 "<html>\n"
		 "<head>\n"
		 "</head>\n"
		 "<body>\n"
		 "	<h2>Choo choo cachoo</h2>\n"
		 "	<p>\n"
		 "		MySQL makes me tired.\n"
		 "	</p>	\n"
		 "	<h2>Choo choo cachoo</h2>\n"
		 "	<p>\n"
		 "		MySQL makes me giddy.\n"
		 "	</p>	\n"
		 "	<h2>Choo choo cachoo</h2>\n"
		 "	<p>\n"
		 "		MySQL makes me ecstatic.\n"
		 "	</p>	\n"
		 "	<h2>Choo choo cachoo</h2>\n"
		 "	<p>\n"
		 "		MySQL makes me a bother.\n"
		 "	</p>	\n"
		 "</body>\n"
		 "</html>\n"
	},


	{ .sentinel = 1 }
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
		lt_dump( t ); 
	
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

	return 0;
}
