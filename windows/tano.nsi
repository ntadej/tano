;NSIS Modern User Interface
;Start Menu Folder Selection Example Script
;Written by Joost Verburg
;Modified by Tadej Novak

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  ;Name and file
  !define VERSION 0.8.0
  !define COMPANY "Copyright © Tadej Novak 2008-2010"
  !define URL http://tanoplayer.co.cc

  Name "Tano ${VERSION}"
  OutFile "tano_${VERSION}_win32.exe"
  InstallDir "$PROGRAMFILES\Tano"

  BrandingText "${COMPANY} | ${URL}"

  InstType "Minimum"
  InstType $(S_Full)

  !include "tano-ui.nsh"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Tano Player" ""

  SetCompressor lzma
  
;--------------------------------
;Variables

  Var StartMenuFolder

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Language Selection Dialog Settings

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\Tano Player" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "LICENSE.GPL.TXT"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  
  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Tano Player" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  
  !insertmacro MUI_PAGE_INSTFILES
  !define MUI_FINISHPAGE_RUN "$INSTDIR\tano.exe"
  !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English" ;first language is the default language
  !insertmacro MUI_LANGUAGE "Slovenian"
  !insertmacro MUI_LANGUAGE "Checz"

;--------------------------------
;Reserve Files
  
  ;If you are using solid compression, files that are required before
  ;the actual installation should be stored first in the data block,
  ;because this will make your installer start faster.
  
  !insertmacro MUI_RESERVEFILE_LANGDLL

;--------------------------------
;Installer Sections

Section $(NAME_SecMain) SecMain
  SectionIn 1 2 RO

  !include "tano-install.nsh"
  
  
  ;Store installation folder
  WriteRegStr HKCU "Software\Tano Player" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
	CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Tano.lnk" "$INSTDIR\tano.exe"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END
  
  WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tano Player" "DisplayName" "Tano ${VERSION} (remove only)"
  WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tano Player" "UninstallString" '"$INSTDIR\Uninstall.exe"'

SectionEnd

Section $(NAME_SecDesk) SecDesk
  SectionIn 2
  CreateShortCut "$DESKTOP\Tano.lnk" "$INSTDIR\tano.exe"
SectionEnd

Section $(NAME_SecQuick) SecQuick
  SectionIn 2
  CreateShortCut "$QUICKLAUNCH\Tano.lnk" "$INSTDIR\tano.exe"
SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  !include "tano-language.nsh"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} $(DESC_SecMain)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDesk} $(DESC_SecDesk)
	!insertmacro MUI_DESCRIPTION_TEXT ${SecQuick} $(DESC_SecQuick)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
  
;--------------------------------
;Installer Functions

Function .onInit

  !insertmacro MUI_LANGDLL_DISPLAY
  
  ReadRegStr $0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tano Player" "UninstallString"
  StrCmp $0 "" done
 
  MessageBox MB_YESNO|MB_ICONEXCLAMATION $(PRE) IDNO done
 
  ;Run the uninstaller
  ;uninst:
    ClearErrors
    ExecWait '$0 _?=$INSTDIR' ;Do not copy the uninstaller to a temp file
  done:

FunctionEnd
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

  !include "tano-uninstall.nsh"

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"
  
  Delete "$DESKTOP\Tano.lnk"
  Delete "$QUICKLAUNCH\Tano.lnk"
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    
  Delete "$SMPROGRAMS\$StartMenuFolder\Tano.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"
  
  DeleteRegKey HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tano Player"
  DeleteRegKey /ifempty HKCU "Software\Tano Player"

SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd