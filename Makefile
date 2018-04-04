# Makefile for single 
NAME = single
WILDCARD = *
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man
DFLAGS = -DNW_VERBOSE
PICTYPE=-fPIC
#CFLAGS = -g -Wall -Wno-unused -Werror -std=c99 $(COMPLAIN) -fsanitize=address -fsanitize-undefined-trap-on-error  -DSQLITE3_PATH="\"vendor/sqlite3.h\""
#CC=clang
CFLAGS = -g -Wall -Wno-unused -Werror -std=c99 $(COMPLAIN) -DSQLITE3_PATH="\"vendor/sqlite3.h\"" #-DDEBUG_H
CC=gcc

#if 0
# Profiling
INVOKE=time valgrind --tool=callgrind --dsymutil=yes

# Packaging
IGNORE = archive src tests
IGNCLEAN = "sqlite3.o"
ARCHIVEDIR = ..
ARCHIVEFMT = gz
ARCHIVEFILE = $(NAME).`date +%F`.`date +%H.%M.%S`.tar.${ARCHIVEFMT}
PKGDIR = $(NAME).$(VERSION)
PKGFILE = $(ARCHIVEDIR)/$(NAME).$(VERSION).tar.${ARCHIVEFMT}
#endif


#Phony targets 
.PHONY: main clean debug leak run other

#Primary target
main: build 
main:
	@printf ''>/dev/null	

#Create a new templating engine.
newte:
	gcc -DSQROOGE_H newt.c single.c -o newt

#Build a shared object	
build: single.o
build: tests
	@printf ''>/dev/null	

#Primary target
debug: CFLAGS += -DDEBUG_H
debug: single.o 
debug: tests
	@printf ''>/dev/null	

#Tests
tests: single.o tests/harness.o sqlite3.o
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
	-find . -maxdepth 1 -type f -iname "*.o" -o -iname "*.so" \
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
