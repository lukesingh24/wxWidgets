#!################################################################################
#! File:    unx.t
#! Purpose: tmake template file from which Makefile.in is generated by running
#!          tmake -t unx wxwin.pro -o Makefile.in
#! Author:  Vadim Zeitlin, Robert Roebling, Julian Smart
#! Created: 14.07.99
#! Version: $Id$
#!################################################################################

#${
    #! include the code which parses filelist.txt file and initializes
    #! %wxCommon, %wxGeneric, %wxHtml, %wxUnix and %wxGTK hashes.
    IncludeTemplate("filelist.t");

    #! Generic
    
    foreach $file (sort keys %wxGeneric) {
        #! native wxDirDlg can't be compiled due to GnuWin32/OLE limitations,
        #! so take the generic version
        if ( $wxGeneric{$file} =~ /\b(PS|G|U|16)\b/ ) {
            next unless $file =~ /^dirdlgg\./;
        }

        $file =~ s/cp?p?$/\o/;
        $project{"WXMSW_GENERICOBJS"} .= "src/generic/" . $file . " "
    }

    foreach $file (sort keys %wxGeneric) {
        next if $wxGeneric{$file} =~ /\bR\b/;

        $file =~ s/cp?p?$/\o/;
        $project{"WXGTK_GENERICOBJS"} .= "src/generic/" . $file . " "
    }

    foreach $file (sort keys %wxGeneric) {
        next if $wxCommon{$file} =~ /\bX\b/;

        $file =~ s/cp?p?$/\o/;
        $project{"WXMOTIF_GENERICOBJS"} .= "src/generic/" . $file . " "
    }

    #! Common
    
    foreach $file (sort keys %wxCommon) {
        next if $wxCommon{$file} =~ /\bR\b/;

        $file =~ s/cp?p?$/\o/;
        $project{"WXGTK_COMMONOBJS"} .= "src/common/" . $file . " "
    }

    foreach $file (sort keys %wxCommon) {
        next if $wxCommon{$file} =~ /\bX\b/;

        $file =~ s/cp?p?$/\o/;
        $project{"WXMOTIF_COMMONOBJS"} .= "src/common/" . $file . " "
    }

    foreach $file (sort keys %wxCommon) {
        next if $wxCommon{$file} =~ /\b(16)\b/;

        #! needs extra files (sql*.h) so not compiled by default.
        next if $file =~ /^odbc\./;

        $file =~ s/cp?p?$/\o/;
        $project{"WXMSW_COMMONOBJS"} .= "src/common/" . $file . " "
    }

    #! GUI
    
    foreach $file (sort keys %wxMSW) {
        #! Mingw32 doesn't have the OLE headers and has some troubles with
        #! socket code
        next if $wxMSW{$file} =~ /\b(O|16)\b/;

        #! native wxDirDlg can't be compiled due to GnuWin32/OLE limitations,
        next if $file =~ /^dirdlg\./;

        $file =~ s/cp?p?$/\o/;
        $project{"WXMSW_GUIOBJS"} .= "src/msw/" . $file . " "
    }

    foreach $file (sort keys %wxGTK) {
        $file =~ s/cp?p?$/\o/;
        $project{"WXGTK_GUIOBJS"} .= "src/gtk/" . $file . " "
    }

    foreach $file (sort keys %wxMOTIF) {
        $file =~ s/cp?p?$/\o/;
        $project{"WXMOTIF_GUIOBJS"} .= "src/motif/" . $file . " "
    }

    #! others
    
    foreach $file (sort keys %wxHTML) {
        $file =~ s/cp?p?$/\o/;
        $project{"WXHTMLOBJS"} .= "src/html/" . $file . " "
    }

    foreach $file (sort keys %wxUNIX) {
        $file =~ s/cp?p?$/\o/;
        $project{"WXUNIXOBJS"} .= "src/unix/" . $file . " "
    }
    
    #! headers
    
    foreach $file (sort keys %wxWXINCLUDE) {
        $project{"WX_HEADERS"} .= $file . " "
    }
    
    foreach $file (sort keys %wxGENERICINCLUDE) {
        $project{"WXGENERIC_HEADERS"} .= "generic/" . $file . " "
    }
    
    foreach $file (sort keys %wxMOTIFINCLUDE) {
        $project{"WXMOTIF_HEADERS"} .= "motif/" . $file . " "
    }
    
    foreach $file (sort keys %wxGTKINCLUDE) {
        $project{"WXGTK_HEADERS"} .= "gtk/" . $file . " "
    }
    
    foreach $file (sort keys %wxMSWINCLUDE) {
        $project{"WXMSW_HEADERS"} .= "msw/" . $file . " "
    }
    
    foreach $file (sort keys %wxHTMLINCLUDE) {
        $project{"WXHTML_HEADERS"} .= "html/" . $file . " "
    }
    
    foreach $file (sort keys %wxUNIXINCLUDE) {
        $project{"WXUNIX_HEADERS"} .= "unix/" . $file . " "
    }
    
    foreach $file (sort keys %wxPROTOCOLINCLUDE) {
        $project{"WXPROTOCOL_HEADERS"} .= "protocol/" . $file . " "
    }
#$}
#
# This file was automatically generated by tmake at #$ Now()
# DO NOT CHANGE THIS FILE, YOUR CHANGES WILL BE LOST! CHANGE UNX.T!

#
# File:     makefile.unx
# Author:   Julian Smart, Robert Roebling, Vadim Zeitlin
# Created:  1993
# Updated:  1999
# Copyright:(c) 1993, AIAI, University of Edinburgh,
# Copyright:(c) 1999, Vadim Zeitlin
# Copyright:(c) 1999, Robert Roebling
#
# Makefile for libwx_gtk.a, libwx_motif.a and libwx_msw.a

###################################################################

include ./src/make.env

############## override make.env for PIC ##########################

# Clears all default suffixes
.SUFFIXES:	.o .cpp .c .cxx

.c.o :
	$(CCC) -c $(CFLAGS) $(PICFLAGS) -o $@ $<

.cpp.o :
	$(CC) -c $(CPPFLAGS) $(PICFLAGS) -o $@ $<

.cxx.o :
	$(CC) -c $(CPPFLAGS) $(PICFLAGS) -o $@ $<

########################### Paths #################################

srcdir = @srcdir@

VPATH = :@top_srcdir@

top_srcdir = @top_srcdir@
prefix = @prefix@
exec_prefix = @exec_prefix@

bindir = @bindir@
sbindir = @sbindir@
libexecdir = @libexecdir@
datadir = @datadir@
sysconfdir = @sysconfdir@
sharedstatedir = @sharedstatedir@
localstatedir = @localstatedir@
libdir = @libdir@
infodir = @infodir@
mandir = @mandir@
includedir = @includedir@
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/@PACKAGE@
pkglibdir = $(libdir)/@PACKAGE@
pkgincludedir = $(includedir)/@PACKAGE@

top_builddir = .

INSTALL = @INSTALL@
INSTALL_PROGRAM = @INSTALL_PROGRAM@
INSTALL_DATA = @INSTALL_DATA@
INSTALL_SCRIPT = @INSTALL_SCRIPT@
transform = @program_transform_name@

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_alias = @build_alias@
build_triplet = @build@
host_alias = @host_alias@
host_triplet = @host@
target_alias = @target_alias@
target_triplet = @target@

############################# Dirs #################################

WXDIR = $(top_srcdir)

# Subordinate library possibilities

SRCDIR   = $(WXDIR)/src
GENDIR   = $(WXDIR)/src/generic
COMMDIR  = $(WXDIR)/src/common
HTMLDIR  = $(WXDIR)/src/html
UNIXDIR  = $(WXDIR)/src/unix
PNGDIR   = $(WXDIR)/src/png
JPEGDIR  = $(WXDIR)/src/jpeg
ZLIBDIR  = $(WXDIR)/src/zlib
GTKDIR   = $(WXDIR)/src/gtk
MOTIFDIR = $(WXDIR)/src/motif
MSWDIR   = $(WXDIR)/src/msw
INCDIR   = $(WXDIR)/include
SAMPDIR  = $(WXDIR)/samples

DOCDIR = $(WXDIR)/docs

########################## Archive name ###############################

WXARCHIVE = wx$(TOOLKIT)-$(WX_MAJOR_VERSION_NUMBER).$(WX_MINOR_VERSION_NUMBER).$(WX_RELEASE_NUMBER).tgz
DISTDIR = ./_dist_dir/wx$(TOOLKIT)

############################## Files ##################################

WX_HEADERS = \
		#$ ExpandList("WX_HEADERS");

GTK_HEADERS = \
		#$ ExpandList("WXGTK_HEADERS");

MOTIF_HEADERS = \
		#$ ExpandList("WXMOTIF_HEADERS");

MSW_HEADERS = \
		#$ ExpandList("WXMSW_HEADERS");

UNIX_HEADERS = \
		#$ ExpandList("WXUNIX_HEADERS");

GENERIC_HEADERS = \
		#$ ExpandList("WXGENERIC_HEADERS");

PROTOCOL_HEADERS = \
		#$ ExpandList("WXPROTOCOL_HEADERS");

HTML_HEADERS = \
		#$ ExpandList("WXHTML_HEADERS");

GTK_GENERICOBJS = \
		#$ ExpandList("WXGTK_GENERICOBJS");

GTK_COMMONOBJS = \
		parser.o \
		#$ ExpandList("WXGTK_COMMONOBJS");

GTK_GUIOBJS = \
		#$ ExpandList("WXGTK_GUIOBJS");

MOTIF_GENERICOBJS = \
		#$ ExpandList("WXMOTIF_GENERICOBJS");

MOTIF_COMMONOBJS = \
		parser.o \
		#$ ExpandList("WXMOTIF_COMMONOBJS");

MOTIF_GUIOBJS = \
		src/motif/xmcombo/xmcombo.o \
		#$ ExpandList("WXMOTIF_GUIOBJS");

MSW_GENERICOBJS = \
		#$ ExpandList("WXMSW_GENERICOBJS");

MSW_COMMONOBJS = \
		#$ ExpandList("WXMSW_COMMONOBJS");

MSW_GUIOBJS = \
		#$ ExpandList("WXMSW_GUIOBJS");

HTMLOBJS = \
		#$ ExpandList("WXHTMLOBJS");

UNIXOBJS = \
		#$ ExpandList("WXUNIXOBJS");

ZLIBOBJS    = \
		src/zlib/adler32.o \
		src/zlib/compress.o \
		src/zlib/crc32.o \
		src/zlib/gzio.o \
		src/zlib/uncompr.o \
		src/zlib/deflate.o \
		src/zlib/trees.o \
		src/zlib/zutil.o \
		src/zlib/inflate.o \
		src/zlib/infblock.o \
		src/zlib/inftrees.o \
		src/zlib/infcodes.o \
		src/zlib/infutil.o \
		src/zlib/inffast.o

PNGOBJS     = \
		src/png/png.o \
		src/png/pngread.o \
		src/png/pngrtran.o \
		src/png/pngrutil.o \
		src/png/pngpread.o \
		src/png/pngtrans.o \
		src/png/pngwrite.o \
		src/png/pngwtran.o \
		src/png/pngwutil.o \
		src/png/pngerror.o \
		src/png/pngmem.o \
		src/png/pngwio.o \
		src/png/pngrio.o \
		src/png/pngget.o \
		src/png/pngset.o


JPEGOBJS    = \
		src/jpeg/jcomapi.o \
		src/jpeg/jutils.o \
		src/jpeg/jerror.o \
		src/jpeg/jmemmgr.o \
		src/jpeg/jmemnobs.o \
		src/jpeg/jcapimin.o \
		src/jpeg/jcapistd.o \
		src/jpeg/jctrans.o \
		src/jpeg/jcparam.o \
		src/jpeg/jdatadst.o \
		src/jpeg/jcinit.o \
		src/jpeg/jcmaster.o \
		src/jpeg/jcmarker.o \
		src/jpeg/jcmainct.o \
		src/jpeg/jcprepct.o \
		src/jpeg/jccoefct.o \
		src/jpeg/jccolor.o \
		src/jpeg/jcsample.o \
		src/jpeg/jchuff.o \
		src/jpeg/jcphuff.o \
		src/jpeg/jcdctmgr.o \
		src/jpeg/jfdctfst.o \
		src/jpeg/jfdctflt.o \
		src/jpeg/jfdctint.o \
		src/jpeg/jdapimin.o \
		src/jpeg/jdapistd.o \
		src/jpeg/jdtrans.o \
		src/jpeg/jdatasrc.o \
		src/jpeg/jdmaster.o \
		src/jpeg/jdinput.o \
		src/jpeg/jdmarker.o \
		src/jpeg/jdhuff.o \
		src/jpeg/jdphuff.o \
		src/jpeg/jdmainct.o \
		src/jpeg/jdcoefct.o \
		src/jpeg/jdpostct.o \
		src/jpeg/jddctmgr.o \
		src/jpeg/jidctfst.o \
		src/jpeg/jidctflt.o \
		src/jpeg/jidctint.o \
		src/jpeg/jidctred.o \
		src/jpeg/jdsample.o \
		src/jpeg/jdcolor.o \
		src/jpeg/jquant1.o \
		src/jpeg/jquant2.o \
		src/jpeg/jdmerge.o


OBJECTS = $(@GUIOBJS@) $(@COMMONOBJS@) $(@GENERICOBJS@) $(@UNIXOBJS@) $(HTMLOBJS) \
	  $(JPEGOBJS) $(PNGOBJS) $(ZLIBOBJS)

HEADERS = $(@GUIHEADERS@) $(HTML_HEADERS) $(UNIX_HEADERS) $(PROTOCOL_HEADERS) \
	  $(GENERIC_HEADERS) $(WX_HEADERS)

REQUIRED_DIRS = ./lib ./src ./src/common ./src/gtk ./src/motif ./src/msw \
                ./src/generic ./src/unix ./src/motif/xmombo ./src/html \
		./src/zlib ./src/jpeg ./src/png

all:    $(REQUIRED_DIRS) $(OBJECTS) @WX_TARGET_LIBRARY@ @WX_CREATE_LINKS@

$(REQUIRED_DIRS):	$(WXDIR)/include/wx/defs.h $(WXDIR)/include/wx/object.h $(WXDIR)/include/wx/setup.h
	@if test ! -d ./lib; then mkdir ./lib; fi
	@if test ! -d ./src; then mkdir ./src; fi
	@if test ! -d ./src/common; then mkdir ./src/common; fi
	@if test ! -d ./src/msw; then mkdir ./src/msw; fi
	@if test ! -d ./src/gtk; then mkdir ./src/gtk; fi
	@if test ! -d ./src/motif; then mkdir ./src/motif; fi
	@if test ! -d ./src/motif/xmcombo; then mkdir ./src/motif/xmcombo; fi
	@if test ! -d ./src/generic; then mkdir ./src/generic; fi
	@if test ! -d ./src/unix; then mkdir ./src/unix; fi
	@if test ! -d ./src/html; then mkdir ./src/html; fi
	@if test ! -d ./src/png; then mkdir ./src/png; fi
	@if test ! -d ./src/jpeg; then mkdir ./src/jpeg; fi
	@if test ! -d ./src/zlib; then mkdir ./src/zlib; fi

@WX_LIBRARY_NAME_STATIC@:  $(OBJECTS)
	$(AR) $(AROPTIONS) ./lib/$@ $(OBJECTS)
	$(RANLIB) ./lib/$@

@WX_LIBRARY_NAME_SHARED@:  $(OBJECTS)
	$(SHARED_LD) ./lib/$@ $(OBJECTS) $(EXTRALIBS)
	
CREATE_LINKS:  ./lib/@WX_TARGET_LIBRARY@
	@if test -e ./lib/@WX_LIBRARY_LINK1@; then rm -f ./lib/@WX_LIBRARY_LINK1@; fi
	@if test -e ./lib/@WX_LIBRARY_LINK2@; then rm -f ./lib/@WX_LIBRARY_LINK2@; fi
	@if test -e ./lib/@WX_LIBRARY_LINK3@; then rm -f ./lib/@WX_LIBRARY_LINK3@; fi
	$(LN_S) @WX_TARGET_LIBRARY@ ./lib/@WX_LIBRARY_LINK1@
	$(LN_S) @WX_TARGET_LIBRARY@ ./lib/@WX_LIBRARY_LINK2@
	$(LN_S) @WX_TARGET_LIBRARY@ ./lib/@WX_LIBRARY_LINK3@
	
CREATE_INSTALLED_LINKS:  $(libdir)/@WX_TARGET_LIBRARY@
	@if test -e $(libdir)/@WX_LIBRARY_LINK1@; then rm -f $(libdir)/@WX_LIBRARY_LINK1@; fi
	@if test -e $(libdir)/@WX_LIBRARY_LINK2@; then rm -f $(libdir)/@WX_LIBRARY_LINK2@; fi
	@if test -e $(libdir)/@WX_LIBRARY_LINK3@; then rm -f $(libdir)/@WX_LIBRARY_LINK3@; fi
	$(LN_S) @WX_TARGET_LIBRARY@ $(libdir)/@WX_LIBRARY_LINK1@
	$(LN_S) @WX_TARGET_LIBRARY@ $(libdir)/@WX_LIBRARY_LINK2@
	$(LN_S) @WX_TARGET_LIBRARY@ $(libdir)/@WX_LIBRARY_LINK3@
	
$(OBJECTS):	$(WXDIR)/include/wx/defs.h $(WXDIR)/include/wx/object.h $(WXDIR)/include/wx/setup.h

parser.o:    parser.c lexer.c
	$(CCLEX) -c $(CFLAGS) -o $@ parser.c

parser.c:	$(COMMDIR)/parser.y lexer.c
	$(YACC) $(COMMDIR)/parser.y
	@sed -e "s;$(COMMDIR)/y.tab.c;parser.y;g" < y.tab.c | \
	sed -e "s/BUFSIZ/5000/g"            | \
	sed -e "s/YYLMAX 200/YYLMAX 5000/g" | \
	sed -e "s/yy/PROIO_yy/g"            | \
	sed -e "s/input/PROIO_input/g"      | \
	sed -e "s/unput/PROIO_unput/g"      > parser.c
	@$(RM) y.tab.c

lexer.c:	$(COMMDIR)/lexer.l
	$(LEX) $(COMMDIR)/lexer.l
	@sed -e "s;$(COMMDIR)/lex.yy.c;lexer.l;g" < lex.yy.c | \
	sed -e "s/yy/PROIO_yy/g"            | \
	sed -e "s/input/PROIO_input/g"      | \
	sed -e "s/unput/PROIO_unput/g"      > lexer.c
	@$(RM) lex.yy.c

preinstall: $(top_builddir)/lib/@WX_TARGET_LIBRARY@ $(top_builddir)/wx-config $(top_builddir)/setup.h
	@echo " "
	@echo " Installing wxWindows..."
	@echo " "

	$(INSTALL_SCRIPT) $(top_builddir)/wx-config $(bindir)/wx-config
	$(INSTALL_PROGRAM) $(top_builddir)/lib/@WX_TARGET_LIBRARY@ $(libdir)/@WX_TARGET_LIBRARY@
	
	@if test ! -d $(libdir)/wx; then mkdir $(libdir)/wx; fi
	@if test ! -d $(libdir)/wx/include; then mkdir $(libdir)/wx/include; fi
	@if test ! -d $(libdir)/wx/include/wx; then mkdir $(libdir)/wx/include/wx; fi
	@if test ! -d $(libdir)/wx/include/wx/@TOOLKIT_DIR@; then mkdir $(libdir)/wx/include/wx/@TOOLKIT_DIR@; fi
	$(INSTALL_DATA) $(top_builddir)/setup.h $(libdir)/wx/include/wx/@TOOLKIT_DIR@/setup.h
	
	@if test ! -d $(includedir)/wx; then mkdir $(includedir)/wx; fi
	@if test ! -d $(includedir)/wx/msw; then mkdir $(includedir)/wx/msw; fi
	@if test ! -d $(includedir)/wx/gtk; then mkdir $(includedir)/wx/gtk; fi
	@if test ! -d $(includedir)/wx/motif; then mkdir $(includedir)/wx/motif; fi
	@if test ! -d $(includedir)/wx/html; then mkdir $(includedir)/wx/html; fi
	@if test ! -d $(includedir)/wx/protocol; then mkdir $(includedir)/wx/protocol; fi
	@if test ! -d $(includedir)/wx/unix; then mkdir $(includedir)/wx/unix; fi
	@if test ! -d $(includedir)/wx/generic; then mkdir $(includedir)/wx/generic; fi
	@list='$(HEADERS)'; for p in $$list; do \
	  $(INSTALL_DATA) $(top_srcdir)/include/wx/$$p $(includedir)/wx/$$p; \
	  echo "$(INSTALL_DATA) $(top_srcdir)/include/wx/$$p $(includedir)/wx/$$p"; \
	done
	
write_message:
	@echo " "
	@echo " The installation of wxWindows is finished.  On certain"
	@echo " platforms (e.g. Linux, Solaris) you'll now have to run"
	@echo " ldconfig if you installed a shared library."
	@echo " "
	@echo " wxWindows comes with  no guarantees  and doesn't claim"
	@echo " to be suitable for any purpose."
	@echo " "
	@echo " Read the wxWindows Licence on licencing conditions."
	@echo " "

install: preinstall @WX_CREATE_INSTALLED_LINKS@ write_message

uninstall:
	@echo " "
	@echo " Uninstalling wxWindows..."
	@echo " "
	@echo " Removing library..."
	@rm -f $(libdir)/@WX_TARGET_LIBRARY@
	@rm -f $(libdir)/@WX_LIBRARY_LINK1@
	@rm -f $(libdir)/@WX_LIBRARY_LINK2@
	@rm -f $(libdir)/@WX_LIBRARY_LINK3@
	@echo " Removing helper files..."
	@rm -f $(libdir)/wx/include/wx/@TOOLKIT_DIR@/setup.h
	@rm -f $(bindir)/wx-config
	@echo " Removing headers..."
	@list='$(HEADERS)'; for p in $$list; do \
	  rm -f $(includedir)/wx/$$p; \
	done
	@echo " Removing directories..."
	@if test -d $(libdir)/wx/include/wx/@TOOLKIT_DIR@; then rmdir $(libdir)/wx/include/wx/@TOOLKIT_DIR@; fi
	@if test -d $(libdir)/wx/include/wx; then rmdir $(libdir)/wx/include/wx; fi
	@if test -d $(libdir)/wx/include; then rmdir $(libdir)/wx/include; fi
	@if test -d $(libdir)/wx; then rmdir $(libdir)/wx; fi
	@if test -d $(includedir)/wx/gtk; then rmdir $(includedir)/wx/gtk; fi
	@if test -d $(includedir)/wx/motif; then rmdir $(includedir)/wx/motif; fi
	@if test -d $(includedir)/wx/motif; then rmdir $(includedir)/wx/msw; fi
	@if test -d $(includedir)/wx/html; then rmdir $(includedir)/wx/html; fi
	@if test -d $(includedir)/wx/unix; then rmdir $(includedir)/wx/unix; fi
	@if test -d $(includedir)/wx/generic; then rmdir $(includedir)/wx/generic; fi
	@if test -d $(includedir)/wx/protocol; then rmdir $(includedir)/wx/protocol; fi
	@if test -d $(includedir)/wx; then rmdir $(includedir)/wx; fi

ALL_DIST:
	mkdir _dist_dir
	mkdir $(DISTDIR)
	cp $(WXDIR)/wx$(TOOLKIT).spec $(DISTDIR)
	cp $(WXDIR)/configure $(DISTDIR)
	cp $(WXDIR)/config.sub $(DISTDIR)
	cp $(WXDIR)/config.guess $(DISTDIR)
	cp $(WXDIR)/install-sh $(DISTDIR)
	cp $(WXDIR)/mkinstalldirs $(DISTDIR)
	cp $(WXDIR)/wx-config.in $(DISTDIR)
	cp $(WXDIR)/setup.h.in $(DISTDIR)
	cp $(WXDIR)/Makefile.in $(DISTDIR)
	cp $(DOCDIR)/lgpl.txt $(DISTDIR)/COPYING.LIB
	cp $(DOCDIR)/licence.txt $(DISTDIR)/LICENCE.txt
	cp $(DOCDIR)/symbols.txt $(DISTDIR)/SYMBOLS.txt
	cp $(DOCDIR)/$(TOOLKITDIR)/install.txt $(DISTDIR)/INSTALL.txt
	cp $(DOCDIR)/$(TOOLKITDIR)/changes.txt $(DISTDIR)/CHANGES.txt
	cp $(DOCDIR)/$(TOOLKITDIR)/readme.txt $(DISTDIR)/README.txt
	cp $(DOCDIR)/$(TOOLKITDIR)/todo.txt $(DISTDIR)/TODO.txt
	mkdir $(DISTDIR)/include
	mkdir $(DISTDIR)/include/wx
	mkdir $(DISTDIR)/include/wx/$(TOOLKITDIR)
	mkdir $(DISTDIR)/include/wx/generic
	mkdir $(DISTDIR)/include/wx/html
	mkdir $(DISTDIR)/include/wx/unix
	mkdir $(DISTDIR)/include/wx/protocol
	cp $(INCDIR)/wx/*.h $(DISTDIR)/include/wx 
	cp $(INCDIR)/wx/*.cpp $(DISTDIR)/include/wx
	cp $(INCDIR)/wx/generic/*.h $(DISTDIR)/include/wx/generic
	cp $(INCDIR)/wx/generic/*.xpm $(DISTDIR)/include/wx/generic
	cp $(INCDIR)/wx/html/*.h $(DISTDIR)/include/wx/html
	cp $(INCDIR)/wx/unix/*.h $(DISTDIR)/include/wx/unix
	cp $(INCDIR)/wx/protocol/*.h $(DISTDIR)/include/wx/protocol
	mkdir $(DISTDIR)/src
	mkdir $(DISTDIR)/src/common
	mkdir $(DISTDIR)/src/generic
	mkdir $(DISTDIR)/src/html
	mkdir $(DISTDIR)/src/html/bitmaps
	mkdir $(DISTDIR)/src/$(TOOLKITDIR)
	mkdir $(DISTDIR)/src/unix
	mkdir $(DISTDIR)/src/png
	mkdir $(DISTDIR)/src/jpeg
	mkdir $(DISTDIR)/src/zlib
	cp $(SRCDIR)/*.in $(DISTDIR)/src
	cp $(COMMDIR)/*.cpp $(DISTDIR)/src/common
	cp $(COMMDIR)/*.c $(DISTDIR)/src/common
	cp $(COMMDIR)/*.inc $(DISTDIR)/src/common
	cp $(COMMDIR)/*.l $(DISTDIR)/src/common
	cp $(COMMDIR)/*.h $(DISTDIR)/src/common
	cp $(COMMDIR)/*.y $(DISTDIR)/src/common
	cp $(GENDIR)/*.cpp $(DISTDIR)/src/generic
	cp $(HTMLDIR)/*.cpp $(DISTDIR)/src/html
	cp $(HTMLDIR)/*.h $(DISTDIR)/src/html
	cp $(HTMLDIR)/bitmaps/*.xpm $(DISTDIR)/src/html/bitmaps
	cp $(UNIXDIR)/*.h $(DISTDIR)/src/unix
	cp $(UNIXDIR)/*.c $(DISTDIR)/src/unix
	cp $(UNIXDIR)/*.cpp $(DISTDIR)/src/unix
	cp $(PNGDIR)/*.h $(DISTDIR)/src/png
	cp $(PNGDIR)/*.c $(DISTDIR)/src/png
	cp $(PNGDIR)/README $(DISTDIR)/src/png
	cp $(ZLIBDIR)/*.h $(DISTDIR)/src/zlib
	cp $(ZLIBDIR)/*.c $(DISTDIR)/src/zlib
	cp $(ZLIBDIR)/README $(DISTDIR)/src/zlib
	cp $(JPEGDIR)/*.h $(DISTDIR)/src/jpeg
	cp $(JPEGDIR)/*.c $(DISTDIR)/src/jpeg
	cp $(JPEGDIR)/README $(DISTDIR)/src/jpeg

GTK_DIST:
	cp $(WXDIR)/wxGTK.spec $(DISTDIR)
	cp $(INCDIR)/wx/gtk/*.h $(DISTDIR)/include/wx/gtk
	cp $(INCDIR)/wx/gtk/*.xpm $(DISTDIR)/include/wx/gtk
	cp $(GTKDIR)/*.cpp $(DISTDIR)/src/gtk
	cp $(GTKDIR)/*.c $(DISTDIR)/src/gtk
	cp $(GTKDIR)/*.xbm $(DISTDIR)/src/gtk

MOTIF_DIST:
	cp $(WXDIR)/wxMOTIF.spec $(DISTDIR)
	cp $(INCDIR)/wx/motif/*.h $(DISTDIR)/include/wx/motif
	cp $(MOTIFDIR)/*.cpp $(DISTDIR)/src/motif
	cp $(MOTIFDIR)/*.xbm $(DISTDIR)/src/motif
	mkdir $(DISTDIR)/src/motif/xmcombo
	cp $(MOTIFDIR)/xmcombo/*.c $(DISTDIR)/src/motif/xmcombo
	cp $(MOTIFDIR)/xmcombo/*.h $(DISTDIR)/src/motif/xmcombo
	cp $(MOTIFDIR)/xmcombo/copying.txt $(DISTDIR)/src/motif/xmcombo

MSW_DIST:
	cp $(WXDIR)/wxWINE.spec $(DISTDIR)
	cp $(INCDIR)/wx/msw/*.h $(DISTDIR)/include/wx/msw
	cp $(INCDIR)/wx/msw/*.cur $(DISTDIR)/include/wx/msw
	cp $(INCDIR)/wx/msw/*.ico $(DISTDIR)/include/wx/msw
	cp $(INCDIR)/wx/msw/*.bmp $(DISTDIR)/include/wx/msw
	cp $(INCDIR)/wx/msw/*.rc $(DISTDIR)/include/wx/msw
	cp $(MSWDIR)/*.cpp $(DISTDIR)/src/msw
	cp $(MSWDIR)/*.c $(DISTDIR)/src/msw
	cp $(MSWDIR)/*.def $(DISTDIR)/src/msw
	mkdir $(DISTDIR)/src/msw/ole
	cp $(MSWDIR)/ole/*.cpp $(DISTDIR)/src/msw/ole

SAMPLES_DIST:
	mkdir $(DISTDIR)/samples
	cp $(SAMPDIR)/Makefile.in $(DISTDIR)/samples
	mkdir $(DISTDIR)/samples/bombs
	cp $(SAMPDIR)/bombs/Makefile.in $(DISTDIR)/samples/bombs
	cp $(SAMPDIR)/bombs/*.cpp $(DISTDIR)/samples/bombs
	cp $(SAMPDIR)/bombs/*.h $(DISTDIR)/samples/bombs
	cp $(SAMPDIR)/bombs/*.xpm $(DISTDIR)/samples/bombs
	cp $(SAMPDIR)/bombs/readme.txt $(DISTDIR)/samples/bombs
	mkdir $(DISTDIR)/samples/caret
	cp $(SAMPDIR)/caret/Makefile.in $(DISTDIR)/samples/caret
	cp $(SAMPDIR)/caret/*.cpp $(DISTDIR)/samples/caret
	cp $(SAMPDIR)/caret/*.xpm $(DISTDIR)/samples/caret
	mkdir $(DISTDIR)/samples/config
	cp $(SAMPDIR)/config/Makefile.in $(DISTDIR)/samples/config
	cp $(SAMPDIR)/config/*.cpp $(DISTDIR)/samples/config
	mkdir $(DISTDIR)/samples/controls
	mkdir $(DISTDIR)/samples/controls/icons
	cp $(SAMPDIR)/controls/Makefile.in $(DISTDIR)/samples/controls
	cp $(SAMPDIR)/controls/*.cpp $(DISTDIR)/samples/controls
	cp $(SAMPDIR)/controls/*.xpm $(DISTDIR)/samples/controls
	cp $(SAMPDIR)/controls/icons/*.??? $(DISTDIR)/samples/controls/icons
	mkdir $(DISTDIR)/samples/checklst
	cp $(SAMPDIR)/checklst/Makefile.in $(DISTDIR)/samples/checklst
	cp $(SAMPDIR)/checklst/*.cpp $(DISTDIR)/samples/checklst
	cp $(SAMPDIR)/checklst/*.xpm $(DISTDIR)/samples/checklst
	mkdir $(DISTDIR)/samples/checkls
	cp $(SAMPDIR)/printing/Makefile.in $(DISTDIR)/samples/printing
	cp $(SAMPDIR)/printing/*.cpp $(DISTDIR)/samples/printing
	cp $(SAMPDIR)/printing/*.h $(DISTDIR)/samples/printing
	cp $(SAMPDIR)/printing/*.xpm $(DISTDIR)/samples/printing
	cp $(SAMPDIR)/printing/*.xbm $(DISTDIR)/samples/printing

dist: ALL_DIST @GUIDIST@ SAMPLES_DIST
	cd _dist_dir; tar ch wx$(TOOLKIT) | gzip -f9 > $(WXARCHIVE); mv $(WXARCHIVE) ..
	rm -f -r _dist_dir

clean:
	rm -f ./src/msw/*.o
	rm -f ./src/gtk/*.o
	rm -f ./src/motif/*.o
	rm -f ./src/html/*.o
	rm -f ./src/common/*.o
	rm -f ./src/unix/*.o
	rm -f ./src/generic/*.o
	rm -f ./src/png/*.o
	rm -f ./src/jpeg/*.o
	rm -f ./src/zlib/*.o
	rm -f *.o
	rm -f parser.c
	rm -f lexer.c
	rm -f ./lib/*

cleanall: clean
