/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QStyleFactory>

#include "Config.h"

#include "core/Resources.h"
#include "core/plugins/Plugins.h"
#include "core/settings/Settings.h"

#include "style/Common.h"
#include "style/ManhattanStyle.h"
#include "style/StyleHelper.h"

void Tano::Style::setMainStyle()
{
#ifdef Q_OS_MAC
    qApp->setAttribute(Qt::AA_DontShowIconsInMenus);
    qApp->setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    QString baseName = QApplication::style()->objectName();
#ifdef Q_OS_LINUX
    if (baseName == QLatin1String("windows")) {
        // Sometimes we get the standard windows 95 style as a fallback
        if (QStyleFactory::keys().contains(QLatin1String("Fusion")))
            baseName = QLatin1String("fusion"); // Qt5
        else { // Qt4
            // e.g. if we are running on a KDE4 desktop
            QByteArray desktopEnvironment = qgetenv("DESKTOP_SESSION");
            if (desktopEnvironment == "kde")
                baseName = QLatin1String("plastique");
            else
                baseName = QLatin1String("cleanlooks");
        }
    }
#endif
    qApp->setStyle(new ManhattanStyle(baseName));

    StyleHelper::setBaseColor(qApp->palette().color(QPalette::Highlight));
}

void Tano::Style::setIconName()
{
    QScopedPointer<Settings> settings(new Settings());
    if (!settings->icons().isEmpty()) {
        QString name = settings->icons();
#if defined(Q_OS_MAC)
        if (qApp->devicePixelRatio() >= 2)
            name.append("@2x");
#endif
        QIcon::setThemeName(name);
    }

    qDebug() << "Icon theme:" << QIcon::themeName();
}

void Tano::Style::setIconPaths()
{
    QStringList paths = QIcon::themeSearchPaths();
    paths.prepend(QCoreApplication::applicationDirPath() + "/" + "icons");

    if (globalConfig) paths.prepend(globalConfig->applicationDataDir() + "/" + "icons");

#if defined(Q_OS_MAC)
    paths.prepend(QCoreApplication::applicationDirPath().replace("MacOS", "Resources") + "/icons");

    QDir bundleDir(QCoreApplication::applicationDirPath());
    bundleDir.cdUp();
    bundleDir.cdUp();
    bundleDir.cdUp();
    paths.append(bundleDir.path() + "/icons");
#endif

    QIcon::setThemeSearchPaths(paths);

    qDebug() << "Icon theme search paths:" << QIcon::themeSearchPaths();
}
