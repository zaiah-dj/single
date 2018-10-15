
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

void render_dump_mark ( Render *r ) {
	Mark *ct = &r->markers[0];
	while ( ct->blob ) {
		fprintf( stderr, "{ size:   %-4d,", ct->size  );
		fprintf( stderr,  " type:   %-1d,", ct->type  );
		fprintf( stderr,  " index:  %-4d,", ct->index );
		fprintf( stderr,  " action: %-6s,'",
			&"RAW  \0""-LOOP\0""^LOOP\0""$LOOP\0""STUB \0""DIRECT\0""KEYVAL\0"[ ct->action * 6 ] ); 
		fprintf( stderr,  " lcount: %-4d,", ct->loopcount );
		fprintf( stderr,  " ix: %-10p,\n\t", ct->ix );
		for ( int i=0 ; i < ct->size ; i++ )
			fprintf( stderr, "%c", (ct->blob[ i ] == '\n' ) ? '@' : ct->blob[ i ] ); 
		//write( 2, ct->blob, ct->size );
		fprintf( stderr, "' },\n" );
		ct++;
	}
}


//This finds all integer keys that belong to a table located at a certain depth.
//This is used to assist in finding hashes of values within tables that might
//not usually be fetched explicitly
static int render_prep_ints ( Render *r, int hash, int **p ) {
	//Set index and set current
	lt_set( r->srctable, hash + 1 );
	LiteKv *ik = lt_current( r->srctable );
	int c = lt_countall( r->srctable ) - 1;
	int a = 0;
	int *pp = *p;

	//Loop and find the shallow keys
	while ( (ik = lt_current( r->srctable )) && ((r->srctable)->index < c) ) {
		//write the key and whatnot
		if ( ik->key.type == LITE_INT ) {
			*pp = ik->key.v.vint; pp++; a++;
		} 

		//if its a table, there are other things to do
		if ( ik->value.type == LITE_TBL ) {
			//have to just move forward until it's a terminator
			int dep=1;
			for ( int i=(r->srctable)->index; dep > 0 ; i++ ) {
				//Advance by one, get new table, 
				lt_set( r->srctable, 1 );
				LiteKv *ik = lt_current( r->srctable );
				dep += (ik->value.type == LITE_TBL) ? 1 : 0; 
				dep -= (ik->key.type == LITE_TRM) ? 1 : 0; 
			}
		}

		//Advance again (lt_next is really not working at all)
		lt_set( r->srctable, 1 );
	}
	return a;
}


//Do a map
int render_map ( Render *r, uint8_t *src, int srclen ) {
	//Define stuff
	Mark *raw=NULL, *ct=NULL; 
	struct parent *par = r->parent;
	memset( r->parent, 0, sizeof( struct parent ) * 10 );
	uint8_t *parent = NULL;
	unsigned int psize = 0;
	Parser p = render_dialect;
	pr_prepare( &p );
	int depth=0, follow=1;
lt_dump( r->srctable );
	lt_reset( r->srctable );

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
			ct->action = R_PLOOP;
//ct->loopHasMany = 
			depth++;
		}
		else if ( t == '^' ) {
			//The start of "negative" loops (items that should be false)
			ct->blob = &src[p.prev],
			ct->size = p.size,
			ct->action = R_RAW;
			REALLOC( raw, r->markers );
			ct->action = R_NLOOP;
			depth--;
		}
#if 0
		else if ( t == '$' ) {
			//The start of "positive" loops (items that should be true)
			ct->blob = &src[p.prev],
			ct->size = p.size,
			ct->action = R_RAW;
			REALLOC( raw, r->markers );
			ct->action = R_KLOOP;
			depth++;
		}
#endif
		else if (t == '/') {
			//The end of either a "positive" or "negative" loop
			ct->blob = &src[p.prev],
			ct->size = p.size,
			ct->action = R_RAW;
			REALLOC( raw, r->markers );
			ct->action = R_ELOOP;
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
			int stat = ( ct->action != R_PLOOP && ct->action != R_ELOOP && ct->action != R_NLOOP );

			//...
			if ( *ct->blob == '.' && stat ) {
				ct->action = R_STUB;
				//the parent and other calc should have been done already, so copy stub
				//and loop
		
				//define buffers for stubs and loops	
				int *p = ct->ix;
				int tmplen=0;
				uint8_t tmp[1024]={0}; 

				//do the copy once
				memcpy( tmp, parent, psize );
				tmplen += psize;
				memcpy( &tmp[ tmplen ], ".", 1 );
				tmplen ++;
write( 2, tmp, tmplen ); write( 2, "\n", 1 );
niprintf( ct->loopcount );	
				//then copy each new index and add it to the int chain	
				for ( int i=0, len=0; i<ct->loopcount; i++ ) {
					char ii[10]={0};
niprintf( *p );
nsprintf( "sup" );
					int l = snprintf(ii,9,"%d",*p);
					memcpy( &tmp[ tmplen ], ii, l );
					tmplen += l;
					len += l;
					memcpy( &tmp[ tmplen ], ct->blob, ct->size );
					tmplen += ct->size;
					len += ct->size;
write( 2, tmp, tmplen ); write( 2, "\n", 1 );
					tmplen -= len;
				}

//show me the stub
				getchar();
			}
			else {
				//get the index of the first table... (should just be a regular element)
				if ( !par->size ) {
					ct->index = lt_get_long_i( r->srctable, ct->blob, ct->size );
					ct->ix = malloc( sizeof(int) );
					*ct->ix = ct->index; 
					ct->type = lt_vta( r->srctable, ct->index );
					//fprintf( stderr, "first i: %d, t: %d\n", ct->index, ct->type );
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
					ct->ix = malloc( sizeof(int) );
					*ct->ix = ct->index; 
					ct->type  = lt_vta( r->srctable, ct->index );
				}

				int parentHash=0, skeyCount=0, st=0;
				st = (ct->action==R_PLOOP || ct->action==R_NLOOP || ct->action==R_KLOOP);

				//ct->type == LITE_TBL
				if ( (ct->type == LITE_TBL) && st ) {
					if ( depth == 1 ) {
						parent = (&par[ 0 ])->src = ct->blob; 
						(&par[ 0 ])->size = ct->size;	
						psize = (&par[ 0 ])->size;// = ct->size;	
					}
					else {
						parent = (&par[ depth - 1 ])->src = ct->blob; 
						(&par[ depth - 1 ])->size = ct->size;	
						psize = (&par[ depth - 1 ])->size; 
					}

					//Get the hash, get a count of the shallow elements, and allocate
					parentHash = lt_get_long_i(r->srctable, parent, psize); 
					skeyCount = lt_counti(r->srctable, parentHash);

					//if ((h = lt_get_long_i(r->srctable, parent, psize)) == -1 || !(c=lt_counti(r->srctable, h)))
					//Don't allocate if there were no elements here
					if ( parentHash == -1 || skeyCount < 2 )
						ct->ix = NULL;
					else {
						ct->ix = malloc( sizeof(int) * skeyCount + sizeof(int) );
						memset( ct->ix, 0, sizeof(int) * skeyCount + sizeof(int) ); 
						//gotta catch this...	
						if ( !ct->ix ) ;
						//*ct->ix = 0;
						//loop through the indices and find the hashes
						ct->loopcount = render_prep_ints( r, parentHash, &ct->ix );	
#if 0
niprintf( *&ct->ix[0] );
niprintf( *&ct->ix[1] );
niprintf( *&ct->ix[2] );
niprintf( *&ct->ix[3] );
exit( 0 );
#endif
					}
					ct->type  = lt_vta( r->srctable, ct->index );
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
		if ( ct->action != R_ELOOP && dt->skip ) { 
			ct++; 
			continue; 
		}
		else if ( ct->action == R_ELOOP ) {
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
		else if ( ct->action == R_NLOOP || ct->action == R_PLOOP ) {
			if ( ct->action == R_NLOOP && ct->index > -1 )
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

write(2,par,parlen);

niprintf( dt->times ); exit( 0 );

//find index, move and do work
int a = lt_geti( r->srctable, "cities" );
lt_set( r->srctable, a );
LiteKv *ik = lt_current( r->srctable );

//if my index is a table, i'll have to loop
//and find the shallow keys
do {
	char keybuf[ 1024 ] = {0};
	if ( ik->key.type == LITE_BLB )
		memcpy( keybuf, ik->key.v.vblob.blob, ik->key.v.vblob.size );
	else if ( ik->key.type == LITE_TXT )
		memcpy( keybuf, ik->key.v.vchar, strlen( ik->key.v.vchar )); 
	else if ( ik->key.type == LITE_INT ) {
		snprintf( keybuf, 1023, "%d", ik->key.v.vint );	
	}

	//if the type is a table, skip the count 
	fprintf( stderr, "[%d] => '%s' (%s) -> (%s)\n", 
	   (r->srctable)->index
		,keybuf 
		,lt_typename( ik->key.type )
		,lt_typename(ik->value.type ) 
	);
	if ( ik->value.type == LITE_TBL ) {
		//get count
		int el = lt_counti( r->srctable, (r->srctable)->index );  
		niprintf( el );	
		niprintf( r->srctable->index );	
		niprintf( lt_set( r->srctable, el ) );
		niprintf( r->srctable->index );	
	}
	getchar();
} while ( (ik = lt_next( r->srctable )) );


fprintf(stderr,"stopping function...\n" ); getchar(); exit( 0 );
						}
					}
				}
			}
		}
		ct++;
	}
	
	return 1;
}
