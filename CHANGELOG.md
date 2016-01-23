# Tano Changelog

## 1.99.3 (2016-01-23)
 - Sparkle and WinSparkle support for updates
 - XMLTV time offset setting
 - Various crash fixes
 - Ubuntu packages
 - Fix: Wrong maximum udpxy port (issue #49)

## 1.99.2 (2013-11-23)
 - Updated recorder with ability to change timer's channel
 - OS X version now supports Mavericks

## 1.99.1 (2013-10-05)
 - Beta 1 release for Tano 2.0
 - New modern user interface
 - OS X support
 - Store XMLTV information in a database

## 1.2.1 (2013-02-08)
 - Updated Czech and Ukrainian translation
 - Fix: Consistently show/hide mouse cursor
 - Fix: Fix mute on exit
 - Fix: Install missing DLL in Windows

## 1.2 (2013-01-26)
 - Separated Tano Core and Tano Widgets library
 - Updated and improved OSD and fullscreen handling
 - Automatically map XMLTV IDs to channels in editor
 - Map standard XMLTV categories to translations
 - Full MSVC and Qt5 support
 - Experimental QML UI - Qt5 only (in a git submodule)
 - Fix: Don't use channel-based settings for other media
 - Fix: Use correct Tab order in all UI files
 - Fix: Instant recording works again (issue #39)

## 1.1 (2012-10-14)
 - Command line arguments (see tano --help)
 - New channel types
 - Scale/zoom options
 - Audio and video output selection
 - S/PDIF output
 - YUV -> RGB hardware accelerated convertion on Windows (turn on/off)
 - Select network interface
 - Ability to mute on minimize
 - Ability to preview channels
 - Option for filters to be always visible
 - Save main window size, position and visible elements
 - Icon theme support
 - Drop support for older versions of Qt and VLC
    Supported are Qt 4.8 or later and VLC 2.0 or later.
 - Removed splash screen and Windows installer branding.
 - Removed standalone editor

## 1.0.3 (2012-08-20)
 - Update: Changed playlist OSD position in fullscreen
 - Fix: Playlist selection crash
 - Fix: Remember visible items after lite mode
 - Fix: Session volume settings
 - Fix: Save to all available formats

## 1.0.2 (2012-08-10)
 - Fix: Encoding issues
 - Fix: Don't move playlist widget, if item is visible

## 1.0.1 (2012-07-13)
 - Various XMLTV improvements and fixes
 - Fix: Language detection
 - Fix: Scrolling widgets don't get stuck on resize
 - Fix: Don't autoplay before main window is visible
 - Fix: Open action in main toolbar

## 1.0 (2012-07-02)
 - Ability to take snapshots
 - Separated settings for player and editor
 - Removed first-run wizard
 - Fullscreen OSD improvements
 - Teletext is disabled by default
 - Updated translations
 - [WINDOWS] VLC 2.0.2

## 1.0-beta2 (2012-05-27)
 - Some video playback updates
 - OSD improvements
 - GUI optimizations for small screens
 - Fix compilation issues on X11
 - Fixed Issue #30 - sync channel changes
 - Basic Qt5 support

## 1.0-beta1 (2012-05-03)
 - Single instance support for Tano
 - Recorder (Daemon) merged with main application
 - XMLTV support (Issue #20)
 - Rewritten data models
 - Rewritten recorder
 - Updated channels scan
 - Updated file dialogs
 - Copyright issues fix - Tano will no longer ship playlists and integrated EPG
 - Overall fixes and improvements
 - VLC 2.0 support
 - Czech, French, Russian, Slovenian and Ukrainian translation

## 0.8.1 (2011-06-27)
 - Teletext support
 - CSV playlist export/import
 - UDPXY support (Issue #24)
 - Recorder stabilisation
 - Standalone Tano Editor improvements
 - [LINUX] Qt 4.6 support
 - [LINUX] Ubuntu 10.04 support
 - [WINDOWS] VLC 1.1.10

## 0.8.0 (2011-04-03)
 - Completely rewriten Recorder and Timers Manager
 - Language filter for playlists
 - Keyboard shortcuts for media controls
 - Playlist / Info OSD in fullscreen
 - Playlist export to Sagem JS format
 - Web Playlist import
 - EPG fixes
 - New icon theme
 - Russian language support + playlists
 - GUI Updates
 - [FIX] Bug #2960518 - Timers - errors
 - [FIX] Bug #2980268 - Progress Buffer
 - [FIX] Bug #2985475 - Recoding doesnt works under Windows
 - [FIX] Bug #2995052 - Aspect Ratio
 - [FIX] Bug #3088149 - Empty channel list -> crash
 - [FIX] Bug #3088128 - Crash when starting with own playlist

## 0.7.3 (2010-10-14)
 - VLC-Qt library support
 - New Updates manager
 - Major code cleanup
 - Resources updated
 - [WINDOWS] Installation scripts updated

## 0.7.2 (2010-09-17)
 - Settings backend changed
 - Czech language
 - World TV playlist
 - Schedule for all channels independent from playback
 - Channel logo support
 - Channel list printing
 - Playlist channels scan
 - Epg fixes
 - Maximise/minimise updates
 - Overall improvements and GUI updates
 - [FIX] Bug #2979690 - Channel Logos
 - [FIX] Bug #2969835 - Thread termination error
 - [FIX] Bug #2979976 - Channel Editor wont close mroe
 - [FIX] Bug #2979977 - Channel Editor Button for Up/Down
 - [WINDOWS] VLC updated to 1.1.4

## 0.7.1 (2010-03-21)
 - EPG backend with plugin support
 - Minor GUI updates and fixes
 - [FIX] Bug #2960715 - channel list
 - [FIX] Bug #2960314 - Doesn't remember volume
 - [FIX] Bug #2960315 - Windows compatibility

## 0.7.0 (2010-02-25)
 - Timers support
 - New modified M3U format
 - New OSD
 - Playlist filters added
 - Default subtitle language setting
 - Ability to store video settings when changing media
 - Tuš Telekom support (SLO)
 - Plugins support (currently only for recorder, EPG support comming)
 - Code cleanup and optimisation
 - Updated About dialog
 - Updated playlist editor
