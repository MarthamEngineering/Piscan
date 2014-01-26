###################################################################################################
## Very generic Makefile:
## It generates the object files from all cpp or cc files in a directory and links them together.
## You only have to supply it with your TARGET, LIBS, FLAGS, PATHS etc.
###################################################################################################

# ---> WHICH ENDINGS DO THE SOURCE FILES HAVE ?

PROJECT       := ScanDraiD
ENDING_SOURCE := cpp
ENDING_HEADER := hpp
LOCALEDIR     := i18n
LANG          := de_DE
DEVDOCDIR     := ../doc/developer

# ---> Is the target a shared lib, a static lib or a executable ?
TARGETTYPE      := executable
#TARGETTYPE      := sharedlib
#TARGETTYPE      := staticlib

# ---> The name of the shared lib, static lib or executable:
TARGETNAME      := ${PROJECT}.exe
#TARGETNAME      := libMyLib.so
#TARGETNAME      := libMyLib.a
# ---> Compileroptions
CC              := gcc
CXX             := g++
CFLAGS          := -pipe -mcpu=i686 -march=i686 -O3 -fomit-frame-pointer -Wall -W -g -DDEBUG -D_REENTRANT
#CXXFLAGS        := -pipe -mcpu=i686 -march=i686 -O3 -fno-rtti -Wall -W -g -DDEBUG -D_REENTRANT
CXXFLAGS        := -pipe -O3 -fno-rtti -Wall -W -g -D_REENTRANT -DPATH2CONF=\"ScanDraiD.conf\"  -DPACKAGE=\"${PROJECT}\" -DLOCALEDIR=\"${LOCALEDIR}\" #-DDEBUG
INCPATH         := -I/home/awi/usr/include/jpeg-6b
# ---> Linkeroptions
LINK            := g++
LFLAGS          :=  # -fPIC
LIBS            := -ljpeg # -lintl #-lpthread -ldl
LIBPATH         := -L/home/awi/usr/lib64/
# ---> Options for a shared library
SHARE_LIB_FLAG  := -shared -W1,-soname,
SHARE_COMP_FLAG := -fPIC
SHARE_LINK_FLAG := -rdynamic
MAJOR           := 1
MINOR           := 0
PATCH           := 0
# ---> Dependency options
DEPS            := Makefile.dep
DEP_FLAG        := -MM
# ---> Generates object files of all *.c, *.cc, *.cpp, *.h etc. files thar are found in directory
HEADERS         := $(wildcard *.$(ENDING_HEADER))
SOURCES         := $(wildcard *.$(ENDING_SOURCE))
OBJECTS         := $(patsubst %.$(ENDING_SOURCE),%.o,$(SOURCES))
# --> Tools
LN              := ln -s
TAR             := tar -cf
GZIP            := gzip -9f
BZIP            := bzip2
RM              := rm -rvf
TAGS            := ctags --extra=+q
CP              := cp -vf
LDCONFIG        := /sbin/ldconfig
RANLIB          := ranlib
AR              := ar crv

# -------------------> Do not edit <-------------------- #

ifeq ($(TARGETTYPE),sharedlib)
	LFLAGS    := $(LFAGS) $(SHARE_COMP_FLAG) $(SHARE_LINK_FLAG)
	CXXFLAGS  := $(CXXFLAGS) $(SHARE_COMP_FLAG)
	CCFLAGS   := $(CCFLAGS) $(SHARE_COMP_FLAG)
endif

# ---> Implicit rules

.SUFFIXES: .cpp .cxx .cc .C .c

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
.C.o:
	$(CC) -c  $(CCFLAGS) $(INCPATH)  -o $@ $<
.c.o:
	$(CC) -c  $(CFLAGS) $(INCPATH)   -o $@ $<

# ---> Build rules

.PHONY : all depend objects clean proper world staticlib sharedlib executable


all : $(TARGETTYPE)

executable: $(OBJECTS)
	$(LINK) $(LFLAGS) -o $(TARGETNAME) $(OBJECTS) $(LIBPATH) $(LIBS)

sharedlib: $(OBJECTS)
	$(LINK) $(LFLAGS) $(SHARE_LIB_FLAG)$(TARGETNAME) -o $(TARGETNAME).$(MAJOR).$(MINOR).$(PATCH) $(OBJECTS) $(LIBPATH) $(LIBS)
	$(LN) $(TARGETNAME).$(MAJOR).$(MINOR).$(PATCH) $(TARGETNAME)
	#$(CP) $(TARGETNAME)* /usr/lib/
	#$(LDCONFIG)

staticlib: $(OBJECTS)
	$(AR) $(TARGETNAME) $(OBJECTS)
	$(RANLIB) $(TARGETNAME)

depend:
	$(CXX) ${INCPATH} $(DEP_FLAG) $(SOURCES) > $(DEPS)

objects: $(OBJECTS)

clean:
	$(RM) $(TARGETNAME)*
	$(RM) $(OBJECTS)
	$(RM) *.rpo
	$(RM) *.d
	$(RM) *~core

proper: clean
	$(RM) *.a
	$(RM) *.bak
	$(RM) *.old
	$(RM) $(DEPS)
	$(RM) tags
	$(RM) *~
	$(RM) ${LOCALEDIR}/${PROJECT}.pot
	$(RM) ${LOCALEDIR}/${LANG}
	$(RM) ${DEVDOCDIR}/${PROJECT}

tags: $(HEADERS) $(SOURCES)
	$(TAGS) $(HEADERS) $(SOURCES)

pot: $(SOURCES)
	xgettext -d ${PROJECT} -s -o ${LOCALEDIR}/${PROJECT}.pot $(SOURCES)

po: pot
	cd ${LOCALEDIR} &&  msgmerge -s -U ${LANG}.po ${PROJECT}.pot && mkdir -p ${LANG}/LC_MESSAGES && msgfmt -c -v -o ${LANG}/LC_MESSAGES/${PROJECT}.mo ${LANG}.po

doc: $(HEADERS)
	cd ${DEVDOCDIR} && doxygen Doxyfile


world: proper tags depend all po doc

-include $(DEPS)
