/* 

renderLastTry.c

This is my third attempt to get "rendering" or templating to work 
with the Table structure that comes with 'single'

- # loop
- # boolean match (if key is not a table) 
- ^ inverted loop (if the key is false or doens't exist)
- ! ignore, b/c it's a comment

Not Mustache
- . stub, simple referencing matching

*/ 

int main (int argc, char *argv[]) {


	//SEPERATE STEPS

	//First part would map out everything, can still be seperate
#if 0
	//condition can be just about anything
	while ( marks still exist && (condition == true) ) {

		'#'
			starts loop
			extract text 
			parent = text
			hash = get hash 	
			type = get type
			depth++

		'/'
			ends loop
			extract text 
			if ( ! text matches the parent )
				fail
			else {
				depth--	
			}	
	
		'^'
			extract text
			hash = get hash
			if ( hash == -1 ) {
				whatever is in between is the raw extraction
			}
		
		'}'
			this should never be used if written correctly...	
				
		
	}
#endif
	
	//Second step would actually loop through and pull all the replacements	
}
