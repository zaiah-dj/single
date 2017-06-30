#include "../single.h"
#include "harness.h"


//int buff_main (int argc, char *argv[]) 
TEST( buff )
{
	Buffer b;

	//Initialize
	if ( !bf_init( &b, NULL, 300 ) ) 
	{
		fprintf( stderr, "Failed to allocate a new buffer.\n" );
		return 0;
	} 

	//Add forward and backward
	for ( int i=0; i<2; i++ ) 
	{
		Parser p = {.words={{";"},{NULL}}};
		pr_prepare( &p );	

		int (*bf_add)(Buffer *, uint8_t *, int) = (!i) ? bf_append : bf_prepend;

		while ( pr_next( &p, (uint8_t *)CSV, sizeof(CSV)) ) 
		{
			if ( !bf_add( &b, (uint8_t *)&CSV[ p.prev ], p.size ) ) {
				fprintf( stderr, "Failed to add to current buffer.\n" );
				return 0;
			}
		}
	}

	//Write the response
	write( 2, b.buffer, b.size );
	bf_free( &b );
	return 0; 
}
