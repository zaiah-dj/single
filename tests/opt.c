#define SHORT(a, b, c) \
	{ "-" a, "--" b, "Run the " b " tests.", c }  

#define TESTPROGNAME "./a "

#define TESTAOPT(a) \
	""
	
Option nullopt[] = {
	{ .sentinel = 1 }
};


Option a_opt[] = {
	SHORT( "d", "doublehyphen", 's' ),
	SHORT( "s", "string", 's' ),
	SHORT( "n", "numeric", 'n' ),
	SHORT( "y", "hyphenated-arg", 's' ),
	SHORT( "s", "s", 0 ), //one character flag...
	{ .sentinel = 1 }
};


struct TestOpts {
	int expected;
	const char *argstr, *desc;
	Option *opt;
	int errcode;
	char *exargs[4];
} options[] = {
	{ ERR_NONE,                "./a", "No arguments, no flags", a_opt, 0, { NULL } } 
#if 0
 ,{ ERR_OPT_EXPECTED_ANY,    "./a --b", "Flag, but no arguments", a_opt, 0, { 0 } }
 ,{ ERR_OPT_UNEXPECTED_FLAG, "./a --b --fish", "Flag, with flag as argument.", a_opt, 0, { 0 } }
 ,{ ERR_NONE,                "./a --b -- --fishy", "Flag with argument starting with -- " , a_opt, 0, { 0 } }
, { ERR_OPT_EXPECTED_STRING_ARG, "./a --string 1", "Argument type mismatch ( type == number )", a_opt, 0, { 0 } }
 ,{ ERR_OPT_EXPECTED_NUMERIC_ARG, "./a --numeric str", "Argument type mismatch ( type == string)", a_opt, 0, { 0 } }
#endif
 ,{ ERR_NONE,                "./a --string string", "String argument is correct", a_opt, 0, { "string" } }
 ,{ ERR_NONE, "./a --numeric 22", "Numeric argument is correct", a_opt, 0, { "22" } }
#if 0
 ,{ ERR_NONE,                "./a --string abc --numeric --something else", "Flags, but no argument exists for flag expecting argument.", a_opt, 0, { "abc" } }
 ,{ ERR_NONE,                "./a --string abc --numeric 1 --something else", "but argument exists and is right", a_opt, 0, { "abc", "1" } }
#endif
 ,{ 0, NULL }
};


TEST( opt )
{
	char buf[ 2048 ] = { 0 };
	//const char **ot = opt_tests;
	struct TestOpts *ot = options;

	//Loop through all of these
	while ( ot->argstr ) {
		int status = 0;
		char *fakeArgv[ 100 ] = { NULL };
		int fakeArgn = 0; 
		Mem m;
		memset( &m, 0, sizeof(Mem));
		fprintf( stderr, "Evaluating command-line: %s\n", ot->argstr );
		LPRINTF( "%s", ot->desc );

		//Make a copy of the opt structure
		int ao_sz = sizeof(a_opt); 
		Option *ao = malloc( ao_sz );
		memcpy( ao, ot->opt, ao_sz );

		//Populate fakeArgv for opt
		while ( strwalk( &m, ot->argstr, " " ) ) {
			char *a = fakeArgv[ fakeArgn ] = malloc( m.size + 1 );
			memcpy( a, &ot->argstr[m.pos], m.size );
			a[ m.size ] = '\0';	
			fakeArgn++;
		}

		//What's in this array?  Uncomment and find out :) !	
		//for ( int i=0; i<fakeArgn; i++ ) fprintf( stderr, "'%s'\n", fakeArgv[ i ] );
	
		//Run opt and check for results...	
		//opt_eval( ot->opt, fakeArgn, fakeArgv );
		opt_eval( ao, fakeArgn, fakeArgv );

		//Check for the correct error messages
		if ( ot->opt->error != ot->expected ) {
			//RPRINTF( "FAILURE, error: %d - '%s'", ot->opt->error, ot->opt->errmsg );
			RPRINTF( "FAILURE, error: %d - '%s'", ao->error, ao->errmsg );
			ot++;
			continue;
		}
		else {
			RPRINTF( "SUCCESS!" );
		}

		//Check that args are set and pointers are returned correctly
		Option *od = ao;
		int cmpind=0;
		while ( !od->sentinel ) {
			//First check that args are set
			int stat = 0;
			int os = opt_set( od, od->lng ); 
			LPRINTF( "Was opt %s set? ", od->lng );  
			RPRINTF( os ? "YES" : "NO" );  
			fprintf( stderr, "\n" );

			//Check (out of all args received) that values are available and correct
			fprintf( stderr, "od->type: %c, %d\n", od->type, od->type );
#if 1
			if ( os && od->type > 0 ) {
				LPRINTF( "Is this the correct value for arg %s? ", od->lng );  
				if ( od->type == 's' ) {
					stat = memcmp( opt_get( od, od->lng ).s, ot->exargs[ cmpind ], strlen( ot->exargs[ cmpind ] ) );
					RPRINTF( "%s: %s %s %s", ( stat ) ? "NO" : "YES", ot->exargs[ cmpind ], ( stat ) ? "!=" : "==", opt_get( od, od->lng ).s );
				}
				else if ( od->type == 'n' ) {
					char b[ 10 ];
					snprintf( b, 10, "%d", opt_get( od, od->lng ).n );
					stat = memcmp( ot->exargs[ cmpind ], b, strlen(ot->exargs[cmpind]) );
					RPRINTF( "%s: %s %s %s", ( stat ) ? "NO" : "YES", ot->exargs[cmpind], ( stat ) ? "!=" : "==", b );
				}
				else if ( od->type == 'c' ) {
					stat = ( opt_get( od, od->lng ).c == *(ot->exargs[ cmpind ]) );
					RPRINTF( "%s: %c %s %c", ( !stat ) ? "NO" : "YES", *ot->exargs[cmpind], ( !stat ) ? "!=" : "==", opt_get( od, od->lng ).c );
				}
			}
#endif
			od++;	
		}

		free( ao );
		ot++;
	}

	return 1;
}
