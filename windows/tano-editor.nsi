;------------------------------------------------------------------------
; Tano - An Open IP TV Player
; Copyright (C) 2012 Tadej Novak <tadej@tano.si>
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program. If not, see <http://www.gnu.org/licenses/>.
;------------------------------------------------------------------------
;NSIS Modern User Interface
;Start Menu Folder Selection Example Script
;Written by Joost Verburg
;Modified by Tadej Novak
;--------------------------------

;--------------------------------
;Include Modern UI and UAC support

  !include "MUI2.nsh"
  !include UAC.nsh

;--------------------------------
;General

  ;Name and file
  !define VERSION 1.0-git
  !define COMPANY "Copyright (C) Tadej Novak 2008-2012"
  !define URL http://projects.tano.si

  Name "Tano Editor ${VERSION}"
  OutFile "tano_editor_${VERSION}_win32.exe"
  InstallDir "$PROGRAMFILES\Tano Editor"

  BrandingText "${COMPANY} | ${URL}"

  InstType $(S_Minimum)
  InstType $(S_Full)

  !include "tano-ui.nsh"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Tano Editor" ""

  SetCompressor lzma

  ;Request application privileges for Windows Vista and later
  RequestExecutionLevel user

;--------------------------------
;Variables

  Var STARTMENU_FOLDER

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Language Selection Dialog Settings

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\Tano Editor" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "LICENSE.GPL.TXT"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  
  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Tano Editor"
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Startmenu"
  !define MUI_STARTMENUPAGE_DEFAULTFOLDER "Tano Editor"
  
  !insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER
  
  !insertmacro MUI_PAGE_INSTFILES
  !define MUI_FINISHPAGE_RUN
  !define MUI_FINISHPAGE_RUN_FUNCTION PageFinishRun
  !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English" ;first language is the default language
  !insertmacro MUI_LANGUAGE "Slovenian"
  !insertmacro MUI_LANGUAGE "Czech"

;--------------------------------
;Reserve Files
  
  ;If you are using solid compression, files that are required before
  ;the actual installation should be stored first in the data block,
  ;because this will make your installer start faster.
  
  !insertmacro MUI_RESERVEFILE_LANGDLL

;--------------------------------
;Macros

!macro Init type
  uac_tryagain:
  !insertmacro UAC_RunElevated
  ${Switch} $0
    ${Case} 0
      ${IfThen} $1 = 1 ${|} Quit ${|} ;we are the outer process, the inner process has done its work, we are done
      ${IfThen} $3 <> 0 ${|} ${Break} ${|} ;we are admin, let the show go on
      ${If} $1 = 3 ;RunAs completed successfully, but with a non-admin user
        MessageBox mb_YesNo|mb_IconExclamation|mb_TopMost|mb_SetForeground "This ${type} requires admin privileges, try again" /SD IDNO IDYES uac_tryagain IDNO 0
      ${EndIf}
      ;fall-through and die
    ${Case} 1223
      MessageBox mb_IconStop|mb_TopMost|mb_SetForeground "This ${type} requires admin privileges, aborting!"
      Quit
    ${Case} 1062
      MessageBox mb_IconStop|mb_TopMost|mb_SetForeground "Logon service not running, aborting!"
      Quit
    ${Default}
      MessageBox mb_IconStop|mb_TopMost|mb_SetForeground "Unable to elevate , error $0"
      Quit
  ${EndSwitch}

  SetShellVarContext all
!macroend

;--------------------------------
;Installer Sections

Section $(NAME_SecEditor) SecEditor
  SectionIn 1 2 RO

  !include "tano-install-editor.nsh"
  
  
  ;Store installation folder
  WriteRegStr HKCU "Software\Tano Editor" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
  ;Create shortcuts
  CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Tano Editor.lnk" "$INSTDIR\tano-editor.exe"
  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END
  
  WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tano Editor" "DisplayName" "Tano Editor ${VERSION} (remove only)"
  WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tano Editor" "UninstallString" '"$INSTDIR\Uninstall.exe"'

SectionEnd

Section $(NAME_SecDesk) SecDesk
  SectionIn 2
  CreateShortCut "$DESKTOP\Tano Editor.lnk" "$INSTDIR\tano-editor.exe"
SectionEnd

Section $(NAME_SecQuick) SecQuick
  SectionIn 2
  CreateShortCut "$QUICKLAUNCH\Tano Editor.lnk" "$INSTDIR\tano-editor.exe"
SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  !include "tano-language.nsh"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecEditor} $(DESC_SecEditor)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecDesk} $(DESC_SecDesk)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecQuick} $(DESC_SecQuick)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
  
;--------------------------------
;Functions

Function .onInit
  !insertmacro Init "installer"

  !insertmacro MUI_LANGDLL_DISPLAY
  
  ReadRegStr $0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tano Editor" "UninstallString"
  StrCmp $0 "" done
 
  MessageBox MB_YESNO|MB_ICONEXCLAMATION $(PRE_E) IDNO done
 
  ;Run the uninstaller
  ;uninst:
    ClearErrors
    ExecWait '$0 _?=$INSTDIR' ;Do not copy the uninstaller to a temp file
  done:

FunctionEnd

Function un.onInit
  !insertmacro Init "uninstaller"

  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd

Function PageFinishRun
  !insertmacro UAC_AsUser_ExecShell "" "$INSTDIR\tano-editor.exe" "" "" ""
FunctionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  !include "tano-uninstall-editor.nsh"

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"
  
  Delete "$DESKTOP\Tano Editor.lnk"
  Delete "$QUICKLAUNCH\Tano Editor.lnk"
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $STARTMENU_FOLDER
    
  Delete "$SMPROGRAMS\$STARTMENU_FOLDER\Tano Editor.lnk"
  Delete "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$STARTMENU_FOLDER"
  
  DeleteRegKey HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tano Editor"
  DeleteRegKey /ifempty HKCU "Software\Tano Editor"

SectionEnd
