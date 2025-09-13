# makefile for stacksim program
#

#------------------------------------------------------------------------
# Include system make include files
#------------------------------------------------------------------------

MODROOT	= ../simulator
include $(MODROOT)/module.mk
##include depend.mk


VERSION=0.3.1


INSTALL_DIR = ../bin


#
# --------------------------------------------------------------------
# you shouldn't need to change anything below this line

PLATFILES = debug.xxx parse.xxx binUtils.xxx opCodes.xxx 

SIMFILES  = uiSim.xxx regArray.xxx coreSim.xxx memArray.xxx memSim.xxx

FILES	  = $(PLATFILES) $(SIMFILES) main.xxx

LIB_FILES = 

LIBRARY	= 

TARGET	= stacksim


#------------------------------------------------------------------------

INCLUDES 	=  

LIBLIST		= 

LIBS		= $(LIBLIST) $(MOD_LIBS)

#------------------------------------------------------------------------

# OPTIMIZE 	= 

DEP_CPPFLAGS	= $(CFLAGS) $(MOD_CPPFLAGS) $(MOD_INCLUDES)

ALL_CPPFLAGS 	= $(CFLAGS) $(MOD_CPPFLAGS) $(MOD_INCLUDES)

ALL_LDFLAGS	= $(LDFLAGS) $(MOD_LDFLAGS) $(MOD_LIB_PATHS) -L$(MODROOT)

ALL_ARFLAGS	= $(MOD_ARFLAGS)

OFILES	   = $(FILES:xxx=o)
CFILES	   = $(FILES:xxx=cpp)
HFILES	   = $(FILES:xxx=h)
LINTFILES  = $(FILES:xxx=lint)

LIB_OFILES = $(LIB_FILES:.xxx=.o)
LIB_CFILES = $(LIB_FILES:.xxx=.cpp)


#
# make executables
#

default: all

#all exe : depend.mk $(TARGET)
all exe : $(TARGET)

$(TARGET): $(OFILES) $(LIBLIST)
	$(CPP) $(ALL_LDFLAGS) -o $(TARGET) $(OFILES) $(LIBS)

$(OFILES): %.o: %.cpp *.h
	$(CPP) $(OPTIMIZE) $(ALL_CPPFLAGS) -c $< 

$(LIB_OFILES): %.o: %.cpp *.h
	$(CPP) $(OPTIMIZE) $(ALL_CPPFLAGS) -c $< 
	$(AR) $(ALL_ARFLAGS) $(LIBRARY) $*.o
#	$(RM) $*.o

#------------------------------------------------------------------------

NPP = /cygdrive/c/Utils/Notepad++/notepad++.exe

ide: ide-npp

ide-atom:
	atom . &

ide-cb:
	codeblocks $(TARGET).cbp &

ide-vsc:
	code -a . &
	
ide-npp:
	$(NPP) Makefile main.cpp uiSim.h uiSim.cpp memSim.h memSim.cpp coreSim.h coreSim.cpp opCodes.h opCodes.cpp &

#------------------------------------------------------------------------

run: test

test: test-host

test-host: all
	$(TARGET) -m 1 -f unit.bin

unit: test-unit test-code test-error

test-unit: all
	$(TARGET) -h 
	$(TARGET) -v
	$(TARGET) -m 0
	$(TARGET) -m 1 -u
	$(TARGET) -m 2 -f memory.bin -u
	$(TARGET) -f memory.bin -m 2 -u
	$(TARGET) -m 3 -u

test-code: all
	$(TARGET) -m 99 -f unit.bin -u

test-error: all
	$(TARGET) -m 77
	$(TARGET) -f fail.bin -u
	$(TARGET) -z

perf: all
	time $(TARGET) -m 2 -f memory.bin -u > $(TARGET).perf

#------------------------------------------------------------------------

gcov-run: unit
	$(GCOV) binUtils.cpp
	$(GCOV) coreSim.cpp
	$(GCOV) debug.cpp
	$(GCOV) main.cpp
	$(GCOV) memArray.cpp
	$(GCOV) memsim.cpp
	$(GCOV) opCodes.cpp
	$(GCOV) parse.cpp
	$(GCOV) regArray.cpp
	$(GCOV) uisim.cpp

GCOV_REPORT = $(TARGET).gcov

gcov: gcov-run
	-@echo "COVERAGE REPORT" > $(GCOV_REPORT)	
	cat binUtils.cpp.gcov >> $(GCOV_REPORT)
	cat coreSim.cpp.gcov >> $(GCOV_REPORT)
	cat debug.cpp.gcov >> $(GCOV_REPORT)
	cat main.cpp.gcov >> $(GCOV_REPORT)
	cat memArray.cpp.gcov >> $(GCOV_REPORT)
	cat memSim.cpp.gcov >> $(GCOV_REPORT)
	cat opCodes.cpp.gcov >> $(GCOV_REPORT)
	cat parse.cpp.gcov >> $(GCOV_REPORT)
	cat regArray.cpp.gcov >> $(GCOV_REPORT)
	cat uiSim.cpp.gcov >> $(GCOV_REPORT)
	-@echo "END REPORT" >> $(GCOV_REPORT)

coverage: distclean gcov
	./coverage.sh $(GCOV_REPORT)

#------------------------------------------------------------------------

prof-test: all
	$(TARGET) -m 0
	$(TARGET) -m 1 -u
	$(TARGET) -m 2 -f memory.bin -u
	$(TARGET) -f memory.bin -m 2 -u
	$(TARGET) -m 3 -u
	$(TARGET) -m 99 -f unit.bin -u

gprof: distclean prof-test
	$(GPROF) -b ./$(TARGET) 

kprof: distclean prof-test
	$(KPROF) -f ./$(TARGET) &

# #------------------------------------------------------------------------

valcall: all
	$(VALGRIND) $(VG_CALL) $(TARGET) -m 2 -f memory.bin -u
	kcachegrind callgrind.out.*

valmem: all
	$(VALGRIND) $(VG_MEM) $(TARGET) -m 2 -f memory.bin -u

#------------------------------------------------------------------------

install: all
	$(CP) $(TARGET) $(INSTALL_DIR)

clean:
	$(RM) *.o *% *~ *.bak depend.mk

realclean: clean
	$(RM) $(LIBRARY) $(TARGET) $(TARGET).exe 
	$(RM) *.gcov gmon.out *.gcno *.gcda *.perf callgrind.out.* *.log
	$(RM) -rf bin lib obj

distclean: realclean

depend:
	makedepend $(DEP_CPPFLAGS) $(CFILES)

depend.mk:
	$(CPP) -MM $(DEP_CPPFLAGS) $(CFILES) > $@

lint: $(CFILES)
	$(LINT) -preproc $(MOD_INCLUDES) -I$(ENV_PREFIX)/usr/include $<


#------------------------------------------------------------------------
# make distribution file
#

PKG_HOME = simulator
PKG_NAME = stacksim-$(VERSION)

tar: distclean
	cd .. ; \
	$(TAR) -cvf $(PKG_NAME).tar $(PKG_HOME) ; \
	$(GZIP) $(PKG_NAME).tar ; \
	$(MV) $(PKG_NAME).tar.gz $(PKG_NAME).tgz
	-@echo "Distribution package is ../$(PKG_NAME).tgz" 

.PHONY : exe

#------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- make depend depends on it.

