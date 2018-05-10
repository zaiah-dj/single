#include "../single.h"
#include "harness.h"


/*This tests that this works with a large dataset...*/
//int json_main ( int argc, char *argv[] ) 
TEST(json)
{
	//...
	Table t;
	char *file = NULL;
	unsigned char *src = NULL, *ns = NULL;
	int bytes=0, fd = 0;
	struct stat sb;

	//....
	if ( !json_json( &t, TEST_BYTES, TEST_BYTES_LENGTH ) )
		return 0;//perr( 1, "You're going to die...\n" );

	lt_dump( &t );
	json_free( &t );
	free( src ); 
	return 0;
}


