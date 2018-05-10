/* why didn't I write this yet? */

//let's get intense with testing again
struct SqItem {
	Database *db;	
	const char *sql;
	SQWrite sq;
};

//get a query with no records (does it crash, etc?)

//get a query with one record 

//get a query with three records

//get a query with 100 records

//create should work

//select should work

//inserts via the SQWrite struct should work with both numeric and alpha arguments

//queries should work with or without ';'


TEST( sqrooge )
{
	return 0;
}
