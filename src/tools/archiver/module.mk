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

CP = cp
MV = mv
RM = rm -f


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
EXTRA_DEBUG 	= -W -Wall -ggdb -O2
#EXTRA_DEBUG 	= -g

EXTRA_DEFINES   = 

MOD_CFLAGS 	= $(EXTRA_DEBUG) $(EXTRA_DEFINES)

MOD_CPPFLAGS 	= $(EXTRA_DEBUG) $(EXTRA_DEFINES) -Wno-deprecated
#MOD_CPPFLAGS 	= $(EXTRA_DEBUG) $(EXTRA_DEFINES) -fexceptions -lang-c++


#------------------------------------------------------------------------
# Define local library variables
#------------------------------------------------------------------------

#MOD_LDFLAGS 	= -lang-c++ -static
#MOD_LDFLAGS 	= -static
MOD_LDFLAGS 	= 

MOD_LIB_PATHS 	= -L/usr/local/lib

MOD_LIBS 		= -lm

MOD_ARFLAGS 	= -rc



