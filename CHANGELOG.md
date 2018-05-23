# CHANGELOG

## Tue, Jan 23, 2018  9:29:37 PM
- The table bug has finally been fixed.  Each new table still had the FIRST table added set as its oldest parent.  So when going back up through the "ancestry" chain, the root element (which isn't a table at all) was never selected, so incrementing the element count would affect both the entire Table and the first table added.

## Sun Oct 22 15:55:40 EDT 2017
- Added SQLITE3_PATH and SQLITE3_GLOBAL to the build.  Users should have been able to do this a long time ago...
- Added some slightly better documentation.  Still lots to go on this....

