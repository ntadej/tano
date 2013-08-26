/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#include <QtCore/QDir>
#include <QtCore/QTextCodec>

#include "core/Common.h"
#include "core/Resources.h"
#include "core/application/Arguments.h"
#include "core/application/Log.h"
#include "core/application/Output.h"
#include "core/plugins/Plugins.h"

#include "widgets/dialogs/PasswordDialog.h"
#include "widgets/style/Common.h"

#include "TanoApplication.h"

TanoApplication::TanoApplication(int argc, char *argv[])
    : SingleApplication(argc, argv),
      _arguments(new Arguments(argc, argv)) { }

TanoApplication::~TanoApplication()
{
    delete _arguments;
}

bool TanoApplication::preInit()
{
#if defined(Q_OS_MAC) && defined(QT_NO_DEBUG) // Fix plugin detection
    QDir dir(argv[0]);  // e.g. appdir/Contents/MacOS/appname
    dir.cdUp();
    dir.cdUp();
    dir.cd("PlugIns");  // e.g. appdir/Contents/PlugIns
    QCoreApplication::setLibraryPaths(QStringList(dir.absolutePath()));
#endif

    Tano::Plugins::initConfig();
    Tano::Plugins::initNetwork();

    QCoreApplication::setApplicationName(Tano::name());
    QCoreApplication::setApplicationVersion(Tano::version());

    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);

#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

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

        if (globalConfig && globalConfig->requiresAuthentication()) {
            PasswordDialog dialog;
            switch (dialog.exec())
            {
            case QDialog::Accepted:
                break;
            default:
                return false;
                break;
            }
        }
    } else {
        return false;
    }

    return true;
}
