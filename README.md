# single

## Summary
<p>
Single is a two file open-source library for common functionality in C.

It includes:
<ul>
<li>TCP Sockets</li>
<li>Buffers</li>
<li>Option processing</li>
<li>Parsing primitives</li>
<li>A Mime library</li>
<li>A clock timer</li>
<li>Rendering</li>
<li>A Table implementation (similar to Lua's table structure)</li>
<li>A JSON parser</li>
<li>An SQL driver (but only for SQLite right now)</li>
<li>Random number and string generator</li>
<li>...and many general utilities</li>
</ul>
</p>



## Usage

### Building
<p>
An object can be built with the following:
<pre>
$	gcc -Wall -Werror -Wno-unused -c single.c  
</pre>

Reference the library's functions using 
<pre>
#include "single.h"
</pre>
</p>



### Package selection 
<p>
single will let you remove certain parts of the build by defining compile-time flags.  For example, if you do not want
the timer library, you can define TIMER_H to skip building the module. 
<pre>
$	gcc -Wall -Werror -Wno-unused -c single.c -DTIMER_H
</pre>
</p>



## More

single is the result of combining a lot of separate utilities into one useful library.  The included functionality can be controlled by a set of compilation flags.  What do I mean by this?:

Assume that you're writing some a game that processes basic messages from a network socket.  You may already have a SQL driver that you're planning on using.  Or maybe you are using Sean Barrett's `stretchy buffers` from stb_buffer.   Even though this library includes an implementation of both of those--they can be disabled by specifying the flags -DBUFF_H and -DSQROOGE_H during compilation.  Your program won't have this code compiled into the final version of your program.



### Error Handling


## How Tos and Examples

### Buffers

Buffers can be loosely described as "flexible unsigned char \*"s.  You can use them to store (and more importantly) append to strings.  Here's a picture:

<pre>
//Assume we've got a regular array of chars. 
char farf[ 5 ] = { 0 };
char \*myString = "this string is way too long and will cause a crash..." ;

//I can only write 5 bytes to this array.  So if I write the following, I've got a buffer overflow.
strcpy( farf, myString ); 
</pre>

<pre>
//Now, let's try the same with a buffer.
Buffer farf;
char \*myString = "this string is way too long and will cause a crash..." ;

//The following tells the Buffer library to initialize this structure dynamically.
bf_init( &farf, NULL, 1 );  

//Then I can append as often as I want and as much as I want.
bf_append( farf, myString, strlen( myString ) );

//To read it off, I need to use the following.
char \*myNewString = (char \*)bf_data( farf );
</pre>


### Sockets

uh... :)



### Templating via the Render() module 

Setting up rendering with this library is a long ten-step process.     The first step is declaring a few types and initializing them.

<pre>
//Set up a table structure with space for at least 1024 keys
Table t;
lt_init( &t, NULL, 1024 );

//Set up a render structure
Render rn;
memset( &rn, 0, sizeof(Render));
render_init( &rn, &t );
</pre>

So what we've just done is set up a hash table, an instance of Table.  The rendering module uses this hash table when running its process.  The render_init step sets up some key fields in the Render structure.   It is written this way because the render buffer size will eventually be able to be set by the user.


<pre>
//Declare a bunch of stuff
uint8_t ab[];
Render R;
Table t;

//Somehow initialize the table...
// init_table( &t );
</pre>


<pre>
//Initialize the templating engine 
render_init( &R, &t );
</pre>

Before you start doing any templating, you'll need to initialize the Render datatype.  You are basically telling the datatype what you plan to use.


<pre>
//Figure out where the replacements are, then use that in later runs 
render_map( &R, &ab, strlen( ab ) ); 
</pre>

At this point, the render module knows what to "find and replace".   This is not always useful, but very much so when looping through large tables.


<pre>
//This is where it actually gets done
render_render( &R )
</pre>

<pre>
//Retrieve the written data
uint8_t \*output = bf_data( render_rendered( &R ) );
int outputLen = bf_written( render_rendered( &R ) );
</pre>


## Caveats / Bugs

### Database
<p> 
Note, that the library ships a version of SQLite.  However, it is not the newest and really should be removed since this code is not really part of the base.   Since most Unix based systems include a globally installed version of the library, a compile time option can be used to utilize the system's SQLite headers. 
 
<pre>
$	gcc -Wall -Werror -Wno-unused -c single.c -DSQLITE3_GLOBAL
</pre>

However, if you have downloaded or modified your own version you may specify the full path to it using the SQLITE3_PATH flag.  Assuming we have it in a folder titled 'hero', then the following will work.  (Notice all the strange quoting rules...)
<pre>
$	gcc -Wall -Werror -Wno-unused -c single.c -DSQLITE3_PATH="\"hero/sqlite3.h\""
</pre>

Unfortunately, support for other open source SQL databases does not yet exist.
</p> 


## Contact and Help
Got questions on how to use this?  Contact me at ramar.collins@gmail.com.
