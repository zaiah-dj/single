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



## Caveats / Bugs
None yet :)


## Contact and Help
Got questions on how to use this?  Contact me at ramar.collins@gmail.com.
