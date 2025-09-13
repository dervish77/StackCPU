#*
#*     Module Name:  module.mk
#*
#*
#*	Make file definitions for the this module.
#*
#*
#*/

#------------------------------------------------------------------------
# Include system make include files
#------------------------------------------------------------------------

PREFIX = /usr/bin/

#------------------------------------------------------------------------
# Define some tools
#------------------------------------------------------------------------

CPP   = $(PREFIX)g++
AR    = $(PREFIX)ar

STRIP = $(PREFIX)strip
LINT  = lint

TAR   = tar
GZIP  = gzip

CP    = cp
MV    = mv
RM    = rm -f

GPROF		= gprof
KPROF		= kprof
GCOV		= gcov

VALGRIND	= valgrind
VG_CALL 	= --tool=callgrind
VG_MEM		= --tool=memcheck --leak-check=yes -v

GPROF_OPT	= -pg
GCOV_OPT	= -fprofile-arcs -ftest-coverage

OPT		=
#OPT		= $(GPROF_OPT)
#OPT		= $(GCOV_OPT)
#OPT		= $(GPROF_OPT) $(GCOV_OPT)

#------------------------------------------------------------------------
# Define some extras
#------------------------------------------------------------------------

MOD_OUT_DIR	= .


#------------------------------------------------------------------------
# Define local includes
#------------------------------------------------------------------------
EXTRA_INCLUDE	= -I/usr/local/include 

MOD_INCLUDES	= -I.  $(EXTRA_INCLUDE)


#------------------------------------------------------------------------
# Define local cflags
#------------------------------------------------------------------------
EXTRA_DEBUG 	= -W -Wall -ggdb -O2 $(OPT)
#EXTRA_DEBUG 	= -g $(OPT)

EXTRA_DEFINES   = 

MOD_CFLAGS 	= $(EXTRA_DEBUG) $(EXTRA_DEFINES)

MOD_CPPFLAGS 	= $(EXTRA_DEBUG) $(EXTRA_DEFINES) -Wno-deprecated
#MOD_CPPFLAGS 	= $(EXTRA_DEBUG) $(EXTRA_DEFINES) -fexceptions -lang-c++


#------------------------------------------------------------------------
# Define local library variables
#------------------------------------------------------------------------

#MOD_LDFLAGS 	= -lang-c++ -static
#MOD_LDFLAGS 	= -static
MOD_LDFLAGS 	= $(OPT)

MOD_LIB_PATHS 	= -L/usr/local/lib

MOD_LIBS 		= -lm

MOD_ARFLAGS 	= -rc



