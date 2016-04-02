/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
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

#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtWidgets/QStyleFactory>

#include "application/Common.h"
#include "application/Log.h"
#include "application/Output.h"
#include "common/Resources.h"
#include "style/ManhattanStyle.h"
#include "style/StyleHelper.h"
#include "style/Theme.h"
#include "style/Theme_p.h"

#include "TanoApplication.h"

#ifdef Q_OS_MAC
#include <objc/objc.h>
#include <objc/message.h>

bool dockClickHandler(id self,
                      SEL cmd,
                      ...)
{
    Q_UNUSED(self)
    Q_UNUSED(cmd)

    ((TanoApplication *) qApp)->onClickOnDock();

    return true;
}
#endif

TanoApplication::TanoApplication(int &argc,
                                 char **argv)
    : SingleApplication(argc, argv)
{
#ifdef Q_OS_MAC
    setupDockHandler();
#endif
}

TanoApplication::~TanoApplication() { }

void TanoApplication::preInit()
{
    QCoreApplication::setOrganizationDomain(Tano::domain());
    QCoreApplication::setApplicationName(Tano::name());
    QCoreApplication::setApplicationVersion(Tano::version());

    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    Tano::Log::setup();
}

void TanoApplication::postInit()
{
    Output::welcome();

    setMainTheme();
    setMainStyle();
}

void TanoApplication::onClickOnDock()
{
    emit dockClicked();
}

#ifdef Q_OS_MAC
void TanoApplication::setupDockHandler()
{
    Class cls = objc_getClass("NSApplication");
    objc_object *appInst = objc_msgSend((objc_object*)cls, sel_registerName("sharedApplication"));

    if (appInst != NULL) {
        objc_object* delegate = objc_msgSend(appInst, sel_registerName("delegate"));
        Class delClass = (Class)objc_msgSend(delegate,  sel_registerName("class"));
        SEL shouldHandle = sel_registerName("applicationShouldHandleReopen:hasVisibleWindows:");
        if (class_getInstanceMethod(delClass, shouldHandle)) {
            if (class_replaceMethod(delClass, shouldHandle, (IMP)dockClickHandler, "B@:"))
                qDebug() << "Registered dock click handler (replaced original method)";
            else
                qWarning() << "Failed to replace method for dock click handler";
        } else {
            if (class_addMethod(delClass, shouldHandle, (IMP)dockClickHandler,"B@:"))
                qDebug() << "Registered dock click handler";
            else
                qWarning() << "Failed to register dock click handler";
        }
    }
}
#endif

void TanoApplication::setMainStyle()
{
#ifdef Q_OS_MAC
    setAttribute(Qt::AA_DontShowIconsInMenus);
#endif

    QString baseName = style()->objectName();
    // Sometimes we get the standard windows 95 style as a fallback
#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
    if (baseName == QLatin1String("windows")) {
        baseName = QLatin1String("fusion");
    }
#endif

    // if the user has specified as base style in the theme settings,
    // prefer that
    const QStringList available = QStyleFactory::keys();
    foreach (const QString &s, Tano::applicationTheme()->preferredStyles()) {
        if (available.contains(s, Qt::CaseInsensitive)) {
            baseName = s;
            break;
        }
    }
    setStyle(new ManhattanStyle(baseName));

    StyleHelper::setBaseColor(palette().color(QPalette::Highlight));
}

void TanoApplication::setMainTheme()
{
    QSettings themeSettings(Tano::Resources::resource("style/DefaultTheme.ini"), QSettings::IniFormat);
    Theme *theme = new Theme("Default", qApp);
    theme->readSettings(themeSettings);
    if (theme->flag(Theme::ApplyThemePaletteGlobally))
        setPalette(theme->palette());
    Tano::setApplicationTheme(theme);
}
