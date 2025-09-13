# makefile for StackCPU top level tree
#
# simply type 'make' to make everything
#
#
#

VERSION = 0.3.1

DATE = `date +%D-%T`

DIRS = src 



#
# make all programs
#

default: all

release: distro

distro: install realclean tar

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
	rm -f *~

distclean: realclean
	for dir in $(DIRS);  do               \
	   cd $$dir ; make distclean ; cd ..; \
	done;
	rm -f *~
	rm -f Version Version.txt


#
# make distribution file
#

PKG_HOME = StackCPU
PKG_NAME = stackcpu-$(VERSION)

tar: newversion
	cp /dev/null ../$(PKG_NAME).txt
	cd .. ; \
	tar -cvf $(PKG_NAME).tar $(PKG_HOME)/CHANGES.txt ; \
	tar -rvf $(PKG_NAME).tar $(PKG_HOME)/BUILD.txt ; \
	tar -rvf $(PKG_NAME).tar $(PKG_HOME)/LICENSE ; \
	tar -rvf $(PKG_NAME).tar $(PKG_HOME)/Makefile ; \
	tar -rvf $(PKG_NAME).tar $(PKG_HOME)/README.md ; \
	tar -rvf $(PKG_NAME).tar $(PKG_HOME)/StackCPU_README.pdf ; \
	tar -rvf $(PKG_NAME).tar $(PKG_HOME)/TODO.txt ; \
	tar -rvf $(PKG_NAME).tar $(PKG_HOME)/Version ; \
	tar -rvf $(PKG_NAME).tar $(PKG_HOME)/docs/ ; \
	tar -rvf $(PKG_NAME).tar $(PKG_HOME)/examples/ ; \
	tar -rvf $(PKG_NAME).tar $(PKG_HOME)/hw/ ; \
	tar -rvf $(PKG_NAME).tar $(PKG_HOME)/ref/ ; \
	tar -rvf $(PKG_NAME).tar $(PKG_HOME)/src/ ; \
	gzip $(PKG_NAME).tar ; \
	mv $(PKG_NAME).tar.gz $(PKG_NAME).tgz
	-@echo "Distribution is ../$(PKG_NAME).tgz (also see ../$(PKG_NAME).txt)"
	-@echo "Distribution is ../$(PKG_NAME).tgz" >> ../$(PKG_NAME).txt
	-@echo "-----------------------------------" >> ../$(PKG_NAME).txt
	-@cat Version >> ../$(PKG_NAME).txt
	-@echo "-----------------------------------" >> ../$(PKG_NAME).txt

newversion:
	cp /dev/null Version.txt
	-@echo "VERSION = $(VERSION)" >> Version.txt
	-@echo "DATE    = $(DATE)" >> Version.txt


