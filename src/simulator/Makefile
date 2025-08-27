# makefile for stacksim program
#

#------------------------------------------------------------------------
# Include system make include files
#------------------------------------------------------------------------

MODROOT	= ../simulator
include $(MODROOT)/module.mk
##include depend.mk


VERSION=0.1.0


#
# --------------------------------------------------------------------
# you shouldn't need to change anything below this line

PLATFILES = debug.xxx opCodes.xxx

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

ide:
	codeblocks audio.cbp &

#------------------------------------------------------------------------

run: test

test: test-host

test-host: all
	$(TARGET) -m 1
	
unit: test-unit

test-unit: all
	$(TARGET)
	$(TARGET) -h 
	$(TARGET) -v
	$(TARGET) -m 1
	$(TARGET) -m 2 -f memory.bin
	$(TARGET) -f memory.bin -m 2
	$(TARGET) -m 3
	$(TARGET) -m 77
	$(TARGET) -f fail.bin
	$(TARGET) -z

#------------------------------------------------------------------------

install: all
	$(CP) $(TARGET) $(INSTALL_DIR)

clean:
	$(RM) *.o *% *~ *.bak depend.mk

realclean: clean
	$(RM) $(TARGET) $(TARGET).exe 
	$(RM) *.gcov gmon.out *.gcno *.gcda *.perf callgrind.out.* *.log
	$(RM) -rf bin lib obj

distclean: realclean

depend:
	makedepend $(DEP_CPPFLAGS) $(CFILES)

depend.mk:
	$(CPP) -MM $(DEP_CPPFLAGS) $(CFILES) > $@

lint: $(CFILES)
	$(LINT) -preproc $(MOD_INCLUDES) -I$(ENV_PREFIX)/usr/include $<

.PHONY : exe

#------------------------------------------------------------------------
# make distribution file
#

PKG_HOME = simulator
PKG_NAME = simulator-$(VERSION)

tar: distclean
	cd .. ; \
	$(TAR) -cvf $(PKG_NAME).tar $(PKG_HOME) ; \
	$(GZIP) $(PKG_NAME).tar ; \
	$(MV) $(PKG_NAME).tar.gz $(PKG_NAME).tgz
	-@echo "Distribution package is ../$(PKG_NAME).tgz" 

#------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- make depend depends on it.

