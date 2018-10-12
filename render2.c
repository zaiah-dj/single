
struct renderDT { 
	Mark *mark;
	uint8_t *src, *parent;
	int size, psize, skip, times, position;
};


//dialects should be defined on the outside,
//new ones can be set from elsewhere...
Parser render_dialect   = {
	.words = {
		{"{{#"}, 
		{"{{/"}, 
		{"{{^"}, 
		{"{{>"}, 
		{"{{"}, 
		{"}}"}, 
		{NULL}
	}
};


typedef struct
{ 
  uint8_t *blob;
  unsigned int size; 
	short int index, action /*bitmasked with type*/;
} TestMark;  


//Do a map
int render_map ( Render *r, uint8_t *src, int srclen ) {
	//Define stuff
	Mark *raw=NULL, *ct=NULL; 
	struct parent *par = r->parent;
	memset( r->parent, 0, sizeof( struct parent ) * 10 );
	//uint8_t *parent = NULL;
	//unsigned int psize = 0;
	Parser p = render_dialect;
	pr_prepare( &p );
	int depth=0, follow=1;

	//Prepare the markers
	if ( !(r->markers = malloc( sizeof( Mark ) )) )
		return 0;
	else {
		memset( r->markers, 0, sizeof(Mark) );
		ct = r->markers;
	}

	//Loop through a thing
	for ( int alloc=2, t;  pr_next( &p, src, srclen );  ) {
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

		//Just mark each section (and it's position)
		if ( t == '#' ) {
			//The start of "positive" loops (items that should be true)
			ct->blob = &src[p.prev],
			ct->size = p.size,
			ct->action = R_RAW;
			REALLOC( raw, r->markers );
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
		else if (t == '}') { 
			//Anything within here will always be a table
			ct->blob  = trim((uint8_t *)&src[ p.prev ], (char *)trimchars, p.size, &ct->size);
			//This is for all inner stubs, same code and all.
			int stat = ( ct->action != R_POSLOOP && ct->action != R_ENDLOOP && ct->action != R_NEGLOOP );
			if ( *ct->blob == '.' && stat ) {
				//reverse lookup the parent + stub	
				niprintf( ct->psize );
				ct->action = R_STUB;
			}
			else {
				//get the index of the first table... (should just
				//be a regular element)
				if ( !par->size ) { 
					ct->index = lt_get_long_i( r->srctable, ct->blob, ct->size );
					ct->type  = lt_vta( r->srctable, ct->index );
					fprintf( stderr, "first i: %d, t: %d\n", ct->index, ct->type );
				}
				else {
					//This is a temporary buffer, not the whole thing
					uint8_t b[1024]; int bs=0;

					//Copy all keys into table
					if ( depth > 0 ) {
						for ( int i=0; i < (depth - 1); i++ ) {
							memcpy( &b[ bs ], (&par[i])->src, (&par[i])->size );
							bs += (&par[i])->size;
						}

						memcpy( &b[ bs ], ".0", 2 );
						bs += 2;
						memcpy( &b[ bs ], ct->blob, ct->size );
						bs += ct->size;
					} 
				
					//Set columns	
					ct->index = lt_get_long_i( r->srctable, b, bs );
					ct->type  = lt_vta( r->srctable, ct->index );
				}

				if ((ct->type == LITE_TBL) && (ct->action == R_POSLOOP || ct->action == R_NEGLOOP)) {
					if ( depth == 1 )
						(&par[ 0 ])->src = ct->blob, (&par[ 0 ])->size = ct->size;	
					else {
						(&par[ depth - 1 ])->src = ct->blob; 
						(&par[ depth - 1 ])->size = ct->size;	
					}
				}
			}
			REALLOC( raw, r->markers );
		}
	}

	return 1;
}



//Render
int render_render ( Render *r ) {
#if 0
	char *hhhh = "cities.0.metadata.population";
	uint8_t *hh = (uint8_t *)hhhh;
	int hashFound = lt_get_long_i( r->srctable, hh, strlen(hhhh) );
lt_dump( r->srctable );
	niprintf( hashFound );
	exit( 0 );
#endif
	//Loops can just use pointers... probably...
	Mark *lt=NULL, *ct = &r->markers[0];
	unsigned char srch[1024] = {0}, par[1024] = {0};
	int parlen = 0;
	struct DT { Mark *mark; uint8_t *src, *parent; int size, psize, skip, times, position; } d[10]; 

	//everytime you descend, src is what you got, size is length of src
	//and times is times to repeat
	struct DT *dt = d;
	//int top = 0;
	memset( dt, 0, sizeof (struct DT));
	
	while ( ct->blob ) {
		//Is the skip bit on?
		if ( ct->action != R_ENDLOOP && dt->skip ) { 
			ct++; 
			continue; 
		}
		else if ( ct->action == R_ENDLOOP ) {
			//Stop skipping if these match
			if ( ct->size == dt->psize && (memcmp( dt->parent, ct->blob, dt->size ) == 0)) {
				if ( dt->skip )
					dt->skip = 0;
				else {
					//Decrement repetition
					if ( dt->times == 0 )
						dt--;
					else {
						--dt->times;
						ct = dt->mark;
						continue;
					}
				}
			}
		}

		//Simply copy this data
		if ( ct->action == R_RAW ) {
			bf_append( &r->dest, ct->blob, ct->size );
		}
		//Retrieve the reference and write it
		else if ( ct->action == R_DIRECT && ct->index > -1 ) {
			if ( ct->type == LITE_BLB ) {
				uint8_t *b = lt_blobdata_at( r->srctable, ct->index );
				bf_append( &r->dest, b, lt_blobsize_at( r->srctable, ct->index ));
			}
			else { 
				char *a = NULL, b[128] = {0};
				if ( ct->type == LITE_INT )
					snprintf( a = b, 63, "%d", lt_int_at( r->srctable, ct->index )); 
				else if ( ct->type == LITE_FLT )
					snprintf( a = b, 127, "%f", lt_float_at( r->srctable, ct->index )); 
				else if ( ct->type == LITE_USR )
					snprintf( a = b, 127, "%p", lt_userdata_at( r->srctable, ct->index )); 
				else if ( ct->type == LITE_TBL )
					snprintf( a = b, 127, "%p", (void *)&lt_table_at( r->srctable, ct->index )); 
				else if ( ct->type == LITE_TXT )
					a = lt_text_at( r->srctable, ct->index );
				else { 
					a = 0;//Skip all other types
				}	
				bf_append( &r->dest, (uint8_t *)a, strlen( a ) );
			}
		}
		else if ( ct->action == R_STUB ) {
			//Check if the key is .key or .value. Loop through keys and values...
			int i=0, p=0;
#if 0
			memcpy( &srch[ p ], dt->parent, dt->psize );
			write(2, dt->parent, dt->psize );
			p += dt->psize;
#else
			memcpy( &srch[ p ], par, parlen);
			fprintf( stderr, "%s", "parent: " );write(2, par, parlen );write(2, "\n", 1 );
			p += parlen;
#endif

			//some keys are really numeric indices, account for that here...
			//i either have to figure out what kind of table is there (mixed, num,
			//text) or
			//loop through all the keys and make sure that they are a certain type

			//if type of key is numeric, do calc here, if not, just write
			//lt_keytypeat( ..., i )

			//specific keys are tough to come by this way
			//Reverse can be done by manipulating dt->times (top = dt->times; num = top - dt->times )
			p += snprintf( (char *)&srch[ p ], 64, ".%d", dt->times );
			memcpy( &srch[ p ], ct->blob, ct->size );
			p += ct->size;
		
			write( 2, srch, p ); write( 2, "\n", 1 );	
			i = lt_get_long_i( r->srctable, srch, p );

			//niprintf( i );getchar();
		
			//Get long i, yay
			if ( i == -1 ) {
				ct++;
				continue;
			}
			else {
				uint8_t *src = NULL;
				LiteType t = lt_vta( r->srctable, i );
				memset( srch, 0, sizeof(srch) );

				if (t == LITE_USR)
					p = snprintf((char *)( src = srch ), sizeof(srch), "%p", lt_userdata_at( r->srctable, i ));
				else if (t == LITE_FLT) 
					p = snprintf((char *)( src = srch ), sizeof(srch), "%0.2f", lt_float_at( r->srctable, i ));
				else if (t == LITE_INT)
					p = snprintf((char *)( src = srch ), sizeof(srch), "%d", lt_int_at( r->srctable, i ));
				else if (t == LITE_TXT)
					p = strlen( lt_text_at(r->srctable, i) ), src = (uint8_t *)lt_text_at( r->srctable, i );
				else if (t == LITE_BLB) 
					p = lt_blobsize_at( r->srctable, i), src = lt_blobdata_at( r->srctable, i ); 
				else if (t == LITE_TBL)
					p = snprintf((char *)( src = srch ), sizeof(srch), "%p", &lt_table_at( r->srctable, i ));
				else {
					ct++;
					continue;
				}
				bf_append( &r->dest, src, p ); 
			}	
			memset( srch, 0, sizeof(srch) );
		}
		else if ( ct->action == R_NEGLOOP || ct->action == R_POSLOOP ) {
			if ( ct->action == R_NEGLOOP && ct->index > -1 )
				dt->skip = 1; //Set something
			else {
				if ( ct->index == -1 )
					dt->skip = 1;
				else {
					//No looping necessary
					if ( ct->type != LITE_TBL )
						; //This is where true or false should take place...
					else {
						dt++;
						memset( dt, 0, sizeof (struct DT));
						//Skip completely if this is a table and there are no entries
						if ( (dt->times = lt_counti( r->srctable, ct->index )) > 0 ) {
							--dt->times;  /*Adjust count b/c the sentinel has its own index*/
							dt->mark = ct + 1;
							dt->psize = ct->size;
							dt->parent = ct->blob;
							memcpy( &par[ parlen ], dt->parent, dt->psize );
							parlen += dt->psize;

LiteKv *ik = NULL; 
while ( (ik = lt_items( r->srctable, "cities" )) ) {
	char keybuf[ 1024 ] = {0};
	if ( ik->key.type == LITE_BLB )
		memcpy( keybuf, ik->key.v.vblob.blob, ik->key.v.vblob.size );
	else if ( ik->key.type == LITE_TXT )
		memcpy( keybuf, ik->key.v.vchar, strlen( ik->key.v.vchar )); 
	else if ( ik->key.type == LITE_INT ) {
		snprintf( keybuf, 1023, "%d", ik->key.v.vint );	
	}

	//if the type is a table, skip the count 
	fprintf( stderr, "[??] => '%s' (%s) -> (%s)\n", keybuf 
		,lt_typename( ik->key.type )
		,lt_typename(ik->value.type ) 
	);
	getchar();
}

fprintf(stderr,"stopping function...\n" ); getchar();
exit( 0 );
						}
					}
				}
			}
		}
		ct++;
	}
	
	return 1;
}
