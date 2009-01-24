#
# This Makefile is used by targets deb and rpm.
#
# Targets deb and rpm create respectively Debian and RPM packages
# for QuarkPlayer. This Makefile cannot be used without CMake.
# Target package from CMake is not powerful enough cf
# http://article.gmane.org/gmane.comp.programming.tools.cmake.user/16242
# See:
# rpm/CMakeLists.txt
# rpm/quarkplayer.spec.in
# debian/CMakeLists.txt
# debian/rules.in
# debian/control.in
#

# Can be changed to /usr/local for example
PREFIX = /usr

# Output directory of CMake install target
#BUILD_DIR = build/linux-gcc43-release

# Directory where to run make
#CMAKE_BINARY_DIR = build

all: quarkplayer-src

configure:
	cd $(CMAKE_BINARY_DIR) && ./build_make-release.sh

#Cannot name this target 'quarkplayer' since a directory has this name already
quarkplayer-src:
	cd $(CMAKE_BINARY_DIR) && $(MAKE) install

clean:
	cd $(CMAKE_BINARY_DIR) && $(MAKE) clean

install:

	#Binary
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(BUILD_DIR)/quarkplayer $(DESTDIR)$(PREFIX)/bin

	#Internal libraries
	install -d $(DESTDIR)$(PREFIX)/lib/quarkplayer
	install -m 644 $(BUILD_DIR)/*.so* $(DESTDIR)$(PREFIX)/lib/quarkplayer

	#Plugins
	install -d $(DESTDIR)$(PREFIX)/lib/quarkplayer/plugins
	install -m 644 $(BUILD_DIR)/plugins/*.so* $(DESTDIR)$(PREFIX)/lib/quarkplayer/plugins

	#MPlayer Phonon backend
	install -d $(DESTDIR)$(PREFIX)/lib/quarkplayer/phonon_backend
	install -m 644 $(BUILD_DIR)/phonon_backend/*.so* $(DESTDIR)$(PREFIX)/lib/quarkplayer/phonon_backend

	#Qt styles
	install -d $(DESTDIR)$(PREFIX)/lib/quarkplayer/styles
	install -m 644 $(BUILD_DIR)/styles/*.so* $(DESTDIR)$(PREFIX)/lib/quarkplayer/styles

	#Translations
	install -d $(DESTDIR)$(PREFIX)/share/quarkplayer/translations
	install -m 644 $(BUILD_DIR)/translations/*.qm $(DESTDIR)$(PREFIX)/share/quarkplayer/translations

	#Documentation
	install -d $(DESTDIR)$(PREFIX)/share/quarkplayer/doc
	install -m 644 $(BUILD_DIR)/README $(DESTDIR)$(PREFIX)/share/quarkplayer/doc
	install -m 644 $(BUILD_DIR)/AUTHORS $(DESTDIR)$(PREFIX)/share/quarkplayer/doc
	install -m 644 $(BUILD_DIR)/ChangeLog $(DESTDIR)$(PREFIX)/share/quarkplayer/doc
	install -m 644 $(BUILD_DIR)/COPYING $(DESTDIR)$(PREFIX)/share/quarkplayer/doc
	install -m 644 $(BUILD_DIR)/COPYING.LESSER $(DESTDIR)$(PREFIX)/share/quarkplayer/doc
	install -m 644 $(BUILD_DIR)/THANKS $(DESTDIR)$(PREFIX)/share/quarkplayer/doc

	#Man pages
	install -d $(DESTDIR)$(PREFIX)/share/man/man1

	#.desktop file
	install -d $(DESTDIR)$(PREFIX)/share/applications
	install -m 644 quarkplayer-app/quarkplayer.desktop $(DESTDIR)$(PREFIX)/share/applications

	#KDE Icons
	install -d $(DESTDIR)$(PREFIX)/share/icons/hicolor/16x16/apps
	install -d $(DESTDIR)$(PREFIX)/share/icons/hicolor/22x22/apps
	install -d $(DESTDIR)$(PREFIX)/share/icons/hicolor/32x32/apps
	install -d $(DESTDIR)$(PREFIX)/share/icons/hicolor/48x48/apps
	install -d $(DESTDIR)$(PREFIX)/share/icons/hicolor/64x64/apps
	install -d $(DESTDIR)$(PREFIX)/share/icons/hicolor/128x128/apps
	install -d $(DESTDIR)$(PREFIX)/share/icons/hicolor/scalable/apps
	install -m 644 quarkplayer-app/icons/hi16-app-quarkplayer.png $(DESTDIR)$(PREFIX)/share/icons/hicolor/16x16/apps/quarkplayer.png
	install -m 644 quarkplayer-app/icons/hi22-app-quarkplayer.png $(DESTDIR)$(PREFIX)/share/icons/hicolor/22x22/apps/quarkplayer.png
	install -m 644 quarkplayer-app/icons/hi32-app-quarkplayer.png $(DESTDIR)$(PREFIX)/share/icons/hicolor/32x32/apps/quarkplayer.png
	install -m 644 quarkplayer-app/icons/hi48-app-quarkplayer.png $(DESTDIR)$(PREFIX)/share/icons/hicolor/48x48/apps/quarkplayer.png
	install -m 644 quarkplayer-app/icons/hi64-app-quarkplayer.png $(DESTDIR)$(PREFIX)/share/icons/hicolor/64x64/apps/quarkplayer.png
	install -m 644 quarkplayer-app/icons/quarkplayer.svgz $(DESTDIR)$(PREFIX)/share/icons/hicolor/scalable/apps/quarkplayer.svgz

uninstall:

	#Binary
	rm -f $(DESTDIR)$(PREFIX)/bin/quarkplayer

	#Internal libraries
	rm -f $(DESTDIR)$(PREFIX)/lib/quarkplayer/*.so*

	#Plugins
	rm -f $(DESTDIR)$(PREFIX)/lib/quarkplayer/plugins/*.so*

	#MPlayer Phonon backend
	rm -f $(DESTDIR)$(PREFIX)/lib/quarkplayer/phonon_backend/*.so*

	#Qt styles
	rm -f $(DESTDIR)$(PREFIX)/lib/quarkplayer/styles/*.so*

	#Translations
	rm -f $(DESTDIR)$(PREFIX)/share/quarkplayer/translations/*.qm

	#Documentation
	rm -f $(DESTDIR)$(PREFIX)/share/quarkplayer/doc/*

	#Man pages
	rm -f $(DESTDIR)$(PREFIX)/share/man/man1/quarkplayer.1.gz

	#.desktop file
	rm -f $(DESTDIR)$(PREFIX)/share/applications/quarkplayer.desktop

	#KDE Icons
	rm -f $(DESTDIR)$(PREFIX)/share/icons/hicolor/16x16/apps/quarkplayer.png
	rm -f $(DESTDIR)$(PREFIX)/share/icons/hicolor/22x22/apps/quarkplayer.png
	rm -f $(DESTDIR)$(PREFIX)/share/icons/hicolor/32x32/apps/quarkplayer.png
	rm -f $(DESTDIR)$(PREFIX)/share/icons/hicolor/48x48/apps/quarkplayer.png
	rm -f $(DESTDIR)$(PREFIX)/share/icons/hicolor/64x64/apps/quarkplayer.png
	rm -f $(DESTDIR)$(PREFIX)/share/icons/hicolor/128x128/apps/quarkplayer.png
	rm -f $(DESTDIR)$(PREFIX)/share/icons/hicolor/scalable/apps/quarkplayer.svgz
