# makefile for StackCPU src directory tree
#
# simply type 'make' to make everything
#
#
#

VERSION = 0.1.1

DATE = `date +%D-%T`

DIRS = simulator tools 



#
# make all programs
#

default: all

distro: tar

# build all executable files

all:
	for dir in $(DIRS);  do               \
	   cd $$dir ; make ; cd ..;     \
	done;


# build and install all executable files

install:
	for dir in $(DIRS);  do               \
	   cd $$dir ; make install ; cd ..;     \
	done;


# remove all *.o and backup files

clean:
	for dir in $(DIRS);  do               \
	   cd $$dir ; make clean ; cd ..;     \
	done;


# remove all *.o, executables, and library files

realclean: clean
	for dir in $(DIRS);  do               \
	   cd $$dir ; make realclean ; cd ..; \
	done;
	rm -f bin/*.exe
	rm -f *~

distclean: realclean


#
# make distribution file
#

PKG_HOME = src
PKG_NAME = stackcpu-$(VERSION)

tar: newversion
	cp /dev/null ../$(PKG_NAME).txt
	cd .. ; \
	tar -cvf $(PKG_NAME).tar $(PKG_HOME) ; \
	gzip $(PKG_NAME).tar ; \
	mv $(PKG_NAME).tar.gz $(PKG_NAME).tgz
	-@echo "Distribution is ../$(PKG_NAME).tgz (also see ../$(PKG_NAME).txt)"
	-@echo "Distribution is ../$(PKG_NAME).tgz" >> ../$(PKG_NAME).txt
	-@echo "-----------------------------------" >> ../$(PKG_NAME).txt
	-@cat Version >> ../$(PKG_NAME).txt
	-@echo "-----------------------------------" >> ../$(PKG_NAME).txt

newversion:
	cp /dev/null Version
	-@echo "VERSION = $(VERSION)" >> Version
	-@echo "DATE    = $(DATE)" >> Version


