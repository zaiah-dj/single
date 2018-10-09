
struct renderDT { 
	Mark *mark;
	uint8_t *src, *parent;
	int size, psize, skip, times, position;
}//try this again

//Do a map
int render_map ( Render *r, uint8_t *src, int srclen ) {
	//Define stuff
	Parser p   = {.words = {{"{{#"}, {"{{/"}, {"{{^"}, {"{{>"}, {"{{"}, {"}}"}, {NULL}}};
	Mark *raw  = NULL, 
			  *ct  = NULL; 
	int follow = 1;
	pr_prepare( &p );
	int depth=0;

	//Prepare the markers
	if ( !(r->markers = malloc( sizeof( Mark ) )) )
		return 0;
	else {
		memset( r->markers, 0, sizeof(Mark) );
		ct = r->markers;
		ct->depth = depth;
	}

	//Loop through a thing
	for ( int alloc=2, t;  pr_next( &p, src, srclen );  ) {
		//What exactly does the marker say each time
//fprintf(stderr,"===\n"); render_dump_mark( r ); 
#if 0
fprintf(stderr,"prev match===\n"); 
write(2,&src[ p.prev ],srclen - p.prev); 
fprintf(stderr,"recent match===\n"); 
write(2,p.word,p.size); 
getchar();
#endif

		//Copy the last of the stream
		if ( p.word == NULL ) {
			ct->action = R_RAW;
			ct->blob = &src[ p.prev ];
			ct->size = srclen - p.prev; 
			REALLOC( raw, r->markers );
			break;
		}

		//This is the token to choose an action based on
		t = p.word[ p.tokenSize - 1 ];
//fprintf( stderr, "\nMatched token: %c\n", t );

		//Just mark each section (and it's position)
		if ( t == '#' ) {
			//The start of "positive" loops (items that should be true)
			ct->blob = &src[p.prev],
			ct->size = p.size,
			ct->action = R_RAW;
			REALLOC( raw, r->markers );
			//If R_POSLOOP has already been marked and not closed, do something
			ct->action = R_POSLOOP;
			depth++;
		}
		else if ( t == '^' ) {
			//The start of "negative" loops (items that should be false)
			ct->blob = &src[p.prev],
			ct->size = p.size,
			ct->action = R_RAW;
			REALLOC( raw, r->markers );
			ct->action = R_NEGLOOP;
			depth--;
		}
		else if (t == '/') {
			//The end of either a "positive" or "negative" loop
			ct->blob = &src[p.prev],
			ct->size = p.size,
			ct->action = R_RAW;
			REALLOC( raw, r->markers );
			ct->action = R_ENDLOOP;
			depth--;
		}
		else if (t == '{') {
			//The start of a key (any type)
			ct->blob = &src[p.prev],
			ct->size = p.size,
			ct->action = R_RAW;
			REALLOC( raw, r->markers );
			ct->action = R_DIRECT;
		}
		else if (t == '}' /*|| t == '!'*/ ) {
			//Anything within here will always be a table
			ct->blob  = trim( (uint8_t *)&src[ p.prev ], (char *)trimchars, p.size, &ct->size );
			//This is for all inner stubs, same code and all.
			int stat = ( ct->action != R_POSLOOP && ct->action != R_ENDLOOP && ct->action != R_NEGLOOP );
			if ( *ct->blob == '.' && stat )
				ct->action = R_STUB;
			else { 
				ct->index = lt_get_long_i( r->srctable, ct->blob, ct->size );
				ct->type  = lt_vta( r->srctable, ct->index );
				if ((ct->type == LITE_TBL) && (ct->action == R_POSLOOP || ct->action == R_NEGLOOP)) {
					ct->parent = ct->blob; 
					ct->psize = ct->size;	
				}
			}
			REALLOC( raw, r->markers );
		}
	}

	return 1;
}

int render_render ( Render *r ) {
	//Loops can just use pointers... probably...
	Mark *ct = &r->markers[0];
	uint8_t srch[ 2048 ];
	int top = 0;
	struct renderDT d[10] = {{0},{0},{0},{0},{0},{0},{0},{0},{0},{0}};
	struct renderDT *dt = d;
	memset( srch, 0, sizeof(srch) );

	//Simple looping won't quite work, kind of.
	//without recursion
	while ( ct->blob ) {
		//
		ct++;
	} 

	return 1;
}; //d[10];
