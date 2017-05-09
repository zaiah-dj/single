#include "../single.h"
#include "harness.h"
#include "harness-test-data.c"

#define FILES_H 
#define FMT_H
#define MEM_H 
#define OPT_H
#define PARSELY_H 
#define RAND_H 
#define RENDER_H 
#define SOCKET_H 
#define SQROOGE_H 
#define TAB_H
#define TIMER_H 
#define UTIL_H 

#define TEST( m ) \
	int m##_main ( int argc, char *argv[] )

#define TEXTEXT \
	"_test.txt"

#define PROGRAM_NAME \
	"single-harness"

#define TEST_COUNT \
	5

#define DESC( a ) \
	{ NULL, "--" a, "Run the " a " tests." }  

#ifndef BUFFER_H
 #include "buffer.c"
#endif
#ifndef FILES_H 
 #include "files.c"
#endif
#ifndef FMT_H
 #include "fmt.c"
#endif
#ifndef JSON_H 
 #include "json.c"
#endif
#ifndef MEM_H 
 #include "mem.c"
#endif
#ifndef OPT_H
 #include "opt.c"
#endif
#ifndef PARSELY_H 
 #include "parsely.c"
#endif
#ifndef RAND_H 
 #include "random.c"
#endif
#ifndef RENDER_H 
 #include "render.c"
#endif
#ifndef SOCKET_H 
 #include "socket.c"
#endif
#ifndef SQROOGE_H 
 #include "sqrooge.c"
#endif
#ifndef TAB_H
 #include "tab.c"
#endif
#ifndef TIMER_H 
 #include "timer.c"
#endif
#ifndef UTIL_H 
 #include "util.c"
#endif



typedef struct 
{
	const char *name;
	int (*test)(int , char *[]);
	int file;
} TestArg;

 
TestArg tests[] =
{
#ifndef BUFFER_H
	{ "buff", buff_main },
#endif
#ifndef FILES_H 
	{ "files", files_main },
#endif
#ifndef FMT_H
	{ "fmt", fmt_main },
#endif
#ifndef JSON_H 
	{ "json", json_main, 1 },
#endif
#ifndef MEM_H 
	{ "mem", mem_main, 1 },
#endif
#ifndef OPT_H
	{ "opt", opt_main },
#endif
#ifndef PARSELY_H 
	{ "parsely", parsely_main, 1 },
#endif
#ifndef RAND_H 
	{ "rand", rand_main },
#endif
#ifndef RENDER_H 
	{ "render", render_main, 1 },
#endif
#ifndef SOCKET_H 
	{ "socket", socket_main },
#endif
#ifndef SQROOGE_H 
	{ "sqrooge", sqrooge_main, 1 },
#endif
#ifndef TAB_H
	{ "tab", tab_main, 1 },
#endif
#ifndef TIMER_H 
	{ "timer", timer_main },
#endif
#ifndef UTIL_H 
	{ "util", util_main },
#endif
	{ NULL }
};



Option opts[] = 
{
#ifndef BUFFER_H
	DESC( "buffer" ),
#endif
#ifndef FILES_H 
	DESC( "files" ),
#endif
#ifndef FMT_H
	DESC( "fmt" ),
#endif
#ifndef JSON_H 
	DESC( "json" ),
#endif
#ifndef MEM_H 
	DESC( "mem" ),
#endif
#ifndef OPT_H
	DESC( "opt" ),
#endif
#ifndef PARSELY_H 
	DESC( "parsely" ),
#endif
#ifndef RAND_H 
	DESC( "rand" ),
#endif
#ifndef RENDER_H 
	DESC( "render" ),
#endif
#ifndef SOCKET_H 
	DESC( "socket" ),
#endif
#ifndef SQROOGE_H 
	DESC( "sqrooge" ),
#endif
#ifndef TAB_H
	DESC( "tab" ),
#endif
#ifndef TIMER_H 
	DESC( "timer" ),
#endif
#ifndef UTIL_H 
	DESC( "util" ),
#endif
	{ .sentinel = 1 }
};


int main (int argc, char *argv[])
{
	TestArg *t = tests;
	(argc < 2) ? opt_usage(opts, "nothing to do.", 0, 0) : opt_eval(opts, argc, argv);

	//Cycle through each test
	while ( t->name )
	{
		fprintf ( stderr, "Running tests for suite '%s'\n", t->name );

		if ( !t->file )
			;//t->test ( 1, NULL );
		else
		{	
			//If the tests are file based, re-run from here
			for ( int i = 0 ; i < TEST_COUNT; i++  )
			{
				//Define a bunch of crap...
				struct stat sb;
				char file[ 2048 ] = { 0 };
				char inty[ 5 ] = { 0 };
				int a = 0;

				//Make a number
				snprintf( inty, 4, "%d", i );

				//Create a file name
				memcpy( &file[ a ], t->name, strlen( t->name ) );
				a += strlen( t->name );
				memcpy( &file[ a ], inty, strlen( inty ));
				a += strlen( inty );
				memcpy( &file[ a ], TEXTEXT, strlen( TEXTEXT ) );
				a += strlen( TEXTEXT );
				file[ a ] = '\0';
				fprintf( stderr, "Assets: %s\n", file );

				//If the file is not there, kill the test and move on
				if ( stat( file, &sb ) != -1 )
					t->test ( 1, NULL );
				else
				{
					fprintf( stderr, "Test file '%s' not found.  Skipping rest of suite.\n", file );
					break;
				}
			}
		}
		t ++;
	}

	return 0;
}
