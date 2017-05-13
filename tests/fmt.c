#include "../single.h"
#include "harness.h"

//int fmt_main (int argc, char *argv[]) 
TEST( fmt )
{
	const char *str = "Yellow weasels in grey torniquettes.";
	int  num = 234123;
	long lon = 2342323423423;
	float flt = 23.2343;
	void *ptr = &str;
	const int bloblen = 20;
	uint8_t blob[ 2048 ];
	memset( blob, 255, 2048 );

	//Print string using name of variable as key
	nsprintf( str );

	//Print number using name of variable as key
	niprintf( num );

	//Print long using name of variable as key
	nlprintf( lon );

	//Print float using name of variable as key
	nfprintf( flt );

	//Print pointer using name of variable as key
	npprintf( ptr );

	//Print binary data (in hex) using name of variable as key
	//nbprintf( blob, bloblen );

	//Print strings
	stprintf( "string", str );

	//Print numbers
	nmprintf( "number", num );

	//Print large numbers 
	ldprintf( "long number", lon );

	//Print float / double 
	fdprintf( "float", flt );

	//Print pointers 
	spprintf( "pointer", ptr );

	//Print binary data (in hex)
	bdprintf( "binary data", blob, bloblen );

	//Finally, print the size of things
	szprintf( str );
	szprintf( num );
	szprintf( lon );
	szprintf( flt );
	szprintf( ptr );
	szprintf( blob );

	return 0;
}
