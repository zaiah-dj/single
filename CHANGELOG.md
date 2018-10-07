# CHANGELOG

## STATS

- Commit count:       67
- Project Inception Date:   Tue May 2 08:23:23 2017 -0400
- Last Commit Date:   Thu Jul 5 20:18:30 2018 -0400
- Authors:
t- Antonio Collins <antonio.collins.contractor.akc.org>
t- Antonio Collins <ramar.collins@gmail.com>
t- Antonio R. Collins II <arcollin@wakehealth.edu>
t- Antonio R. Collins II <ramar.collins.com>
t- Antonio R. Collins II <ramar.collins@gmail.com>
t- Antonio Ramar Collins II <ramar.collins@gmail.com>
t- AntonioRamarCollinsTheSecond <antonio@debian.wrossler>
t- Bulbous Grammatical Elephant Man <bulbous@krunker>

## HISTORY

commit d73d0b793d09a88b6c84fb2494b19a4cfb45b9b2
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Thu Jul 5 20:18:30 2018 -0400

-  edited todo.

commit 8932b57a46c10806ea81fa3a58ecad1d620a6826
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Thu Jul 5 20:15:19 2018 -0400

-  Fixed Option processing bug and added a lot to Socket module.
-  
-  Also added documentation from some other commit.  Can't figure out if this should stay in the file or in some other .md file...
-  Don't really care.  Might as well just keep it here... and strip comments if you must read it in another form...

commit 1fea9dd2157c26084abccf57b17efa2032cd2aa5
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Sun Jun 17 14:32:59 2018 -0400

-  removed large comment block.

commit 99f96a1add75625819dd8e72e127300ad92bd73a
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Fri Jun 15 14:20:02 2018 -0400

-  Fixed a bug in opt, in which arguments were not being returned correctly.

commit e06adf4eb7c8bafb54bcb23351c043bc86b43c4a
Merge: 1b88692 7c62529
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Wed Jun 6 05:53:57 2018 -0400

-  Merge branch 'dev' of https://github.com/zaiah-dj/single into dev

commit 1b886921ddaaa564d3914a73724ea10abbc22a41
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Wed Jun 6 05:53:25 2018 -0400

-  Fixed a formatting issue in sq_lexec.  -Werror catches and does not compile.

commit 7c62529deea511825b2b1d7b72c3715f6fa8c552
Author: Antonio R. Collins II <arcollin@wakehealth.edu>
Date:   Wed May 30 14:57:53 2018 -0400

-  Fixed opt_eval function to return proper errors.
-  
-  - This module needed a little extra work to act like all of the others.
-  
-  - It works, but is a little funky and involves me rewinding a pointer to report correctly.
-  
-  - Still have to rework the tests to evaluate opt_eval's return code against an expected status.

commit 9b832be53c13fcb9be1af164e3a08c2f0448326d
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Sun May 27 11:11:32 2018 -0400

-  Working on Socket stuff.

commit 1fcc1b75f34b42004323a5f2c1551decea110968
Merge: 90f3f73 d4ba9fb
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Thu May 24 13:20:58 2018 -0400

-  Merge branch 'dev' of https://github.com/zaiah-dj/single into dev

commit d4ba9fb5a5e30723de5108225c010855e172abb4
Author: Antonio R. Collins II <arcollin@wakehealth.edu>
Date:   Thu May 24 13:18:20 2018 -0400

-  Deleted extraneous SQL functions.
-  
-  - NEED TO TEST tests/sqrooge.c AGAIN.

commit 90f3f73f1e82d288504e9cf676e4a2f7eaf690ae
Merge: 06fa875 0af1a0c
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Wed May 23 22:09:42 2018 -0400

-  Merge branch 'mergeme' into dev

commit 06fa875bf0be7e3df07b443841c38bdf1a01d9af
Merge: 7ebeb01 99378c9
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Wed May 23 22:09:25 2018 -0400

-  Merge branch 'dev' of https://github.com/zaiah-dj/single into dev

commit 0af1a0ce0793897330c4772cb94eb92adc53b907
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Wed May 23 22:08:42 2018 -0400

-  ...

commit 7ebeb01f87c70f720b798bc8c6f95d415af69879
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Wed May 23 22:06:47 2018 -0400

-  Added 'opt' tests.
-  
-  Strange crashes occur sometimes when specifying flags that need arguments.  More careful testing will solve it.

commit 99378c97d77dbc6cba0e69ddc07a817acb4daa13
Author: Antonio R. Collins II <arcollin@wakehealth.edu>
Date:   Wed May 23 15:43:47 2018 -0400

-  ...

commit da7226309110b23bdb7c42caab6bdb03439930c5
Author: Antonio R. Collins II <arcollin@wakehealth.edu>
Date:   Wed May 23 14:14:01 2018 -0400

-  added some new md files for better docs.

commit 7889d80388da74daf5e2740c97bc0ae5d880970b
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Wed May 23 13:18:34 2018 -0400

-  Finished most of automated SQL testing.  Would love to add MySQL and PostgreSQL to the suite.

commit 76894e6db9f7ca47f2d16989c44894240ac9c7b7
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Wed May 23 11:16:02 2018 -0400

-  I don't know what changed.  Sorry :(

commit 29a71592da124fb322b6e449684ccb6d16c8f679
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Mon May 21 08:31:08 2018 -0400

-  ...

commit 4a82b0a7a67321759831d62a9808df6c34bf3f5b
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Fri May 18 20:26:20 2018 -0400

-  Cleaned up more in sqrooge tests.

commit 3338fac8bb97a20cfba93d4fdc012b0efd4f03d9
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Fri May 18 20:17:51 2018 -0400

-  Finished a new suite of tests.  Have to add something better for checking result sets.

commit 660902a2288b4626dae901e5de2532a364e0807e
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Fri May 18 09:23:07 2018 -0400

-  Still debugging sq_ex(...)

commit e4ddf071874e85e1a9be9f550defe9deb5e1d1be
Author: Antonio R. Collins II <arcollin@wakehealth.edu>
Date:   Fri May 11 16:20:31 2018 -0400

-  Added a new SQL module.

commit 725707cce56e558f53c5cd810c0e52952884508e
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Fri May 11 10:31:40 2018 -0400

-  Added additional tests to sqrooge.c.
-  
-  - sq_save still can't run over zero length queries.  Ridiculous.
-  - .gitignore now will actually exclude binary files.  Not sure why this wasn't here earlier.
-  - single.h and single.c now have some fairly useful defines for debugging and error reporting.
-  - Makefile has a new target for testing the sql stuff, will be out of the file once testing is complete.

commit e41ea5359ec2e3102372129394871944674e17aa
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Thu May 10 21:07:02 2018 -0400

-  Keep adding more to SQL tests and whatnot.

commit 6f5b038edf46353865b26b69b8468dfe3b6c5ab7
Author: Antonio R. Collins II <arcollin@wakehealth.edu>
Date:   Thu May 10 15:58:46 2018 -0400

-  Started on SQL tests and rewrote error functions in a couple of the SQL functions.

commit 309283cdeb4a2403132f280275daacef9ee3bba1
Author: Antonio R. Collins II <arcollin@wakehealth.edu>
Date:   Thu May 10 11:09:03 2018 -0400

-  Changed error string array name.  Started looking at err() macros and coming up with a way to do different types of reporting.

commit 8b25f116d3fea4a0ef97923b480a467374584b44
Author: Antonio R. Collins II <arcollin@wakehealth.edu>
Date:   Thu May 10 10:49:28 2018 -0400

-  Added new error handling members to most common structs.

commit c35b9b9a11260c6f8d0a30d44ee7e5511bf7bb31
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Wed Apr 4 04:57:59 2018 -0400

-  Edited rendering tests.

commit 181b11695e3a73bde8fa5a997d3349c0a28e5990
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Wed Mar 28 14:54:18 2018 -0400

-  Fixed some compile warnings.

commit 2b38de7ce988f1e98591f40830fcad42c2e427ed
Author: Antonio Collins <antonio.collins.contractor.akc.org>
Date:   Thu Feb 1 15:55:11 2018 -0500

-  moving a new set of code to dev

commit 5cb933cd9eacb925069838a72cc4e8708c5377a4
Merge: 09de823 d435824
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Thu Feb 1 11:44:40 2018 -0500

-  Merge branch 'dev' of https://github.com/zaiah-dj/single into dev

commit 09de823bcb1d48e4d06dc0581d7cf843b9050e86
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Thu Feb 1 11:42:41 2018 -0500

-  yay

commit a156aefa2cd2016fb69ab9e0f69736632cb23ec7
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Thu Feb 1 08:50:57 2018 -0500

-  progress...

commit a0ebae17850759d5d04c9bd54d3abe056cf241f5
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Wed Jan 31 22:37:56 2018 -0500

-  Still working...

commit af552ea8f64d215eb546235a9cf2752d97a8da5e
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Wed Jan 31 10:29:05 2018 -0500

-  Added a new tool for templating.
-  
-  Should be done by the end of the day.

commit d435824354bd0038446d3e8c7c36a2780bae6cf4
Author: Antonio R. Collins II <ramar.collins.com>
Date:   Wed Jan 24 16:26:31 2018 -0500

-  Still adding to new table design.

commit d746f7f7369d6f1c8e8a7ded5d0d2ecdf09949e7
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Tue Jan 23 21:48:10 2018 -0500

-  Modified the render tests slightly.
-  
-  - Newlines are annoying when doing any kind of templating...
-  
-  - Looping still cannot be controlled very well (hence the reason for
-    modifying the test)

commit 3040cb1efc6f1fe8ba53401de2c08a492cec3ac1
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Tue Jan 23 21:29:54 2018 -0500

-  Fixed the table problem.
-  
-  - The table bug has finally been fixed.  Each new table still had the FIRST table added set as its oldest parent.  So when going back up through the "ancestry" chain, the root element (which isn't a table at all) was never selected, so incrementing the element count would affect both the entire Table and the first table added.
-  
-  Lots of other things changed:
-  
-  - added gcc's stackdump files to .gitignore
-  
-  - Added a way to count tables in the obvious way or the mathematical way.  All tables use a terminating element that takes up some space.   (That's why when a lt_dump is used, a long number always shows up as the final key in a table).
-  
-  - Added some extra tables to render.c for testing.
-  
-  - tests/tab.c may have changed in some imperceptible way.
-  
-  - added a real lt_countall, since the last one must have been written when I was high...
-          this new function returns all elements in the entire Table, versus just the elements in the first entry...

commit 4266f3265cc40990ccd772e5abe842ced3960ccd
Author: Antonio R. Collins II <ramar.collins.com>
Date:   Tue Jan 23 17:18:20 2018 -0500

-  Continued working on tests.
-  
-  Boy, this bug is hard to track down.

commit 8d38550478cfd4b27f5bda44064da53fff6aca79
Author: Antonio R. Collins II <ramar.collins.com>
Date:   Tue Jan 23 15:50:26 2018 -0500

-  Added some new macros for tests.
-  
-  Also finished timer tests.

commit df774416f7702159f8049cef564a8c89f1b7f6f8
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Sun Jan 21 13:38:02 2018 -0500

-  Figured out where to start looking.
-  
-  Took most of the day to do this.
-  Not moving FIXME to keep this repo clean.

commit 7f790cee0173efce6deea64c47e2ac409402c8d3
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Fri Jan 19 00:08:44 2018 -0500

-  More testing.

commit 5d63121064478cf9d5a3ead39990be9478319587
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Thu Jan 18 22:31:30 2018 -0500

-  Working hard on new tests...

commit dca4a285a92aaa183f76a4565be7662b304f2bc3
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Thu Jan 18 02:10:39 2018 -0500

-  Added some additional tests...

commit 30e3658a00115e3ca20c1261edbbd8d417347150
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Thu Jan 18 01:42:16 2018 -0500

-  ....

commit bfd4fe4242fb822d82e5436bc2eb2b9a1c53c375
Merge: 614029f b1f9929
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Wed Jan 17 10:10:32 2018 -0500

-  Merge branch 'dev' of https://github.com/zaiah-dj/single into dev

commit 614029f5ae433687962afae781055258b6a557c4
Merge: b11a631 a0addd5
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Wed Jan 17 10:10:06 2018 -0500

-  Merge branch 'master' of https://github.com/zaiah-dj/single into work

commit b1f992944ff93549da944d6c6fa5a104695b724a
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Wed Jan 17 10:03:31 2018 -0500

-  Started working on new render tests.
-  
-  Hangups are:
-  checking the results of a finished template run via automated means.
-  generating more random data - notice that LOREMIPSUM has been added to harness.h

commit a0addd53ec091403f53126d5b887dfdef71e4f42
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Wed Jan 17 10:01:41 2018 -0500

-  Added stdlib.h to harness.c.  Need this for random numbers.
-  
-  I am going to come up with a more portable way to do this, especially since crypto-grade randomness won't be important in these tests.

commit ce10fe86cbf43e05b7fe859db17e0e2c099b08a8
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Tue Jan 16 21:04:51 2018 -0500

-  Removed excessive logging at lt_free.

commit e638677c01ce1789bdf5ff5113ce7231e4f014a6
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Tue Jan 16 20:59:32 2018 -0500

-  Re-edited Makefile to build with sqlite from the vendor directory (previously failed and caused build not to work...)

commit b11a631829cbfe90815862dda4fefba45bd270bf
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Wed Jan 10 09:04:32 2018 -0500

-  Updated some docs.

commit 4ddffc517b1761b6de836475b76cf5528aa3c77a
Author: Antonio R. Collins II <ramar.collins.com>
Date:   Thu Nov 9 10:27:37 2017 -0500

-  Cleaned up Makefile and modified test target to use vendor/sqlite3.h

commit a8b668ece6d02618a74884ca91516ba080788a71
Author: Antonio Collins <antonio.collins.contractor.akc.org>
Date:   Thu Nov 9 10:20:18 2017 -0500

-  Why was an object file in here...?

commit 969ecf88c54dbf0ea6c1c2ef57b77fa3ec78af07
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Sat Oct 28 08:46:29 2017 -0400

-  Added options to control debugging output.

commit 50058f971863f3bd6f6b3b81f10a5fcb76c2397c
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Sun Oct 22 15:55:59 2017 -0400

-  Added better docs and SQLITE3_PATH and
-  
-  SQLITE3_GLOBAL to the build options.

commit 76debbad9ee005f944247f213472cea4f93e2ef5
Author: Antonio R. Collins II <ramar.collins@gmail.com>
Date:   Sun Oct 22 15:23:07 2017 -0400

-  Added overwrites for DEBUG_H
-  
-  SHOWDATA and other macros were not "zeroed out" when DEBUG_H not set.

commit 962fa7cbaede3158240f32a48f5d7f436530155e
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Tue Aug 8 22:18:56 2017 -0400

-  Added test data.

commit 09472473289f2fe24f15c02a101b963f9be5b0d5
Author: AntonioRamarCollinsTheSecond <antonio@debian.wrossler>
Date:   Fri Jun 30 02:54:37 2017 -0400

-  Added some additional tests, added vendor/sqlite3.[ch] to the build and made some format changes so the lib will compile with either clang or gcc with no errors...

commit 8b1113cb4051b4ea79bebc61f7c074c81d8115b7
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Sat May 13 11:25:44 2017 -0400

-  Added fmt, json and tab tests to the repo.

commit f3826488de36c816bfd8a1b897269edc9bc17a2f
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Thu May 11 19:15:08 2017 -0400

-  Added some new definitions in opt.
-  
-  Also added a harness program, it just adds a bunch of tests
-  from the command line and the user can compile them out by defining
-  the units that are wanted.
-  
-  Likewise, if you want to run all the tests, don't define anything
-  and run --all when you run the harness ( single-test is the name of
-  the binary ).

commit badfdc0ca1adb8964aec1d27e31af815659a8dba
Author: Antonio Collins <ramar.collins@gmail.com>
Date:   Tue May 9 17:29:42 2017 -0400

-  Created a test harness to make testing these easy and in one place.
-  
-  I should be able to put the rest of this stuff together tonight or something.

commit 90e48380dd769bfab5780688cebd659deee4c947
Author: Antonio Collins <ramar.collins@gmail.com>
Date:   Tue May 9 15:14:05 2017 -0400

-  Removed tests until they're done completely.

commit c58a7a3f0c9d994578e3aca216b3494c9e4b5797
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Tue May 2 08:32:19 2017 -0400

-  Modified README ever so slightly.   Still much more to do.

commit dd70f5f1dd6a122fb53a44ac104562cd8ea683f9
Author: Antonio Ramar Collins II <ramar.collins@gmail.com>
Date:   Tue May 2 08:23:23 2017 -0400

-  Alpha stage commit.

<link rel=stylesheet href=changelog.css>
