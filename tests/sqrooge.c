/* why didn't I write this yet? */

#define TEST_DB_NAME "testDbName.db"

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

const unsigned int bs_jpg_len = 523;



//let's get intense with testing again
struct SqItem {
	const char *testName; 
	const char *tableName;
	const char *sql;
	SQWrite sq[10];
	int sentinel;
} sq_items[] = 
{
	{ 
	.testName = "create"
 ,.tableName = "info"
 ,.sql = ""
		"CREATE TABLE %s ( "
		" fname  TEXT, "
		" mname  TEXT, "
		" lname  TEXT, "
		" id     INTEGER, "
		" addr   TEXT, "
		" email  TEXT, "
		" datem  DATETIME, "
		" avatar BLOB  "
		" );"
 ,.sq   = { { .sentinel = 1 } }
	},

	//inserts via the SQWrite struct should work ith both numeric and alpha arguments
	{ 
	.testName = "insert via structure"
 ,.tableName = "info"
 ,.sql = "INSERT INTO %s VALUES ( ?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8 )"
 ,.sq  = {
			 { SQ_TXT, .v.c = "Antonio" } 
			,{ SQ_TXT, .v.c = "Ramar" } 
			,{ SQ_TXT, .v.c = "Collins" } 
			,{ SQ_FLT, .v.f = 3234 } 
			,{ SQ_TXT, .v.c = "2762 Burlington Ave N, St. Pete, FL" } 
			,{ SQ_TXT, .v.c = "ramar.collins@gmail.com" } 
			,{ SQ_DTE }
			,{ SQ_BLB, .v.d = bs_jpg, .len = 523 } 
			,{ .sentinel = 1 }
		}
	},

#if 0
	{ 
	.name = "create sequential"
 ,.sql = ""
		"CREATE TABLE infoSeq ( "
		" seq_id INTEGER PRIMARY KEY AUTOINCREMENT, "
		" fname  TEXT, "
		" mname  TEXT, "
		" lname  TEXT, "
		" id     INTEGER, "
		" addr   TEXT, "
		" email  TEXT, "
		" datem  TEXT, "
		" avatar BLOB  "
		" );"
 ,.sq   = { { .sentinel = 1 } }
	},

	//inserts via the SQWrite struct should work ith both numeric and alpha arguments
	{ 
	.name = "insert via structure sequential"
 ,.sql = "INSERT INTO infoSeq VALUES ( ?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8 )"
#if 1
 ,.sq  = {
			 { SQ_TXT, .v.c = "Antonio" } 
			,{ SQ_TXT, .v.c = "Ramar" } 
			,{ SQ_TXT, .v.c = "Collins" } 
			,{ SQ_TXT, .v.f = 3234 } 
			,{ SQ_TXT, .v.c = "2762 Burlington Ave N, St. Pete, FL" } 
			,{ SQ_TXT, .v.c = "ramar.collins@gmail.com" } 
			,{ SQ_DTE }
			,{ SQ_BLB, .v.d = bs_jpg, .len = 523 } 
			,{ .sentinel = 1 }
		}
#endif
	},

	//get a query with one record 
	{
	.name = "getSingleRecord"
 ,.sql  = ""
	"	SELECT TOP 1 FROM info "
 ,.sq   = { { .sentinel = 1 } }
	},
#endif

	//get a query with three records

	//get a query with 100 records

	//get a query with no records (does it crash, etc?)

	//select should work

	//queries should work with or without ';'
	{ .sentinel = 1 }
};

TEST( sqrooge )
{
	//Loop through all the test results
	struct SqItem *items = sq_items;		

	//Always delete whatever file was created.
	if ( unlink( TEST_DB_NAME ) == -1 )
		fprintf( stderr, "%s\n", strerror( errno ) );

	//Loop through all tests
	while ( !items->sentinel ) {
		fprintf( stderr, "Testing '%s'\n", items->testName );
		Database db;
		char sqlQuery[ 2048 ] = {0};

		//Write a new string based on the data that is there.
		snprintf( sqlQuery, 2048, items->sql, items->tableName );

		//Open database every time.
		if ( !sq_open( &db, TEST_DB_NAME ) ) {
			fprintf( stderr, "%s\n", db.errmsg );
			break;	
		}
	
		//Either execute or insert depending on data in the test 
		if ( items->sq->sentinel ) {
			if ( !sq_exec( &db, sqlQuery ) ) {
				fprintf( stderr, "%s\n", db.errmsg );
				break;	
			}
		}
		else {
			if ( !sq_insert( &db, sqlQuery, items->sq ) ) {
				fprintf( stderr, "%s\n", db.errmsg );
				break;	
			}
		}

		//0. Reset buffer for SQL statement.
		memset( sqlQuery, 0, 2048 );
		snprintf( sqlQuery, 2048, "SELECT * FROM %s", items->tableName );

		//1. Do a select that would pull back what I expect 
		if ( !sq_save( &db, sqlQuery, "results", NULL ) ) {
			fprintf( stderr, "%s\n", db.errmsg );
			break;	
		}

		//2. Dump table to a big string
		while ( 0 ) { 
			;
		}

		//3. Use memcmp to compare what came back.
		if ( memcmp( (int*)1, (int*)1, 1 ) == 0 )
			0;
		else {

		}

		//Close the database each time.	
		if ( !sq_close( &db ) ) {
			fprintf( stderr, "%s\n", db.errmsg );
			break;	
		}	

		items++;	
	}

#if 0
	//Delete should always happen after the fact...
	//Always delete whatever file was created.
	if ( unlink( TEST_DB_NAME ) == -1 ) {
		fprintf( stderr, "%s\n", strerror( errno ) );
		return 0;
	}
#endif
	return 0;
}


