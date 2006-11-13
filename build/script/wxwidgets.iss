; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#if GetEnv("WXW_VER") == "CVS"
    #define INFOFILE  "C:\wx\inno\wxWidgets\BuildCVS.txt"
    #define SETUPFILENAME  "wxMSW-cvs-Setup"
    #define WX_VERSION "CVS"
#else
    #define INFOFILE "C:\wx\inno\wxWidgets\docs\msw\install.txt"
    #define SETUPFILENAME  "wxMSW-" + GetENV("WXW_VER") + "-Setup"
    #define WX_VERSION GetENV("WXW_VER")
#endif



[Setup]
AppName=wxWidgets
AppVerName=wxWidgets {#WX_VERSION}
AppPublisher=wxWidgets
AppPublisherURL=http://www.wxwidgets.org
AppSupportURL=http://www.wxwidgets.org
AppUpdatesURL=http://www.wxwidgets.org
DefaultDirName={sd}\wxWidgets-{#WX_VERSION}
DefaultGroupName=wxWidgets {#WX_VERSION}
DisableProgramGroupPage=yes
LicenseFile=C:\wx\inno\wxWidgets\docs\licence.txt
InfoBeforeFile=C:\wx\inno\wxWidgets\docs\readme.txt
InfoAfterFile={#INFOFILE}
OutputDir=c:\daily
OutputBaseFilename={#SETUPFILENAME}
SetupIconFile=C:\wx\inno\wxWidgets\art\wxwin.ico
Compression=lzma
SolidCompression=yes

[Files]
Source: "C:\wx\inno\wxWidgets\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[INI]
Filename: "{app}\wx.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.wxwidgets.org"

[Icons]
Name: "{group}\{cm:ProgramOnTheWeb,wxWidgets {#WX_VERSION}}"; Filename: "{app}\wx.url"
Name: {group}\wxWidgets Manual; Filename: {app}\docs\htmlhelp\wx.chm; WorkingDir: {app}; IconIndex: 0; Flags: useapppaths
Name: {group}\Changes; Filename: {app}\docs\changes.txt; WorkingDir: {app}; IconIndex: 0; Flags: useapppaths
Name: {group}\Readme; Filename: {app}\docs\readme.txt; WorkingDir: {app}; IconIndex: 0; Flags: useapppaths
Name: {group}\Compiling wxWidgets; Filename: {app}\docs\msw\install.txt; WorkingDir: {app}; IconIndex: 0; Flags: useapppaths
Name: "{group}\Uninstall wxWidgets {#WX_VERSION}"; Filename: "{uninstallexe}"


[UninstallDelete]
Type: files; Name: "{app}\wx.url"

