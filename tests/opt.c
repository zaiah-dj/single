#define SHORT(a, b, c) \
	{ "-" a, "--" b, "Run the " b " tests.", c }  

#define TESTPROGNAME "./a "

#define TESTAOPT(a) \
	""
	
Option nullopt[] = {
	{ .sentinel = 1 }
};

Option a_opt[] = {
	SHORT( "d", "doublehyphen", 0 ),
	SHORT( "s", "string", 0 ),
	SHORT( "n", "numeric", 0 ),
	SHORT( "y", "hyphenated-arg", 0 ),
	SHORT( "s", "s", 0 ), //one character flag...
	{ .sentinel = 1 }
};

struct TestOpts {
	int expected;
	const char *argstr, *desc;
	Option *opt;
} options[] = {
	{ ERR_NONE,                "./a", "No arguments", a_opt }
 ,{ ERR_OPT_EXPECTED_ANY,    "./a --b", "Flag with expected argument, but no argument", a_opt }
 ,{ ERR_OPT_UNEXPECTED_FLAG, "./a --b --fish", "Flag with expected argument, but got flag instead", a_opt }
 ,{ ERR_NONE,                "./a --b -- --fishy", "Flag with expected argument, but argument starts with -- " , a_opt }
 ,{ ERR_OPT_EXPECTED_NUMBER, "./a --string 1", "Flag with expected argument, but argument is of wrong type ( type == number )", a_opt }
 ,{ ERR_OPT_EXPECTED_STRING, "./a --numeric str", "Flag with expected argument, but argument is of wrong type ( type == string)", a_opt }
 ,{ ERR_NONE,                "./a --string string", "Flag with expected argument, but argument exists and is right", a_opt }
 ,{ ERR_NONE,                "./a --string abc --numeric --something else", "Multiple flags with expected arguments, but no argument exists for one of the flags.", a_opt }
 ,{ ERR_NONE,                "./a --string abc --numeric 1 --something else", "Flag with expected argument, but argument exists and is right", a_opt }
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
		LPRINTF( "Running test: %s", ot->desc );

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
		if ( !opt_eval( ot->opt, fakeArgn, fakeArgv ) ) {
			//Some things need to fail...
			//fprintf( stderr, "Error: %s\n", ot->opt->errmsg );
		}

		//Opt set and get need to run too
		//fprintf( stderr, "moving to next set...\n" );
		RPRINTF( "No success whatsoever..." );
		ot++;
	}

	return 1;
}
