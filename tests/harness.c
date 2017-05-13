#include "../single.h"
#include "harness.h"
#include "harness-test-data.c"

#define FILES_H 
#define MEM_H 
#define OPT_H
#define PARSELY_H 
#define RAND_H 
#define RENDER_H 
#define SOCKET_H 
#define SQROOGE_H 
#define TIMER_H 
#define UTIL_H 

/*Pretty print left side values*/
#define LPRINTF( ... ) \
	memset( buf, 0, sizeof( buf ) ) ; \
	snprintf( buf, 2048, __VA_ARGS__ ) ; \
	fprintf( stderr, "%-60s", buf )

/*Pretty print right side values*/
#define RPRINTF( ... ) \
	memset( buf, 0, sizeof( buf ) ) ; \
	snprintf( buf, 2048, __VA_ARGS__ ) ; \
	fprintf( stderr, "%10s\n", buf )

/*Pretty print headers for tests */
#define HPRINTF( ... ) \
	memset( buf, 0, sizeof( buf ) ) ; \
	snprintf( buf, 2048, __VA_ARGS__ ) ; \
	fprintf( stderr, "%s\n====================\n", buf )

/*Pretty print error messages*/
#define EPRINTF( ... ) \
	memset( buf, 0, sizeof( buf ) ) ; \
	snprintf( buf, 2048, __VA_ARGS__ ) ; \
	fprintf( stderr, "ERROR: %s\n", buf )

/*Test definition so I don't have to remember the declaration for each new test*/
#define TEST( m ) \
	int m##_main ( int argc, char *argv[], uint8_t *TEST_BYTES, int TEST_BYTES_LENGTH )

/*Create the argument table for tests*/
#define ARG( m, d ) \
	{ #m, "--" #m, m##_main, d }  

/*Extension of all test files, .txt should work on windows with no issue*/
#define TEXTEXT \
	"_test.txt"

#define TESTDIR \
	"tests/files/"

#define PROGRAM_NAME \
	"single-harness"

/*Maximum test count*/
#define TEST_COUNT \
	5

/*Short way to define lots of options*/
#define DESC( a ) \
	{ NULL, "--" a, "Run the " a " tests." }  

/*Include all tests here through use of defines*/
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


/*Buffer for formatted text*/
static char buf[ 2048 ];

/*Buffer for filenames*/
static char filename[ 2048 ];

/*Stauts of test calls*/
static int status;

typedef struct 
{
	const char *name;
	const char *arg;
	int (*test)(int , char *[], uint8_t *, int );
	int file;
} TestArg;

 
TestArg tests[] =
{
#ifndef BUFFER_H
	ARG( buff, 0 ),
#endif
#ifndef FILES_H 
	ARG( files, 0 ),
#endif
#ifndef FMT_H
	ARG( fmt, 0 ),
#endif
#ifndef JSON_H 
	ARG( json, 1 ),
#endif
#ifndef MEM_H 
	ARG( mem, 1 ),
#endif
#ifndef OPT_H
	ARG( opt, 0 ),
#endif
#ifndef PARSELY_H 
	ARG( parsely, 1 ),
#endif
#ifndef RAND_H 
	ARG( rand, 0 ),
#endif
#ifndef RENDER_H 
	ARG( render, 1 ),
#endif
#ifndef SOCKET_H 
	ARG( socket, 0 ),
#endif
#ifndef SQROOGE_H 
	ARG( sqrooge, 1 ),
#endif
#ifndef TAB_H
	ARG( tab, 0 ),
#endif
#ifndef TIMER_H 
	ARG( timer, 0 ),
#endif
#ifndef UTIL_H 
	ARG( util, 0 ),
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
	{ "-a", "--all",  "Run all tests" },
	{ "-h", "--help", "Show help" },
	{ .sentinel = 1 }
};


int main (int argc, char *argv[])
{
	TestArg *t = tests;
	(argc < 2) ? opt_usage(opts, "nothing to do.", 0, 0) : opt_eval(opts, argc, argv);
	int all = opt_set( opts, "--all" );

	while ( t->name )
	{
		LPRINTF( "Run tests for suite '%s' ( %s )?", t->name, t->arg );
		
		//Skip if you only asked for certain tests
		if ( !all && !opt_set( opts, t->arg ))
		{
			RPRINTF( "NO" );
			t ++;
			continue;
		}

		RPRINTF( "YES" );
		HPRINTF( t->name ); 

		if ( !t->file )
			//Run non file based tests
			status = t->test ( 1, NULL, NULL, 0 );
		else
		{	
			//If the tests are file based, re-run from here
			for ( int i = 1 ; i < TEST_COUNT; i++  )
			{
				//Define a bunch of crap...
				uint8_t *bd = NULL;
				int bl = 0;	
				struct stat sb;
				char inty[ 5 ] = { 0 };
				int a = 0;

				//Make a number
				snprintf( inty, 4, "%d", i );

				//Create a file name
				memset( filename, 0, sizeof( filename ) );
				snprintf( filename, sizeof( filename ) - 1, "%s%s%s%s", TESTDIR, t->name, inty, TEXTEXT );
				fprintf( stderr, "Assets: %s\n", filename );

				//If the file is not there, kill the test and move on
				if ( stat( filename, &sb ) == -1 )
				{
					EPRINTF( "Test file '%s' not found.  Skipping rest of suite.", filename );
					break;
				}
				else
				{
					//Open the file
					int fd = open ( filename, O_RDONLY );

					//Allocate space and read directly to memory
					if ((bd = malloc( sb.st_size )) == NULL )
					{
						EPRINTF( "Failed to allocate space for file '%s'", filename );
						close( fd );
						break;						
					}

					//Read all into the new filenamefer.
					memset( bd, 0, sb.st_size );
					if (( bl =  read( fd, bd, sb.st_size )) != sb.st_size )
					{
						EPRINTF( "Failed to read all of file '%s' to buffer", filename );
						free( bd );
						close( fd );
						break;						
					}

					//Run the test with the contents
					status = t->test ( 1, NULL, bd, bl );

					//Clean up
					close( fd );
					free( bd );
				}
			}
		}
		t ++;
	}
	

	return 0;
}
