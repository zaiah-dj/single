#!/usr/bin/make
NAME = single
HOST = rc
WILDCARD = *
DYLIB = so
BIN = obsrv
BIN_RENDER = obrender
BIN_CONNECT = obconnect
BINTEST = obtest
SRC = buff.c lite.c json.c mime.c nw.c http.c tab.c parsely.c render.c sqlite3.c sqrooge.c utilities.c
OBJ = ${SRC:.c=.o}
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man
#LDDIRS = -L$(PREFIX)/lib
LDDIRS = -L. -L$(PREFIX)/lib -I$(PREFIX)/include
LDFLAGS = -lm -ldl -llua #-l$(NAMESPACE)$(VERSION) 
COMPLAIN = -Wno-unused
ABC = `locate llvm-symbolizer | grep /usr | head -n 1`
#ABC = llvm-symbolizer
INIT_ASAN = ASAN_SYMBOLIZER_PATH=$(ABC)

#CFlags get very complicated very easily
#All defines go here
DFLAGS = -DNW_VERBOSE
PICTYPE=-fPIC
#Clang compilation flags
CLANGFLAGS = -g $(PICTYPE) -Wall -Werror -std=c99 $(COMPLAIN) -fsanitize=address -fsanitize-undefined-trap-on-error $(DFLAGS)
CLANGSHARED = $(LDFLAGS) $(LDDIRS)
#GCC compilation flags
#GCCFLAGS = -g $(PICTYPE) -Wall -Werror $(COMPLAIN) -Wstrict-overflow -ansi -std=c99 -Wno-deprecated-declarations -Xlinker --verbose -O0 $(DFLAGS)
GCCFLAGS = -g $(PICTYPE) -Wall -Werror $(COMPLAIN) -Wstrict-overflow -ansi -std=c99 -Wno-deprecated-declarations -O0 $(DFLAGS)
GCCSHARED = $(LDDIRS) $(LDFLAGS)

#ifdef LINUX
#Linux / BSD targets will most likely have no trouble with the following options
#SHARED = $(CLANGSHARED)
#CC = clang
#CFLAGS = $(CLANGFLAGS)
#And here's one for those who prefer GCC
#SHARED = $(GCCSHARED)
#CC = gcc
#CFLAGS = $(GCCFLAGS)
#endif

#ifdef CYGWIN
#Cygwin needs a couple of adjustments
PICTYPE=-fpic
GCCFLAGS = -g $(PICTYPE) -Wall -Werror $(COMPLAIN) -Wstrict-overflow -ansi -std=c99 -Wno-deprecated-declarations -O0 $(DFLAGS)
#SHARED = $(GCCSHARED)
CC = gcc
CFLAGS = $(GCCFLAGS)
#endif

#ifdef WIN32
#Windows..., oh boy... Windows...
#SHARED = $(GCCSHARED)
#CC = gcc
#CFLAGS = $(GCCFLAGS)
#endif

#if 0
#These are all development flags that will be pulled out later
#RUNARGS = $(INIT_ASAN) LD_LIBRARY_PATH=.
#RUNARGS = LD_LIBRARY_PATH=. ./$(BIN_CONNECT) -s "blablabla.com" 
#RUNARGS = LD_LIBRARY_PATH=. ./obsrv --dir $(WWWDIR) --check-backend Lua
#RUNARGS = $(INIT_ASAN) LD_LIBRARY_PATH=.:/usr/local/lib ./obsrv --dir $(WWWDIR) --check-backend Lua
#RUNARGS = $(INIT_ASAN) LD_LIBRARY_PATH=. ./$(BIN_RENDER) --database tests/test.db --sql --query "SELECT * FROM content LIMIT 10" -m tests/$(TEST).sql -v tests/$(TEST).html --dump-table #2>&1 | less
#RUNARGS = $(INIT_ASAN) LD_LIBRARY_PATH=. ./$(BIN_RENDER) --json -m tests/$(TEST).json -v tests/$(TEST).html #--dump-table #2>&1 | less
WWWDIR=`realpath ../obsidian-www`
VERSION = 0.2-rc1
NAMESPACE = obsidian
LIB = lib$(NAMESPACE)$(VERSION).$(DYLIB)
TEST = sql
RUNARGS = $(INIT_ASAN) LD_LIBRARY_PATH=. ./obsrv --start --port 2000 --no-daemon --dir $(WWWDIR) 
IGNORE = archive tests
IGNCLEAN = "sqlite3.o"
ARCHIVEDIR = ..
ARCHIVEFMT = gz
ARCHIVEFILE = $(NAME).`date +%F`.`date +%H.%M.%S`.tar.${ARCHIVEFMT}
PKGDIR = $(NAME).$(VERSION)
PKGFILE = $(ARCHIVEDIR)/$(NAME).$(VERSION).tar.${ARCHIVEFMT}
SYNCDIR = space/archives/private/$(NAME)
DB=gdb
DBFLAGS=-ex run --args 
LC=valgrind
LCFLAGS=--leak-check=full --log-fd=3
#endif


#Phony targets 
.PHONY: main clean debug leak run other


#Primary target
main: build 
main:
	@printf ''>/dev/null	

#Make everything
all: build
all: test
all: $(NAMESPACE)-render
all: $(NAMESPACE)-example
all: run
all:
	@printf ''>/dev/null	


#Build a shared object	
#build: $(NAMESPACE)-shared
#build: $(NAMESPACE)-main
#build: $(NAMESPACE)-render
build: etcetc
build:
	@printf ''>/dev/null	

etcetc: single.o
	$(CC) $(CFLAGS) -o su single-main.c

#Static build
static: CFLAGS = -Wall -Werror -std=c99 $(COMPLAIN) -fsanitize=address -fsanitize-undefined-trap-on-error
static: $(OBJ)
static:
	ar rcs lib$(NAMESPACE).a $(OBJ)
 
#This will update header paths with the correct ones 
update:
	@find . -maxdepth 1 -type f -name "*.c" ! -name "main.c" | \
		xargs sed -i '/obsidian/! { /\.h/  { s|#include "|#include "obsidian/| }}'


#...
tests.o: CFLAGS=-g -Wall -Werror -Wno-unused $(PICTYPE) -std=c99 -DOBS_LOCAL_SQLITE3
tests.o:
	@echo $(CC) $(CFLAGS) -c tests.c
	@$(CC) $(CFLAGS) -c tests.c


json-test: TEST=lil
json-test:	
	@$(INIT_ASAN) LD_LIBRARY_PATH=. ./$(BIN_RENDER) --json -m tests/$(TEST).json -v tests/$(TEST).html


sql-test: TEST=sql
sql-test:
	@$(INIT_ASAN) LD_LIBRARY_PATH=. ./$(BIN_RENDER) --database tests/test.db --sql --query "SELECT * FROM content LIMIT 10" -m tests/$(TEST).sql -v tests/$(TEST).html --dump-table

 
#Application library (example 1)
#app: CFLAGS=-g -Wall -Werror -Wno-unused -fPIC -std=c99 -shared -DOBS_LOCAL_SQLITE3
app: CFLAGS =-g -Wall -Werror -Wno-unused $(PICTYPE) -std=c99 -shared -DOBS_LOCAL_SQLITE3
app: CC = gcc
app:
	@echo $(CC) $(CFLAGS) -o libwebapp.so webapp.c
	@$(CC) -o libwebapp.so $(CFLAGS) webapp.c
	@mv libwebapp.so $(WWWDIR)/


#Application build
appbuild: CFLAGS=-g -Wall -Werror -Wno-unused $(PICTYPE) -std=c99 -shared
appbuild: CC = gcc
appbuild: BASENAME = ${APPNAME:.c=}
appbuild:
	-@test ! -z $(LIB) || echo "Error - Obsidian does not exist, run make first."
	@test ! -z $(LIB)
	-@test ! -z $(APPNAME) || echo "Error - No application name specified."
	@test ! -z $(APPNAME)
	@echo $(CC) $(CFLAGS) -o $(BASENAME).$(DYLIB) $(APPNAME)
	@$(CC) -o $(BASENAME).$(DYLIB) $(CFLAGS) $(APPNAME)
	mv $(BASENAME).$(DYLIB) wwwtests/twiddles/


#Build obsidian's server management tool 
#$(NAMESPACE)-main: CFLAGS=-std=c99 -L. -lm -lobsidian -ldl -g -Wall -Werror -Wno-unused -DOBS_LOCAL_SQLITE3 -DOBSIDIAN_TESTS
$(NAMESPACE)-main: CFLAGS += $(SHARED) -l$(NAMESPACE)$(VERSION) -llua -DOBS_LOCAL_SQLITE3 -DOBSIDIAN_TESTS
$(NAMESPACE)-main:
	@echo $(CC) $(CFLAGS) -o $(BIN) main.c
	@$(CC) $(CFLAGS) -o $(BIN) main.c



#Build obsidian's render testing tool
$(NAMESPACE)-render: CFLAGS += $(SHARED) -l$(NAMESPACE)$(VERSION) -DOBS_LOCAL_SQLITE3 -DOBSIDIAN_TESTS
$(NAMESPACE)-render:
	@echo $(CC) $(CFLAGS) -o $(BIN_RENDER) main-render.c
	@$(CC) $(CFLAGS) -o $(BIN_RENDER) main-render.c


#Build obsidian's connect() testing tool
# laxtls 
$(NAMESPACE)-connect: CFLAGS += $(SHARED) -l$(NAMESPACE)$(VERSION) -DOBS_LOCAL_SQLITE3 -DOBSIDIAN_TESTS
$(NAMESPACE)-connect:
	@echo $(CC) $(CFLAGS) -o $(BIN_CONNECT) main-connect.c
	@$(CC) $(CFLAGS) -o $(BIN_CONNECT) main-connect.c




#Build some example applications on top of Obsidian 
$(NAMESPACE)-example: CFLAGS=-std=c99 $(PICTYPE) $(LDDIRS) -l$(NAMESPACE)$(VERSION) -g -Wall -Werror -Wno-unused -shared -DOBS_LOCAL_SQLITE3
$(NAMESPACE)-example: APPNAME=examples/simple.c
$(NAMESPACE)-example: appbuild
	@printf '' > /dev/null


#Build obsidian shared library
#$(NAMESPACE)-shared: CFLAGS=-g -Wall -Werror -Wno-unused -fPIC -std=c99 -DOBS_LOCAL_SQLITE3 -c
$(NAMESPACE)-shared: $(OBJ)
	@echo $(CC) $(CFLAGS) -shared $(OBJ) -L/usr/local -llua -o $(LIB)
	@$(CC) $(CFLAGS) -shared $(OBJ) -L/usr/local -llua -o $(LIB)



#Duktape
#@$(CC) $(CFLAGS) -shared $(OBJ) -o $(LIB)
duktape.o: CFLAGS = $(PICTYPE) -std=c99 -Ivendor
duktape.o:
	@echo ${CC} -c vendor/duktape.c ${CFLAGS}
	@${CC} -c vendor/duktape.c ${CFLAGS}


#SQLITE
sqrooge.o: CFLAGS += $(PICTYPE) -DSQ_VERBOSE
sqrooge.o:
	@echo $(CC) $(CFLAGS) -c sqrooge.c 
	@$(CC) $(CFLAGS) -c sqrooge.c 

xxxxxx.o:
	@echo $(CC) $(CFLAGS) -c main-render.c 
	@$(CC) $(CFLAGS) -c main-render.c 

#SQLITE
sqlite3.o: CFLAGS = $(PICTYPE) -std=c99 -DSQLITE_THREADSAFE=0
sqlite3.o:
	@echo $(CC) $(CFLAGS) -c vendor/sqlite3.c 
	@$(CC) $(CFLAGS) -c vendor/sqlite3.c 


#Objects
.c.o:
	@echo $(CC) $(CFLAGS) -c $<
	@$(CC) $(CFLAGS) -c $<


#Install (the newest version can be a symbolic link)
install:
	-test -d $(PREFIX)/include/$(NAMESPACE)/ || mkdir -p $(PREFIX)/include/$(NAMESPACE)/
	@test -d $(PREFIX)/include/$(NAMESPACE)/ || echo "Failed to create $(PREFIX)/include/$(NAMESPACE)/"
	@test -d $(PREFIX)/include/$(NAMESPACE)/
	-test -d $(PREFIX)/lib/$(NAMESPACE)/ || mkdir -p $(PREFIX)/lib/$(NAMESPACE)/
	@test -d $(PREFIX)/lib/$(NAMESPACE)/ || echo "Failed to create $(PREFIX)/lib/$(NAMESPACE)/"
	@test -d $(PREFIX)/lib/$(NAMESPACE)/
	printf "%s: %s\n" "docroot" $(WWWDIR) > default.conf
	cp default.conf /etc/obsidian.conf
	cp $(NAMESPACE).h $(PREFIX)/include/
	cp -r $(NAMESPACE)/ $(PREFIX)/include/
	cp $(LIB) $(PREFIX)/lib/
	-@test -L $(PREFIX)/lib/lib$(NAMESPACE).$(DYLIB) && rm $(PREFIX)/lib/lib$(NAMESPACE).$(DYLIB)
	ln -s $(PREFIX)/lib/$(LIB) $(PREFIX)/lib/lib$(NAMESPACE).$(DYLIB)
	cp $(BIN) $(BIN_RENDER) $(BIN_CONNECT) $(PREFIX)/bin/


#Uninstall if you don't like it
uninstall:
	rm -f $(PREFIX)/include/$(NAMESPACE).h
	rm -r $(PREFIX)/include/$(NAMESPACE)/
	rm -r $(PREFIX)/lib/$(NAMESPACE)/
	rm -f $(PREFIX)/lib/$(LIB) $(PREFIX)/lib/lib$(NAMESPACE).$(DYLIB)
	rm -f $(PREFIX)/bin/$(BIN)


#Reinstall
reinstall: uninstall
reinstall: main
reinstall: install
reinstall:
	@printf '' >/dev/null

#Test uninstall process
test-uninstall:
	@echo rm -r $(PREFIX)/include/$(NAMESPACE)
	@echo rm -r $(PREFIX)/lib/$(NAMESPACE)
	@echo rm $(PREFIX)/lib/$(LIB) $(PREFIX)/lib/lib$(NAMESPACE).$(DYLIB)


#clean
clean:
	-@find . -maxdepth 1 -type f -iname "*.o" -o -iname "*.so" \
		`echo $(IGNCLEAN) | sed '{ s/ / ! -iname /g; s/^/! -iname /; }'` | \
		sed '/sqlite3.o/d' | xargs rm 
	-@rm $(BIN) $(BIN_RENDER) $(BIN_CONNECT) $(BINTEST) 2>/dev/null


#Really clean
veryclean:
	-@rm -f sqlite3.o
	-@rm -rf wwwtests
	-@find . -type f -iname "*.o" -o -iname "*.$(DYLIB)" -o -iname ".*.swp" | \
		xargs rm	
	-@rm $(BIN) $(BINTEST) 2>/dev/null


#if 0
#Run
run:
	$(RUNARGS)


#Run args always run this
debug-run:
	@-rm twiddly.db
	@for i in `find tests -maxdepth 1 -type d ! -name "apart-from-war" -a ! -name "sink" -a ! -name "tests"`; \
		do ./obsarchiver -a $$i; done
	@echo 'site list'
	@sqlite3 twiddly.db 'select * from sitelist;'
	@$(RUNARGS)


#I'm having some issues with keeping tests here
tests:
	@mkdir -p tests/{ds9,ford-fiestas,tbbucs}.lo


#Debug
debug: build 
debug:
	@echo LD_LIBRARY_PATH=. $(DB) $(DBFLAGS) ./$(BIN) --no-daemon --start -p 2000
	@LD_LIBRARY_PATH=. $(DB) $(DBFLAGS) ./$(BIN) --no-daemon --start -p 2000


#Add flags here, b/c the server should free all resources...
leak: CFLAGS += -DNW_BEATDOWN_POST
leak: CFLAGS += -DNW_BEATDOWN_MODE
leak: clean 
leak: build 
leak:
	@echo $(LC) $(LCFLAGS) $(RUNARGS) 3>etc
	@$(LC) $(LCFLAGS) $(RUNARGS) 3>etc


#Save permissions
permissions:
	@find | grep -v './tools' | grep -v './examples' | grep -v './.git' | sed '1d' | xargs stat -c 'chmod %a %n' > PERMISSIONS


#Restore permissions
restore-permissions:
	chmod 744 PERMISSIONS && ./PERMISSIONS && chmod 644 PERMISSIONS


# Make a tarball that goes to another directory
backup: veryclean
backup:
	@-rm -f sqlite3.o
	@echo tar chzf $(ARCHIVEDIR)/${ARCHIVEFILE} --exclude-backups \
		`echo $(IGNORE) | sed '{ s/^/--exclude=/; s/ / --exclude=/g; }'` ./$(WILDCARD)
	@tar chzf $(ARCHIVEDIR)/${ARCHIVEFILE} --exclude-backups \
		`echo $(IGNORE) | sed '{ s/^/--exclude=/; s/ / --exclude=/g; }'` ./$(WILDCARD)


# Make an archive tarball
archive: ARCHIVEDIR = archive
archive: backup


# Make a package
pkg-base: ARCHIVEDIR = archive
pkg-base: veryclean
pkg-base:
	-@rm -f sqlite3.o
	@mkdir $(PKGDIR)/
	@cp *.c obsidian.h README.md CHANGELOG $(PKGDIR)/
	@cp -r examples $(PKGDIR)/
	@cp -r obsidian $(PKGDIR)/
	echo sed "{ s/#[^if,^endif].$(WILDCARD)//; }" Makefile cpp ${PKGDEBUG:2>/dev/null} sed "{ /^#/d; s/^ /\t/ }" > $(PKGDIR)/Makefile
	@sed "{ s/#[^if,^endif].$(WILDCARD)//; }" Makefile | \
		cpp ${PKGDEBUG:-2>/dev/null} | \
		sed "{ /^#/d; s/^ /\t/ }" > $(PKGDIR)/Makefile
	@echo tar chzf $(PKGFILE) $(PKGDIR)
	@tar chzf $(PKGFILE) $(PKGDIR)
	@rm -rf $(PKGDIR)


# Make a package with SQLite included
pkg-sqlite: pkg-base
	@printf '' >/dev/null

# Debug package creation in case anything funny ends up in the Makefile
pkg-debug: PKGDEBUG = 2>/dev/stderr
pkg-debug: pkg-base
	@printf '' >/dev/null

# Make a "thin" package
pkg: PKGDEBUG = 2>/dev/null
pkg: pkg-base
	@printf '' >/dev/null

# Pull / sync
pull:
	@VAR=`ssh $(HOST) 'ls ~/$(SYNCDIR)/$(NAME)$(WILDCARD) | tail -n 1'`; \
	 ssh $(HOST) "cat $$VAR" | tee archive/`basename $$VAR` | tar xzf -


# Push 
push: ARCHIVEDIR = archive
push:
	@-rm -f sqlite3.o
	@VAR=$(ARCHIVEFILE); \
	tar chzf $(ARCHIVEDIR)/$$VAR --exclude-backups \
		`echo $(IGNORE) | sed '{ s/^/--exclude=/; s/ / --exclude=/g; }'` ./$(WILDCARD) ; \
	ssh $(HOST) 'mkdir ~/$(SYNCDIR)/'; \
	scp $(ARCHIVEDIR)/$$VAR $(HOST):~/$(SYNCDIR)/


# Create a changelog
changelog:
	@echo "Creating / updating CHANGELOG document..."
	@touch CHANGELOG


# Notate a change (Target should work on all *nix and BSD)
change:
	@test -f CHANGELOG || printf "No changelog exists.  Use 'make changelog' first.\n\n"
	@test -f CHANGELOG
	@echo "Press [Ctrl-D] to save this file."
	@cat > CHANGELOG.USER
	@date > CHANGELOG.ACTIVE
	@sed 's/^/\t -/' CHANGELOG.USER >> CHANGELOG.ACTIVE
	@printf "\n" >> CHANGELOG.ACTIVE
	@cat CHANGELOG.ACTIVE CHANGELOG > CHANGELOG.NEW
	@rm CHANGELOG.ACTIVE CHANGELOG.USER
	@mv CHANGELOG.NEW CHANGELOG
#endif
