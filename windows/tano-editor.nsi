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
;Include Modern UI, UAC support and UninstallLog
;--------------------------------

    !include MUI2.nsh
    !include UAC.nsh
    !include UninstallLog.nsh

;--------------------------------
; Variables
;--------------------------------

    Var STARTMENU_FOLDER

;--------------------------------
;General
;--------------------------------
; Config
    !include "InstallerConfig.nsh"

    Name "Tano Editor ${VERSION}"
    OutFile "tano_editor_${VERSION}_win32.exe"
    InstallDir "$PROGRAMFILES\Tano Editor"

    BrandingText "${COMPANY} | ${URL}"

    !define REG_APP HKCU
    !define REG_APP_PATH "Software\Tano Editor"
    !define REG_UNINSTALL HKLM
    !define REG_UNINSTALL_PATH "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tano Editor"

    InstType $(S_Minimum)
    InstType $(S_Full)

    !include "InstallerUI.nsh"

; Get installation folder from registry if available
    InstallDirRegKey ${REG_APP} "${REG_APP_PATH}" ""

    SetCompressor lzma

; Request application privileges for Windows Vista and later
    RequestExecutionLevel user

;--------------------------------
; Interface Settings
;--------------------------------

    !define MUI_ABORTWARNING

;--------------------------------
; Language Selection Dialog Settings
;--------------------------------

; Remember the installer language
    !define MUI_LANGDLL_REGISTRY_ROOT "${REG_APP}"
    !define MUI_LANGDLL_REGISTRY_KEY "${REG_APP_PATH}"
    !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;--------------------------------
; Pages
;--------------------------------

    !insertmacro MUI_PAGE_WELCOME
    !insertmacro MUI_PAGE_LICENSE "LICENSE.GPL.TXT"
    !insertmacro MUI_PAGE_COMPONENTS
    !insertmacro MUI_PAGE_DIRECTORY
  
; Start Menu Folder Page Configuration
    !define MUI_STARTMENUPAGE_REGISTRY_ROOT "${REG_APP}"
    !define MUI_STARTMENUPAGE_REGISTRY_KEY "${REG_APP_PATH}"
    !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "StartMenu"
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
; Languages
;--------------------------------

    !insertmacro MUI_LANGUAGE "English" ;first language is the default language

;--------------------------------
; Reserve Files
;--------------------------------

; If you are using solid compression, files that are required before
; the actual installation should be stored first in the data block,
; because this will make your installer start faster.
  
    !insertmacro MUI_RESERVEFILE_LANGDLL

;--------------------------------
; Macros
;--------------------------------

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
        ; fall-through and die
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
; Configure UnInstall log to only remove what is installed
;--------------------------------
; Set the name of the uninstall log
    !define UninstLog "Uninstall.log"
    Var UninstLog

; AddItem macro
    !define AddItem "!insertmacro AddItem"

; File macro
    !define File "!insertmacro File"

; CreateShortcut macro
    !define CreateShortcut "!insertmacro CreateShortcut"

; Copy files macro
    !define CopyFiles "!insertmacro CopyFiles"

; Rename macro
    !define Rename "!insertmacro Rename"

; CreateDirectory macro
    !define CreateDirectory "!insertmacro CreateDirectory"

; SetOutPath macro
    !define SetOutPath "!insertmacro SetOutPath"

; WriteUninstaller macro
    !define WriteUninstaller "!insertmacro WriteUninstaller"

; WriteRegStr macro
    !define WriteRegStr "!insertmacro WriteRegStr"

; WriteRegDWORD macro
    !define WriteRegDWORD "!insertmacro WriteRegDWORD"

Section -openlogfile
    CreateDirectory "$INSTDIR"
    IfFileExists "$INSTDIR\${UninstLog}" +3
        FileOpen $UninstLog "$INSTDIR\${UninstLog}" w
    Goto +4
        SetFileAttributes "$INSTDIR\${UninstLog}" NORMAL
        FileOpen $UninstLog "$INSTDIR\${UninstLog}" a
        FileSeek $UninstLog 0 END
SectionEnd

;--------------------------------
; Installer Sections
;--------------------------------

Section $(NAME_SecEditor) SecEditor
    SectionIn 1 2 RO

    !include "InstallerFilesEditor.nsh"
  
  
; Store installation folder
    ${WriteRegStr} ${REG_APP} "${REG_APP_PATH}" "" $INSTDIR
  
; Create uninstaller
    ${WriteUninstaller} "$INSTDIR\Uninstall.exe"
  
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
; Create shortcuts
    ${CreateDirectory} "$SMPROGRAMS\$STARTMENU_FOLDER"
    ${CreateShortCut} "$SMPROGRAMS\$STARTMENU_FOLDER\Tano Editor.lnk" "$INSTDIR\tano-editor.exe" "" "" ""
    ${CreateShortCut} "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "" ""
  
    !insertmacro MUI_STARTMENU_WRITE_END
  
    ${WriteRegStr} ${REG_UNINSTALL} "${REG_UNINSTALL_PATH}" "DisplayName" "Tano Editor ${VERSION} (remove only)"
    ${WriteRegStr} ${REG_UNINSTALL} "${REG_UNINSTALL_PATH}" "UninstallString" "$INSTDIR\Uninstall.exe"

SectionEnd

Section $(NAME_SecDesk) SecDesk
    SectionIn 2
    ${CreateShortCut} "$DESKTOP\Tano Editor.lnk" "$INSTDIR\tano-editor.exe" "" "" ""
SectionEnd

Section $(NAME_SecQuick) SecQuick
    SectionIn 2
    ${CreateShortCut} "$QUICKLAUNCH\Tano Editor.lnk" "$INSTDIR\tano-editor.exe" "" "" ""
SectionEnd

;--------------------------------
; Descriptions
;--------------------------------

; Language strings
    !include "InstallerLanguageEnglish.nsh"

; Assign language strings to sections
    !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecEditor} $(DESC_SecEditor)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDesk} $(DESC_SecDesk)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecQuick} $(DESC_SecQuick)
    !insertmacro MUI_FUNCTION_DESCRIPTION_END
  
;--------------------------------
; Functions
;--------------------------------

Function .onInit
    !insertmacro Init "installer"

    !insertmacro MUI_LANGDLL_DISPLAY
  
    ReadRegStr $0 "${REG_UNINSTALL}" "${REG_UNINSTALL_PATH}" "UninstallString"
    StrCmp $0 "" done
 
    MessageBox MB_YESNO|MB_ICONEXCLAMATION $(PRE_E) IDNO done
 
; Run the uninstaller
;   uninst:
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
; Uninstaller
;--------------------------------

Section Uninstall
; Can't uninstall if uninstall log is missing!
    IfFileExists "$INSTDIR\${UninstLog}" +3
        MessageBox MB_OK|MB_ICONSTOP "$(UninstLogMissing)"
        Abort

    Push $R0
    Push $R1
    Push $R2
    SetFileAttributes "$INSTDIR\${UninstLog}" NORMAL
    FileOpen $UninstLog "$INSTDIR\${UninstLog}" r
    StrCpy $R1 -1

    GetLineCount:
        ClearErrors
        FileRead $UninstLog $R0
        IntOp $R1 $R1 + 1
        StrCpy $R0 $R0 -2
        Push $R0
        IfErrors 0 GetLineCount

    Pop $R0

    LoopRead:
        StrCmp $R1 0 LoopDone
        Pop $R0

        IfFileExists "$R0\*.*" 0 +3
            RMDir $R0  #is dir
        Goto +9
        IfFileExists $R0 0 +3
            Delete $R0 #is file
        Goto +6
        StrCmp $R0 "${REG_APP} ${REG_APP_PATH}" 0 +3
            DeleteRegKey ${REG_APP} "${REG_APP_PATH}" #is Reg Element
        Goto +3
        StrCmp $R0 "${REG_UNINSTALL} ${REG_UNINSTALL_PATH}" 0 +2
            DeleteRegKey ${REG_UNINSTALL} "${REG_UNINSTALL_PATH}" #is Reg Element

        IntOp $R1 $R1 - 1
        Goto LoopRead
    LoopDone:
        FileClose $UninstLog
        Delete "$INSTDIR\${UninstLog}"
        RMDir "$INSTDIR"
    Pop $R2
    Pop $R1
    Pop $R0
SectionEnd
