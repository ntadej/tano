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

#include "application/Arguments.h"
#include "application/Common.h"
#include "application/Log.h"
#include "application/Output.h"

#include "widgets/style/Common.h"

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
    : SingleApplication(argc, argv),
      _arguments(new Arguments(argc, argv))
{
#ifdef Q_OS_MAC
    setupDockHandler();
#endif
}

TanoApplication::~TanoApplication()
{
    delete _arguments;
}

bool TanoApplication::preInit()
{
    QCoreApplication::setOrganizationDomain(Tano::domain());
    QCoreApplication::setApplicationName(Tano::name());
    QCoreApplication::setApplicationVersion(Tano::version());

    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    Tano::Log::setup();

    return true;
}

bool TanoApplication::postInit()
{
    Output::welcome();

    if (_arguments->isValid()) {
        Tano::Style::setMainStyle();
        Tano::Style::setIconPaths();
        Tano::Style::setIconName();
    } else {
        return false;
    }

    return true;
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
