/* why didn't I write this yet? */

#define TEST_DB_NAME "testDbName.db"
#define MAT(a,b) #a, a, Expected##a, Check##a, b
#define DAT(a) Data##a
#define EXPECT(a) Expected##a
#define CHECK(a) Check##a

static const char *_sqlite3_C = "}}}}}}}}}";
static const char *_sqlite3_N = "{{{{{{{{{";
static const char *_sqlite3_E = "333333333";
static const int   _sqlite3_L = 9;

// a binary file
unsigned char bs_jpg[] = {
  0xff, 0xd8, 0xff, 0xe0, 0x00, 0x10, 0x4a, 0x46, 0x49, 0x46, 0x00, 0x01,
  0x01, 0x01, 0x00, 0xc8, 0x00, 0xc8, 0x00, 0x00, 0xff, 0xdb, 0x00, 0x43,
  0x00, 0x03, 0x02, 0x02, 0x03, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x04,
  0x03, 0x03, 0x04, 0x05, 0x08, 0x05, 0x05, 0x04, 0x04, 0x05, 0x0a, 0x07,
  0x07, 0x06, 0x08, 0x0c, 0x0a, 0x0c, 0x0c, 0x0b, 0x0a, 0x0b, 0x0b, 0x0d,
  0x0e, 0x12, 0x10, 0x0d, 0x0e, 0x11, 0x0e, 0x0b, 0x0b, 0x10, 0x16, 0x10,
  0x11, 0x13, 0x14, 0x15, 0x15, 0x15, 0x0c, 0x0f, 0x17, 0x18, 0x16, 0x14,
  0x18, 0x12, 0x14, 0x15, 0x14, 0xff, 0xdb, 0x00, 0x43, 0x01, 0x03, 0x04,
  0x04, 0x05, 0x04, 0x05, 0x09, 0x05, 0x05, 0x09, 0x14, 0x0d, 0x0b, 0x0d,
  0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14,
  0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14,
  0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14,
  0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14,
  0x14, 0x14, 0xff, 0xc0, 0x00, 0x11, 0x08, 0x00, 0x0c, 0x00, 0x14, 0x03,
  0x01, 0x22, 0x00, 0x02, 0x11, 0x01, 0x03, 0x11, 0x01, 0xff, 0xc4, 0x00,
  0x17, 0x00, 0x00, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x05, 0xff, 0xc4,
  0x00, 0x28, 0x10, 0x00, 0x01, 0x03, 0x03, 0x04, 0x01, 0x02, 0x07, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
  0x11, 0x21, 0x00, 0x06, 0x12, 0x31, 0x07, 0x08, 0x13, 0x22, 0x23, 0x33,
  0x41, 0x42, 0x71, 0x92, 0xff, 0xc4, 0x00, 0x15, 0x01, 0x01, 0x01, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x04, 0x01, 0xff, 0xc4, 0x00, 0x1e, 0x11, 0x01, 0x00, 0x00, 0x06,
  0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x02, 0x04, 0x05, 0x11, 0x31, 0x06, 0x91, 0xa1, 0x12, 0xff,
  0xda, 0x00, 0x0c, 0x03, 0x01, 0x00, 0x02, 0x11, 0x03, 0x11, 0x00, 0x3f,
  0x00, 0x46, 0x9f, 0xe5, 0xd8, 0xbb, 0x86, 0x52, 0xdb, 0x81, 0x0d, 0x50,
  0x1b, 0x65, 0xd5, 0x24, 0xb9, 0x2c, 0x73, 0x4b, 0xc8, 0xe2, 0x7d, 0xb5,
  0xa4, 0xd9, 0x36, 0xb9, 0x04, 0xdb, 0x36, 0xe2, 0x05, 0xfe, 0x2c, 0x6c,
  0x43, 0xdf, 0x46, 0x60, 0x65, 0x02, 0x9b, 0x77, 0xda, 0x84, 0x99, 0x92,
  0x90, 0x1c, 0x45, 0x94, 0x92, 0xfb, 0x6d, 0xf2, 0x6c, 0xdf, 0x20, 0xa5,
  0x45, 0x40, 0x77, 0x91, 0xd0, 0xd4, 0x7b, 0xba, 0x3c, 0x85, 0x5e, 0x66,
  0x5d, 0x3e, 0x51, 0xa8, 0x3c, 0xf3, 0xbf, 0x2a, 0x62, 0xbd, 0xe5, 0xa9,
  0x69, 0x5b, 0x99, 0x59, 0xe4, 0x92, 0x6c, 0x41, 0x22, 0xc7, 0x1d, 0x63,
  0x4c, 0xd4, 0x3d, 0x41, 0xef, 0xa7, 0xe6, 0x49, 0x98, 0xaa, 0xc9, 0x2e,
  0xb8, 0xbe, 0x5f, 0x49, 0x24, 0x27, 0xa2, 0x00, 0xc5, 0xc0, 0x03, 0x03,
  0x3d, 0x01, 0xa6, 0xbc, 0xb2, 0xe5, 0xf4, 0xa4, 0xfb, 0xf3, 0x50, 0x76,
  0xd7, 0x42, 0x60, 0x64, 0xd7, 0xb1, 0x61, 0x6d, 0xdf, 0x20, 0x51, 0x2a,
  0xd0, 0x5c, 0x76, 0x73, 0x4f, 0xc0, 0x92, 0xdb, 0xee, 0xb2, 0xa6, 0x5a,
  0x61, 0xc7, 0xc5, 0x92, 0xb2, 0x12, 0xae, 0x41, 0x3f, 0x92, 0x40, 0x57,
  0xe8, 0x8f, 0xb5, 0x8e, 0x8d, 0x49, 0x0f, 0x79, 0x03, 0x70, 0x4b, 0x9f,
  0x3d, 0x6a, 0xab, 0x4c, 0x41, 0x12, 0x16, 0x2c, 0xdc, 0x97, 0x12, 0x2d,
  0xfd, 0x68, 0xd3, 0x65, 0xe6, 0x57, 0x00, 0x06, 0x51, 0xee, 0x23, 0x61,
  0xa2, 0x5c, 0xe5, 0xe8, 0x8f, 0xff, 0xd9
};

static const unsigned int bs_jpg_len = 523;


static const char sqlTextCreateFirst[] = 
"CREATE TABLE info_keep_empty ( "
" fname  TEXT, "
" mname  TEXT, "
" lname  TEXT, "
" id     INTEGER, "
" addr   TEXT, "
" email  TEXT, "
" datem  DATETIME, "
" avatar BLOB  "
" );"
;

static const char sqlTextCreate[] = 
"CREATE TABLE info ( "
" fname  TEXT, "
" mname  TEXT, "
" lname  TEXT, "
" id     INTEGER, "
" addr   TEXT, "
" email  TEXT, "
" datem  DATETIME, "
" avatar BLOB  "
" );"
;

static const char sqlTextInsert[] = 
"INSERT INTO info VALUES ( \"Julia\", \"Mayhem\", \"Roberts\", 6, \"Ahoskie, NC\", \"jljl3@yahoo.com\", 2010-09-10, 0 );"
;

static const char sqlTextSelect[] = 
"SELECT fname, lname FROM info;"
;

static const char sqlTextSelectNoRows[] = 
"SELECT * FROM info_keep_empty; "
;

static const char sqlTextInsertBind[] = 
"INSERT INTO info VALUES ( ?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8 )"
;

static const char sqlTextSelectBind[] = 
"SELECT fname, id, lname FROM info WHERE id = ?1"  
;

static const char sqlTextSelectBindMultiple[] = 
"SELECT fname, id, lname FROM info WHERE id = ?1 AND lname = ?2"  
;

static const char sqlTextNonExist[] = 
"SELECT * FROM wallaby; "
;

static const char sqlTextTransaction[] = 
"BEGIN TRANSACTION;"
"SELECT * FROM info;"
"COMMIT;"
;

static const char sqlTextDrop[] = 
"DROP TABLE info; "
;

static SQWrite DAT(sqlTextInsertBind) [] = {
	 { SQ_TXT, .v.c = "Antonio" } 
	,{ SQ_TXT, .v.c = "Ramar" } 
	,{ SQ_TXT, .v.c = "Collins" } 
	,{ SQ_FLT, .v.f = 3234 } 
	,{ SQ_TXT, .v.c = "2762 Burlington Ave N, St. Pete, FL" } 
	,{ SQ_TXT, .v.c = "ramar.collins@gmail.com" } 
	,{ SQ_DTE }
	,{ SQ_BLB, .v.d = bs_jpg, .len = 523 } 
	,{ .sentinel = 1 }
};

static SQWrite DAT(sqlTextSelectBind) [] = {
	 { SQ_INT, .v.n = 1 } 
	,{ .sentinel = 1 }
};

static SQWrite DAT(sqlTextSelectBindMultiple) [] = {
	 { SQ_INT, .v.n = 2 } 
	,{ SQ_TXT, .v.c = "Roberts" } 
	,{ .sentinel = 1 }
};

static const char EXPECT(sqlTextCreateFirst)[] = "";
static const char EXPECT(sqlTextCreate)[] = "";
static const char EXPECT(sqlTextInsert)[] = "";
static const char EXPECT(sqlTextSelect)[] = "Julia|Roberts|";
static const char EXPECT(sqlTextSelectNoRows)[] = "";
static const char EXPECT(sqlTextInsertBind)[] = "";
static const char EXPECT(sqlTextSelectBind)[] = "";
static const char EXPECT(sqlTextSelectBindMultiple)[] = "";
static const char EXPECT(sqlTextNonExist)[] = "";
static const char EXPECT(sqlTextTransaction)[] = "";
static const char EXPECT(sqlTextDrop)[] = "";

static const char CHECK(sqlTextCreateFirst)[] = "";
static const char CHECK(sqlTextCreate)[] = "";
static const char CHECK(sqlTextInsert)[] = "";
static const char CHECK(sqlTextSelect)[] = "";
static const char CHECK(sqlTextSelectNoRows)[] = "";
static const char CHECK(sqlTextInsertBind)[] = "";
static const char CHECK(sqlTextSelectBind)[] = "";
static const char CHECK(sqlTextSelectBindMultiple)[] = "";
static const char CHECK(sqlTextNonExist)[] = "";
static const char CHECK(sqlTextTransaction)[] = "";
static const char CHECK(sqlTextDrop)[] = "";

struct DubChar { 
	const char *name, *stmt, *expected, *check;
	SQWrite *bind;
} dubchars[] = {
	{ MAT(sqlTextCreateFirst, NULL) },
	{ MAT(sqlTextCreate, NULL) },
	{ MAT(sqlTextSelectNoRows, NULL) }, //will still return columns
	{ MAT(sqlTextInsert, NULL) },
	{ MAT(sqlTextInsertBind, DAT(sqlTextInsertBind)) },
	{ MAT(sqlTextSelect, NULL)   },
	{ MAT(sqlTextSelectBind, DAT(sqlTextSelectBind)) },
	{ MAT(sqlTextSelectBindMultiple, DAT(sqlTextSelectBindMultiple)) },
	{ MAT(sqlTextTransaction, NULL) },
	{ MAT(sqlTextNonExist, NULL)   },
	{ MAT(sqlTextDrop, NULL)   },
	{ NULL, NULL }
};


//Needed to track row, column and uint8_t block for SQL results.
typedef struct DbNext {
	Buffer b;
} DbNext;


//Use this to create a uint8_t block from SQL results.  Easy for comparing against a string.
int lt_makestr ( LiteKv *kv, int i, void *p )
{
	DbNext *n = ( DbNext * )p;
	Buffer *b = &n->b;

	//need a way to differentiate between new rows...	
	switch ( kv->value.type ) {
		case LITE_INT:	
		case LITE_FLT:	
			break;
		case LITE_TXT:	
			bf_append( b, kv->value.v.vblob.blob, kv->value.v.vblob.size );
			bf_append( b, (uint8_t *)"|", 1 );
			break;
		case LITE_BLB:	
			bf_append( b, kv->value.v.vblob.blob, kv->value.v.vblob.size );
			bf_append( b, (uint8_t *)"|", 1 );
			break;
		case LITE_NUL:	
		case LITE_USR:	
		case LITE_TBL:	
		case LITE_TRM:	
		case LITE_NOD:	
		case LITE_NON:
			break;	
	}	
	return 1;	
}


TEST( sqrooge )
{
	struct DubChar *t = dubchars;
	char buf[BUFLEN]={0};

	//Loop through all the test results
	while ( t->name ) {
		Database db;
		struct DbNext dbnext;
		VPRINT( "Testing: %s\n", t->name );
		
		//Open database every time.
		VPRINT( "!sq_open( &db, " TEST_DB_NAME " )" ); 
		if ( !sq_open( &db, TEST_DB_NAME ) ) {
			fprintf( stderr, "%s\n", db.errmsg );
		}

		//Nice to just need the one function here... and macros can be there for convenience
		VPRINT( "!sq_lexec( &db, sqlQuery, items->tableName, NULL ) )\n" );
		if ( !sq_lexec( &db, t->stmt, "res", t->bind ) ) {
			fprintf( stderr, "%s\n", db.errmsg );
		}

		//Test differences
		if ( strlen( t->expected ) > 0 ) {
			bf_init( &dbnext.b, NULL, 4 );
			//Write all database results to something else (col seperated by |, rows by \n
			if ( !lt_exec( &db.kvt, &dbnext, lt_makestr ) ) {
				fprintf( stderr, "%s\n", db.errmsg );
				RPRINTF( "FAILED, Could not loop through result set" );
			}
			//Check if any other SQL queries need to be run in order to check.
			if ( t->check && strlen(t->check) > 0 ) {
				if ( !sq_lexec( &db, t->check, "check", NULL ) ) {
					RPRINTF( "FAILED, Something is wrong with the check query..." );
					return 0;
				}
			}
			//Check for zero length (there was probably an error elsewhere)
			if ( bf_written( &dbnext.b ) == 0 ) {
				RPRINTF( "FAILED, No data written." );
			}
			//Copy to buffer
			char tmp[ 2048 ] = { 0 };
			int lim = ( bf_written( &dbnext.b ) > 2048 ) ? 2048 : bf_written( &dbnext.b );
			memcpy( tmp, bf_data( &dbnext.b ), lim ); 
			tmp[ lim ] = '\0';
			//Check if both blocks are even.
			if ( memcmp(t->expected, bf_data( &dbnext.b ), bf_written( &dbnext.b )) != 0 )
				{ RPRINTF( "FAILED, '%s' != '%s'", t->expected, tmp ); }
			else {
				RPRINTF( "SUCCESS, '%s' == '%s'", t->expected, tmp );
			}
		}
		else {
			//I'd have to run a query to check against certain types of code...
			RPRINTF( "SUCCESS!" );
		}

		//Close the database each time.	
		if ( !sq_close( &db ) ) {
			fprintf( stderr, "%s\n", db.errmsg );
		}
		//fprintf( stderr, "Press enter to go to the next test...\n" );getchar();
		t++;
	}

	//Delete should always happen after the fact...
	//Always delete whatever file was created.
#if 1
	if ( unlink( TEST_DB_NAME ) == -1 ) {
		fprintf( stderr, "%s\n", strerror( errno ) );
		return 0;
	}
#endif

	return 1;
}


