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
WWWDIR=`realpath ../obsidian-www`
VERSION = 0.2-rc1
NAMESPACE = obsidian
LIB = lib$(NAMESPACE)$(VERSION).$(DYLIB)
TEST = sql
RUNARGS = $(INIT_ASAN) LD_LIBRARY_PATH=. ./obsrv --start --port 2000 --no-daemon --dir $(WWWDIR) 
IGNORE = archive src tests
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

#Build a shared object	
build: single.o
build: tests
	@printf ''>/dev/null	

#Tests
tests: single.o tests/harness.o vendor/sqlite3.o
	$(CC) $(CFLAGS) single.o harness.o sqlite3.o -o single-test -lm -lpthread -ldl
	
#Sqlite3
sqlite3.o: CFLAGS = $(PICTYPE) -std=c99 -DSQLITE_THREADSAFE=0
sqlite3.o:
	@echo $(CC) $(CFLAGS) -c vendor/sqlite3.c 
	@$(CC) $(CFLAGS) -c vendor/sqlite3.c 


#Objects
.c.o:
	@echo $(CC) $(CFLAGS) -c $<
	@$(CC) $(CFLAGS) -c $<


#clean
clean:
	-@find . -maxdepth 1 -type f -iname "*.o" -o -iname "*.so" \
		`echo $(IGNCLEAN) | sed '{ s/ / ! -iname /g; s/^/! -iname /; }'` | \
		sed '/sqlite3.o/d' | xargs rm 
	-@rm $(BIN) single-test 2>/dev/null


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


#Add flags here, b/c the server should free all resources...
leak: CFLAGS += -DNW_BEATDOWN_POST
leak: CFLAGS += -DNW_BEATDOWN_MODE
leak: clean 
leak: build 
leak:
	@echo $(LC) $(LCFLAGS) $(RUNARGS) 3>etc
	@$(LC) $(LCFLAGS) $(RUNARGS) 3>etc


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
