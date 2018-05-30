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
} options[] = {
	{ ERR_NONE,                "./a", "No arguments, no flags", a_opt }
 ,{ ERR_OPT_EXPECTED_ANY,    "./a --b", "Flag, but no arguments", a_opt }
 ,{ ERR_OPT_UNEXPECTED_FLAG, "./a --b --fish", "Flag, with flag as argument.", a_opt }
 ,{ ERR_NONE,                "./a --b -- --fishy", "Flag with argument starting with -- " , a_opt }
 ,{ ERR_OPT_EXPECTED_STRING_ARG, "./a --string 1", "Argument type mismatch ( type == number )", a_opt }
 ,{ ERR_OPT_EXPECTED_NUMERIC_ARG, "./a --numeric str", "Argument type mismatch ( type == string)", a_opt }
 ,{ ERR_NONE,                "./a --string string", "Argument is correct", a_opt }
 ,{ ERR_NONE,                "./a --string abc --numeric --something else", "Flags, but no argument exists for flag expecting argument.", a_opt }
 ,{ ERR_NONE,                "./a --string abc --numeric 1 --something else", "but argument exists and is right", a_opt }
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
		opt_eval( ot->opt, fakeArgn, fakeArgv );
	
		if ( ot->opt->error != ot->expected ) {
			RPRINTF( "FAILURE, error: %d - '%s'", ot->opt->error, ot->opt->errmsg );
		}
		else {
			RPRINTF( "SUCCESS!" );
		}

		ot++;
	}

	return 1;
}
