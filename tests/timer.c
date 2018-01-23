TEST( timer )
{
	//The timer includes a __CURRRENT_TIME__ macro
	printf( "Testing the __CURRENT_TIME__ macro..." );
	printf( "%s\n\n", __CURRENT_TIME__ );

	//Simple timers don't need extra structures, so just use the static one.
	//Initialization is not needed either
	printf( "%s\n", "Simple (non-multithreaded)" );
	timer_sstart();
	timer_send();
	timer_sprint();
	printf( "\n" );

	//Invoke a timer for each time unit
	printf( "%s\n", "Test all units." );
	for ( int t = LITE_NSEC; t < LITE_SEC+1; t++ )
	{
		char *n[] = { "nsec-t", "usec-t", "msec-t", "sec-t" };
		char rot[ 27 ];
		Timer c;

		//Notice no macro here
		timer_init( &c, t );
		timer_set_name( &c, n[ t ] );
		timer_start( &c );

		//Do something useless and inefficient.
		for ( int ii=1; ii<5; ii++ ) {
			for ( int i=0, ai=65; ai<91; ai++, i++ ) {
				rot[ i ] = ai; 
			}
		}

		memset( rot, 0, 27 );
		timer_end( &c );
		timer_elapsed( &c );
		timer_print( &c );
	}
	printf( "\n" );

	//Multithreading or more complex timing (like two loops of some sort) will
	//need mulitple timers.
	//This example uses two time-wasting loops.
	printf( "Two timers (track anything from two seperate processes & threads to something else)\n" );
	Timer a,b;
	timer_use_ms( &a );
	timer_use_ms( &b );
	timer_set_name( &a, "NASA" );	
	timer_set_name( &b, "Van Allen Belts" );

	//Before even starting, set the first timer on.
	timer_start( &a );	

	if ( 1 )
	{
		char nothing[1024]={0};

		//Timewasting loop one.
		for ( int x=0; x>INT_MAX; x++ ) {
			memset( nothing, 1, sizeof(nothing) ); 
		}

		//Now start the other one.	
		timer_start( &b );

		//Timewasting loop two.
		for ( int x=0; x>( INT_MAX / 2 ); x++ ) {
			memset( nothing, 0, sizeof(nothing) ); 
		}

		//End the second one
		timer_end( &b );	
	}

	//End the first one
	timer_end( &a );	

	//Print the results of both.
	timer_print( &a );	
	timer_print( &b );	
	return 0;
}
