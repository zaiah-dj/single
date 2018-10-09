#include "../single.h"
#include "harness.h"

/* -------------------------------------- *
 * BUGS
 * ----
 *
 * - Keys that don't exist don't seem to 
 * work (i.e. when not there, the rest of
 * the render src does not show)
 *
 * - Tables still start at the wrong index.
 *
 * - Nested tables still don't work...
 *
 *
 * NOTES
 * -----
 * - Certain keys return a carriage return at the end of the sequence.
 *   Why?
 * 
 * - I was able to retrieve the table reference for nested tables, not 
 *   sure if it's right, but it's there.
 * 
 * - Single retrieval keys seem to work.
 *   
 * - 
 * 
 * 
 * 
 * TODO:
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
 #define SPACE_TEST "sister cities"
#else
 #define SPACE_TEST "sister_cities"
#endif


//All tests go here
#define TABLE_DOUBLE
#if 0
#define TABLE_NONE
#define TABLE_SINGLE
#define TABLE_KEYVALUE
#define TABLE_KEYVALUEEMB
#define TABLE_MULTI
#endif

typedef struct 
{
	const char *name  ,
						 *desc  ,
						 *renSrc,
						 *renCmp;
	LiteKv     *values;
	int    sentinel   ;
} RenderTest;



/*
All of these are defined seperately, becuase many will be re-used for the tests.
*/
LiteKv NoTable[] = {
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
};


#if 0
//
LiteKv ST[] = {
	{ TEXT_KEY( "ashor" )  , TABLE_VALUE( )         },
		{ INT_KEY( 7043 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog." )  },
		{ INT_KEY( 7002 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog another time." )  },
		{ TRM() },
	{ LKV_LAST } 
};


LiteKv MT[] = {
	{ INT_KEY( 1 )    , TEXT_VALUE( "Wash the dog." )  },
	{ TEXT_KEY( "ashor" )  , TABLE_VALUE( )         },
		{ INT_KEY( 7043 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog." )  },
		{ INT_KEY( 7002 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog another time." )  },
		{ TRM() },
	{ TEXT_KEY( "artillery" )       , TABLE_VALUE( )         },
		/*Database records look a lot like this*/
		{ INT_KEY( 0 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "val" ), BLOB_VALUE( "MySQL makes me tired." ) },
			{ TEXT_KEY( "rec" ), TEXT_VALUE( "Choo choo cachoo." ) },
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
#endif


LiteKv SingleTable[] = {
	{ TEXT_KEY( "ashor" )  , TABLE_VALUE( )         },
		{ INT_KEY( 7043 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog." )  },
		{ INT_KEY( 7002 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog another time." )  },
		{ INT_KEY( 7003 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the 3rd time." )  },
		{ INT_KEY( 7004 )    , USR_VALUE ( NULL ) },
		{ INT_KEY( 7008 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog again." )  },
		{ INT_KEY( 7009 )    , TEXT_VALUE( "The quick brown fox jumps over the lazy dog for the last time." )  },
		{ TRM() },

	{ TEXT_KEY( "artillery" )       , TABLE_VALUE( )         },
		/*Database records look a lot like this*/
		{ INT_KEY( 0 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "val" ), TEXT_VALUE( "MySQL makes me a bother." ) },
			{ TEXT_KEY( "rec" ), TEXT_VALUE( "Choo choo cachoo" ) },
			{ TRM() },
		{ INT_KEY( 1 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "val" ), TEXT_VALUE( "MySQL makes me ecstatic." ) },
			{ TEXT_KEY( "rec" ), TEXT_VALUE( "Choo choo cachoo" ) },
			{ TRM() },
		{ INT_KEY( 2 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "val" ), TEXT_VALUE( "MySQL makes me giddy." ) },
			{ TEXT_KEY( "rec" ), TEXT_VALUE( "Choo choo cachoo" ) },
			{ TRM() },
		{ INT_KEY( 3 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "val" ), TEXT_VALUE( "MySQL makes me tired." ) },
			{ TEXT_KEY( "rec" ), TEXT_VALUE( "Choo choo cachoo" ) },
			{ TRM() },
		{ TRM() },

	{ LKV_LAST } 
};



LiteKv DoubleTableAlpha[] = {
	{ TEXT_KEY( "cities" )       , TABLE_VALUE( )         },
		/*Database records look a lot like this*/
		{ INT_KEY( 0 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "city" ), BLOB_VALUE( "San Francisco" ) },
			{ TEXT_KEY( "parent_state" ), TEXT_VALUE( "CA" ) },
			{ TEXT_KEY( "desc" ), BLOB_VALUE( "There are so many things to see and do in this wonderful town.  Like talk to a billionaire startup founder or super-educated University of Berkeley professors." ) },
			{ TEXT_KEY( "metadata" ), TABLE_VALUE( )         },
				//Pay attention to this, I'd like to embed uint8_t data here (I think Lua can handle this)
				{ TEXT_KEY( "claim_to_fame" ), TEXT_VALUE( "The Real Silicon Valley" ) },
				{ TEXT_KEY( "skyline" ), BLOB_VALUE( "CA" ) },
				{ TEXT_KEY( "population" ), INT_VALUE( 870887 ) },
				{ TRM() },
			{ TRM() },

		{ INT_KEY( 1 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "city" ), BLOB_VALUE( "New York" ) },
			{ TEXT_KEY( "parent_state" ), TEXT_VALUE( "NY" ) },
			{ TEXT_KEY( "desc" ), BLOB_VALUE( "New York City is one of the most well-known destinations on earth and home to over 8 million residents." ) },
			{ TEXT_KEY( "metadata" ), TABLE_VALUE( )         },
				{ TEXT_KEY( "claim_to_fame" ), TEXT_VALUE( "The Greatest City on Earth" ) },
				{ TEXT_KEY( "skyline" ), BLOB_VALUE( "CA" ) },
				{ TEXT_KEY( "population" ), INT_VALUE( 8750000 ) },
				{ TRM() },
			{ TRM() },

		{ INT_KEY( 2 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "city" ), BLOB_VALUE( "Raleigh" ) },
			{ TEXT_KEY( "parent_state" ), TEXT_VALUE( "NC" ) },
			{ TEXT_KEY( "desc" ), BLOB_VALUE( "Otherwise known as the Oak City, around 600,000 residents call Raleigh home." ) },
			{ TEXT_KEY( "metadata" ), TABLE_VALUE( )         },
				{ TEXT_KEY( "claim_to_fame" ), TEXT_VALUE( "Silicon Valley of the South" ) },
				{ TEXT_KEY( "skyline" ), BLOB_VALUE( "CA" ) },
				{ TEXT_KEY( "population" ), INT_VALUE( 350001 ) },
				{ TRM() },
			{ TRM() },
		{ TRM() },
	{ LKV_LAST } 
};



LiteKv DoubleTableNumeric[] = {
	{ TEXT_KEY( "cities" )       , TABLE_VALUE( )         },
		/*Database records look a lot like this*/
		{ INT_KEY( 0 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "city" ), BLOB_VALUE( "San Francisco, CA" ) },
			{ TEXT_KEY( "desc" ), TEXT_VALUE( "It reeks of weed and opportunity. You know you want it..." ) },
			{ TEXT_KEY( "population" ), INT_VALUE( 332420 ) },
			{ TEXT_KEY( SPACE_TEST ), TABLE_VALUE( )         },
				{ INT_KEY( 0 ), TEXT_VALUE( "Sydney, Austrailia" ) },
				{ INT_KEY( 1 ), TEXT_VALUE( "Beijing, China" ) },
				{ INT_KEY( 2 ), TEXT_VALUE( "Perth, Australia" ) },
				{ INT_KEY( 3 ), TEXT_VALUE( "Johannesburg, South Africa" ) },
				{ TRM() },
			{ TRM() },

		{ INT_KEY( 1 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "city" ), BLOB_VALUE( "Durham, NC" ) },
			{ TEXT_KEY( "population" ), INT_VALUE( 33242 ) },
			{ TEXT_KEY( SPACE_TEST ), TABLE_VALUE( )         },
				{ INT_KEY( 0 ), TEXT_VALUE( "Arusha, Tanzania" ) },
				{ INT_KEY( 1 ), TEXT_VALUE( "Durham, United Kingdom" ) },
				{ INT_KEY( 2 ), TEXT_VALUE( "Kostroma, Russia" ) },
				{ INT_KEY( 3 ), TEXT_VALUE( "Toyama, Japan" ) },
				{ INT_KEY( 4 ), TEXT_VALUE( "Zhuzhou, Hunan Province, China" ) },
				{ TRM() },
			{ TRM() },

		{ INT_KEY( 2 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "city" ), BLOB_VALUE( "Tampa, FL" ) },
			{ TEXT_KEY( "desc" ), TEXT_VALUE( "It's home..." ) },
			{ TEXT_KEY( "population" ), INT_VALUE( 777000 ) },
			{ TEXT_KEY( SPACE_TEST ), TABLE_VALUE( )         },
				{ INT_KEY( 0 ), TEXT_VALUE( "Agrigento, Sicily" ) },
				{ INT_KEY( 1 ), TEXT_VALUE( "Ashdod South, Isreal" ) },
				{ INT_KEY( 2 ), TEXT_VALUE( "Barranquilla, Colombia" ) },
				{ INT_KEY( 3 ), TEXT_VALUE( "Boca del Rio, Veracruz" ) },
				{ TRM() },
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
};


LiteKv MultiLevelTable[] = {
	{ TEXT_KEY( "cities" )       , TABLE_VALUE( )         },
		/*Database records look a lot like this*/
		{ INT_KEY( 0 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "city" ), BLOB_VALUE( "San Francisco" ) },
			{ TEXT_KEY( "parent_state" ), TEXT_VALUE( "CA" ) },
			{ TEXT_KEY( "metadata" ), TABLE_VALUE( )         },
				//Pay attention to this, I'd like to embed uint8_t data here (I think Lua can handle this)
				{ TEXT_KEY( "skyline" ), BLOB_VALUE( "CA" ) },
				{ TEXT_KEY( "population" ), INT_VALUE( 870887 ) },
				{ TEXT_KEY( "demographics" ), TABLE_VALUE( )         },
#if 0
					{ TEXT_KEY( "Black" ),    INT_VALUE( 5.5 ) },
					{ TEXT_KEY( "White" ),    INT_VALUE( 40.5 ) },
					{ TEXT_KEY( "Asian" ),    INT_VALUE( 35.4 ) },
					{ TEXT_KEY( "Hispanic" ), INT_VALUE( 15.2 ) },
#else
					{ TEXT_KEY( "Black" ),    INT_VALUE( 5 ) },
					{ TEXT_KEY( "White" ),    INT_VALUE( 40 ) },
					{ TEXT_KEY( "Asian" ),    INT_VALUE( 35 ) },
					{ TEXT_KEY( "Hispanic" ), INT_VALUE( 15 ) },
#endif
					{ TEXT_KEY( "Other" ),    INT_VALUE( 20 ) },
					{ TRM() },
				{ TRM() },
			{ TRM() },

		{ INT_KEY( 1 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "city" ), BLOB_VALUE( "New York" ) },
			{ TEXT_KEY( "parent_state" ), TEXT_VALUE( "NY" ) },
			{ TEXT_KEY( "metadata" ), TABLE_VALUE( )         },
				{ TEXT_KEY( "skyline" ), BLOB_VALUE( "CA" ) },
				{ TEXT_KEY( "population" ), INT_VALUE( 19750000 ) },
				{ TEXT_KEY( "demographics" ), TABLE_VALUE( )         },
#if 0
					{ TEXT_KEY( "Black" ),    INT_VALUE( 17.7 ) },
					{ TEXT_KEY( "White" ),    INT_VALUE( 55.8 ) },
					{ TEXT_KEY( "Asian" ),    INT_VALUE( 8.9 ) },
#else
					{ TEXT_KEY( "Black" ),    INT_VALUE( 17 ) },
					{ TEXT_KEY( "White" ),    INT_VALUE( 55 ) },
					{ TEXT_KEY( "Asian" ),    INT_VALUE( 8 ) },
#endif
					{ TEXT_KEY( "Hispanic" ), INT_VALUE( 19 ) },
					{ TEXT_KEY( "Other" ),    INT_VALUE( 13 ) },
					{ TRM() },
				{ TRM() },
			{ TRM() },

		{ INT_KEY( 2 )       , TABLE_VALUE( )         },
			{ TEXT_KEY( "city" ), BLOB_VALUE( "Raleigh" ) },
			{ TEXT_KEY( "parent_state" ), TEXT_VALUE( "NC" ) },
			{ TEXT_KEY( "metadata" ), TABLE_VALUE( )         },
				{ TEXT_KEY( "skyline" ), BLOB_VALUE( "CA" ) },
				{ TEXT_KEY( "population" ), INT_VALUE( 870887 ) },
				{ TEXT_KEY( "demographics" ), TABLE_VALUE( )         },
#if 0
					{ TEXT_KEY( "Black" ),    INT_VALUE( 28.4 ) },
					{ TEXT_KEY( "White" ),    INT_VALUE( 57.74 ) },
					{ TEXT_KEY( "Asian" ),    INT_VALUE( 4.69 ) },
					{ TEXT_KEY( "Hispanic" ), INT_VALUE( 11.81 ) },
#else
					{ TEXT_KEY( "Black" ),    INT_VALUE( 28 ) },
					{ TEXT_KEY( "White" ),    INT_VALUE( 57 ) },
					{ TEXT_KEY( "Asian" ),    INT_VALUE( 4 ) },
					{ TEXT_KEY( "Hispanic" ), INT_VALUE( 11 ) },
#endif
					{ TEXT_KEY( "Other" ),    INT_VALUE( 9 ) },
					{ TRM() },
				{ TRM() },
			{ TRM() },
		{ TRM() },
	{ LKV_LAST } 
};




RenderTest r[] = 
{
	//These should be pretty easy to read:
	//.name   = Name of the test
	//.desc   = A quick description of the test
	//.renSrc = the input that the test will use for find and replace
	//.renCmp = the constant to compare against to make sure that rendering worked
	//.values = the Table to use for values (these tests do not test any parsing)
#ifdef TABLE_NONE
	{
		.name = "TABLE_NONE",
		.desc = "Template values with no tables.",
		.values = NoTable, 
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
#endif

#ifdef TABLE_SINGLE
	//one table
	{
		.name = "TABLE_SINGLE",
		.desc = "one level table",
		.values = SingleTable, 

		.renSrc =
		 "<html>\n"
		 "<head>\n"
		 "</head>\n"
		 "<body>\n"
		 "{{# artillery }}"
		 "	<h2>{{ .rec }}</h2>\n"
		 "	<p>\n"
		 "		{{ .val }}\n"
		 "	</p>\n"
		 "{{/ artillery }}"
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
		 "	</p>\n"
		 "	<h2>Choo choo cachoo</h2>\n"
		 "	<p>\n"
		 "		MySQL makes me giddy.\n"
		 "	</p>\n"
		 "	<h2>Choo choo cachoo</h2>\n"
		 "	<p>\n"
		 "		MySQL makes me ecstatic.\n"
		 "	</p>\n"
		 "	<h2>Choo choo cachoo</h2>\n"
		 "	<p>\n"
		 "		MySQL makes me a bother.\n"
		 "	</p>\n"
		 "</body>\n"
		 "</html>\n"
	},
#endif

#ifdef TABLE_DOUBLE
	{
		.name = "TABLE_DOUBLE",
		.desc = "two level table | key value test",
		.values = DoubleTableAlpha,

		//Notice the cities.metadata loop block.  Teset for short and long keys...
		.renSrc =
		 "<html>\n"
		 "<head>\n"
		 "</head>\n"
		 "<body>\n"
		 "{{# cities }}\n"
		 "	<h2>{{ .city }}</h2>\n"
		 "	<p>\n"
		 "    {{ .city }}, {{ .parent_state }} is a city full of {{ .metadata.population }} people.\n" 
		 "	</p>\n"
		 "	<p>\n"
		 "    {{ .desc }}\n"
		 "	</p>\n"
		 "	<table>\n"
		 "	<thead>\n"
		 "		<th>Skyline</th>\n"
		 "		<th>Population</th>\n"
		 "	</thead>\n"
		 "	<tbody>\n"
		 "	{{# .metadata }}\n"
		 "    <tr>\n"
		 "			<td>Population: {{ .population }}</td>\n"  
		 "			<td>Claim to Fame: {{ cities.metadata.claim_to_fame }}</td>\n"
		 "    </tr>\n"
		 "	{{/ .metadata }}\n"
		 "	</tbody>\n"
		 "	</table>\n"
		 "{{/ cities }}\n"
		 "</body>\n"
		 "</html>\n"
		,

		.renCmp = 
		 "<html>\n"
		 "<head>\n"
		 "</head>\n"
		 "<body>\n"
		 "\n"
		 "	<h2>San Francisco</h2>\n"
		 "	<p>\n"
		 "    San Francisco, CA is a city full of 870887 people.\n" 
		 "	</p>\n"
		 "	<p>\n"
		 "  	There are so many things to see and do in this wonderful town.  Like talk to a billionaire startup founder or super-educated University of Berkeley professors.\n" 
		 "	</p>\n"
		 "	<table>\n"
		 "	<thead>\n"
		 "		<th>Skyline</th>\n"
		 "		<th>Population</th>\n"
		 "	</thead>\n"
		 "	<tbody>\n"
		 "    <tr>\n"
		 "			<td>Population: 870887</td>\n"  
		 "			<td>Claim to Fame: The Real Silicon Valley</td>\n"
		 "    </tr>\n"
		 "	</tbody>\n"
		 "	</table>\n"
		 "\n"
		 "	<h2>New York</h2>\n"
		 "	<p>\n"
		 "    New York is a city full of 8750000 people.\n" 
		 "	</p>\n"
		 "	<p>\n"
		 "    New York, NY is one of the most well-known destinations on earth and home to over 8 million residents.\n"
		 "	</p>\n"
		 "	<table>\n"
		 "	<thead>\n"
		 "		<th>Skyline</th>\n"
		 "		<th>Population</th>\n"
		 "	</thead>\n"
		 "	<tbody>\n"
		 "    <tr>\n"
		 "			<td>Population: 19750000</td>\n"  
		 "			<td>Claim to Fame: The Greatest City on Earth</td>\n"
		 "    </tr>\n"
		 "	</tbody>\n"
		 "	</table>\n"
		 "\n"
		 "	<h2>Raleigh</h2>\n"
		 "	<p>\n"
		 "    Raleigh, NC is a city full of 350001 people.\n" 
		 "	</p>\n"
		 "	<table>\n"
		 "	<thead>\n"
		 "		<th>Skyline</th>\n"
		 "		<th>Population</th>\n"
		 "	</thead>\n"
		 "	<tbody>\n"
		 "    <tr>\n"
		 "			<td>350001</td>\n"  
		 "			<td>Silicon Valley of the South</td>\n"
		 "    </tr>\n"
		 "	</tbody>\n"
		 "	</table>\n"
		 "</body>\n"
		 "</html>\n"
	},
#endif

#ifdef TABLE_MULTI
	{
		.name = "two level table",
		.desc = "two level table | key value test",
		.values = DoubleTableNumeric,

		.renSrc =
		 "<html>\n"
		 "<head>\n"
		 "</head>\n"
		 "<body>\n"
		 "{{# cities }}\n"
		 "	<h2>{{ .city }}</h2>\n"
		 "	<p>\n"
		 "    {{ .city }} is a city full of {{ .population }} people.\n" 
		 "	</p>\n"
		 "	<p>\n"
		 "    {{ .desc }}\n"
		 "	</p>\n"
		 "	<ul>\n"
		 "	{{# cities." SPACE_TEST " }}\n"
		 "		<li>{{ $value }}</li>\n"  /*Notice that this is the only way to do this*/
		 "	{{/ cities." SPACE_TEST " }}\n"
		 "	</ul>\n"
		 "{{/ cities }}\n"
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

	//multi-level tables
	{
		.name = "key and value",
		.desc = "key and value",
		.values = MultiLevelTable,

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
#endif

#ifdef TABLE_KEYVALUE
	//key and value vs actual values
	{
		.name = "key and value",
		.desc = "key and value",
		.values = DoubleTable 
	},
#endif

#ifdef TABLE_KEYVALUEEMB
	//key and value embedded
	{
		.name = "key and value embedded",
		.desc = "key and value embedded",
		.values = MultiLevelTable 
	},
#endif

	//sentinel...
	{ .sentinel = 1 }
};


TEST( render ) {
	int set = 1;
	RenderTest *rt = r;
	char buf[ BUFLEN ];
	int destlen = 0;
	uint8_t *dest = NULL;

	//Let me know what's been defined
	fprintf( stderr, "Running tests:\n" );
	while ( !rt->sentinel ) {
		fprintf( stderr, "%s\n", rt->name );
		rt++;
	}

	//Reset RenderTest pointer and run each test.
	fprintf( stderr, "\n" );
	rt = r;
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
		if ( 0 ) 
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
		if ( 1 ) {
			fprintf( stderr, "$src after render_map(...) looks like:\n" );
			render_dump_mark( &R );
			//getchar();
		}

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

	#if 0
		write( 2, dest, destlen );
		getchar();
	#else
		//Check rendered result against what should be
		if ( memcmp( rt->renCmp, dest, destlen ) == 0 ) {
			fprintf( stderr, "SUCCESS: Final matches expected!\n" );
		}
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
	#endif

		//Free everything
		render_free( &R );
		lt_free( t );
		set ++, rt ++;
		fprintf( stderr, "\n" );
	}

	return 0;
}
