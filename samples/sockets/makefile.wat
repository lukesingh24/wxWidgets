# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.1 (http://bakefile.sourceforge.net)
#     Do not modify, all changes will be overwritten!
# =========================================================================

!include ../../build/config.wat

# -------------------------------------------------------------------------
# Do not modify the rest of this file!
# -------------------------------------------------------------------------

# Speed up compilation a bit:
!ifdef __LOADDLL__
!  loaddll wcc      wccd
!  loaddll wccaxp   wccdaxp
!  loaddll wcc386   wccd386
!  loaddll wpp      wppdi86
!  loaddll wppaxp   wppdaxp
!  loaddll wpp386   wppd386
!  loaddll wlink    wlink
!  loaddll wlib     wlibd
!endif

# We need these variables in some bakefile-made rules:
WATCOM_CWD = $+ $(%cdrive):$(%cwd) $-

### Conditionally set variables: ###

PORTNAME =
!ifeq USE_GUI 0
PORTNAME = base
!endif
!ifeq USE_GUI 1
PORTNAME = msw
!endif
WXDEBUGFLAG =
!ifeq BUILD debug
WXDEBUGFLAG = d
!endif
WXDLLFLAG =
!ifeq SHARED 1
WXDLLFLAG = dll
!endif
WXUNICODEFLAG =
!ifeq UNICODE 1
WXUNICODEFLAG = u
!endif
WXUNIVNAME =
!ifeq WXUNIV 1
WXUNIVNAME = univ
!endif
__DEBUGFLAG =
!ifeq BUILD debug
__DEBUGFLAG = debug all
!endif
!ifeq BUILD release
__DEBUGFLAG = 
!endif
__DEBUGFLAG_2 =
!ifeq BUILD debug
__DEBUGFLAG_2 = -d2
!endif
!ifeq BUILD release
__DEBUGFLAG_2 = -d0
!endif
__DEBUG_DEFINE_p =
!ifeq BUILD debug
__DEBUG_DEFINE_p = -d__WXDEBUG__
!endif
__DLLFLAG_p =
!ifeq SHARED 1
__DLLFLAG_p = -dWXUSINGDLL
!endif
__LIB_JPEG_p =
!ifeq USE_GUI 1
__LIB_JPEG_p = wxjpeg$(WXDEBUGFLAG).lib
!endif
__LIB_PNG_p =
!ifeq USE_GUI 1
__LIB_PNG_p = wxpng$(WXDEBUGFLAG).lib
!endif
__LIB_TIFF_p =
!ifeq USE_GUI 1
__LIB_TIFF_p = wxtiff$(WXDEBUGFLAG).lib
!endif
__OPTIMIZEFLAG =
!ifeq BUILD debug
__OPTIMIZEFLAG = -od
!endif
!ifeq BUILD release
__OPTIMIZEFLAG = -ot -ox
!endif
__RUNTIME_LIBS =
!ifeq RUNTIME_LIBS dynamic
__RUNTIME_LIBS = -br
!endif
!ifeq RUNTIME_LIBS static
__RUNTIME_LIBS = 
!endif
__UNICODE_DEFINE_p =
!ifeq UNICODE 1
__UNICODE_DEFINE_p = -dwxUSE_UNICODE=1
!endif
__WXLIB_BASE_p =
!ifeq MONOLITHIC 0
__WXLIB_BASE_p = wxbase25$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib
!endif
__WXLIB_CORE_p =
!ifeq MONOLITHIC 0
__WXLIB_CORE_p = &
	wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_core.lib
!endif
__WXLIB_MONO_p =
!ifeq MONOLITHIC 1
__WXLIB_MONO_p = &
	wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib
!endif
__WXLIB_NET_p =
!ifeq MONOLITHIC 0
__WXLIB_NET_p = wxbase25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_net.lib
!endif
__WXUNIV_DEFINE_p =
!ifeq WXUNIV 1
__WXUNIV_DEFINE_p = -d__WXUNIVERSAL__
!endif

### Variables: ###

CLIENT_CXXFLAGS = $(CPPFLAGS) $(__DEBUGFLAG_2) $(__OPTIMIZEFLAG) -bm &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=.\..\..\include -i=$(LIBDIRNAME) &
	-i=.\..\..\src\tiff -i=.\..\..\src\jpeg -i=.\..\..\src\png &
	-i=.\..\..\src\zlib -i=.\..\..\src\regex -i=.\..\..\src\expat\lib -i=. &
	$(__DLLFLAG_p) $(CXXFLAGS)
CLIENT_OBJECTS =  &
	$(OBJS)\client_client.obj
LIBDIRNAME = &
	.\..\..\lib\wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
OBJS = &
	wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
SERVER_CXXFLAGS = $(CPPFLAGS) $(__DEBUGFLAG_2) $(__OPTIMIZEFLAG) -bm &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=.\..\..\include -i=$(LIBDIRNAME) &
	-i=.\..\..\src\tiff -i=.\..\..\src\jpeg -i=.\..\..\src\png &
	-i=.\..\..\src\zlib -i=.\..\..\src\regex -i=.\..\..\src\expat\lib -i=. &
	$(__DLLFLAG_p) $(CXXFLAGS)
SERVER_OBJECTS =  &
	$(OBJS)\server_server.obj



all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(OBJS)\client.exe $(OBJS)\server.exe

$(OBJS)\client_client.obj :  .AUTODEPEND .\client.cpp
	$(CXX) -zq -fo=$^@ $(CLIENT_CXXFLAGS) $<

$(OBJS)\client_client.res :  .AUTODEPEND .\client.rc
	wrc -q -ad -bt=nt -r -fo=$^@ -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) -i=.\..\..\include -i=$(LIBDIRNAME) -i=.\..\..\src\tiff -i=.\..\..\src\jpeg -i=.\..\..\src\png -i=.\..\..\src\zlib  -i=.\..\..\src\regex -i=.\..\..\src\expat\lib -i=. $(__DLLFLAG_p) $<

$(OBJS)\server_server.obj :  .AUTODEPEND .\server.cpp
	$(CXX) -zq -fo=$^@ $(SERVER_CXXFLAGS) $<

$(OBJS)\server_server.res :  .AUTODEPEND .\server.rc
	wrc -q -ad -bt=nt -r -fo=$^@ -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) -i=.\..\..\include -i=$(LIBDIRNAME) -i=.\..\..\src\tiff -i=.\..\..\src\jpeg -i=.\..\..\src\png -i=.\..\..\src\zlib  -i=.\..\..\src\regex -i=.\..\..\src\expat\lib -i=. $(__DLLFLAG_p) $<

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(OBJS)\client.exe del $(OBJS)\client.exe
	-if exist $(OBJS)\server.exe del $(OBJS)\server.exe

$(OBJS)\client.exe :  $(CLIENT_OBJECTS) $(OBJS)\client_client.res
	@%create $(OBJS)\client.lbc
	@%append $(OBJS)\client.lbc option quiet
	@%append $(OBJS)\client.lbc name $^@
	@%append $(OBJS)\client.lbc option incremental
	@%append $(OBJS)\client.lbc $(LDFLAGS) $(__DEBUGFLAG)  libpath $(LIBDIRNAME) system nt_win ref '_WinMain@16'
	@for %i in ($(CLIENT_OBJECTS)) do @%append $(OBJS)\client.lbc file %i
	@for %i in ( $(__WXLIB_CORE_p) $(__WXLIB_NET_p) $(__WXLIB_BASE_p) $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib  kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib ) do @%append $(OBJS)\client.lbc library %i
	@%append $(OBJS)\client.lbc option resource=$(OBJS)\client_client.res
	wlink @$(OBJS)\client.lbc

$(OBJS)\server.exe :  $(SERVER_OBJECTS) $(OBJS)\server_server.res
	@%create $(OBJS)\server.lbc
	@%append $(OBJS)\server.lbc option quiet
	@%append $(OBJS)\server.lbc name $^@
	@%append $(OBJS)\server.lbc option incremental
	@%append $(OBJS)\server.lbc $(LDFLAGS) $(__DEBUGFLAG)  libpath $(LIBDIRNAME) system nt_win ref '_WinMain@16'
	@for %i in ($(SERVER_OBJECTS)) do @%append $(OBJS)\server.lbc file %i
	@for %i in ( $(__WXLIB_CORE_p) $(__WXLIB_NET_p) $(__WXLIB_BASE_p) $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib  kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib ) do @%append $(OBJS)\server.lbc library %i
	@%append $(OBJS)\server.lbc option resource=$(OBJS)\server_server.res
	wlink @$(OBJS)\server.lbc
