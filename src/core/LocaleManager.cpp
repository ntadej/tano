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
#include <QtCore/QDir>
#include <QtCore/QLocale>

#include "core/Common.h"
#include "core/LocaleManager.h"
#include "core/Settings.h"

LocaleManager::LocaleManager()
{
    _translator = new QTranslator();
    QCoreApplication::installTranslator(_translator);
    setLocale();
}

LocaleManager::~LocaleManager()
{
    QCoreApplication::removeTranslator(_translator);
    delete _translator;
}

QString LocaleManager::language(const QString &locale)
{
    QString language = QLocale::languageToString(QLocale(locale).language());

    if(language == "C")
        return "English";
    else
        return language;
}

QStringList LocaleManager::loadTranslations()
{
    QDir dir(Tano::locateResource("/lang/sl.qm").replace("/sl.qm", ""));
    QStringList list;
    QLocale locale = QLocale::English;
    list << QLocale::languageToString(locale.language());

    foreach (QString fileName, dir.entryList(QDir::Files)) {
        if(fileName.contains(".qm") && !fileName.contains("source")) {
            locale = QLocale(fileName);
            list << locale.name();
        }
    }

    return list;
}

void LocaleManager::setLocale()
{
    QString locale;
    QScopedPointer<Settings> settings(new Settings());
    if(settings->language().isEmpty())
        locale = QLocale::system().name();
    else
        locale = QLocale(settings->language()).name();

    QString langPath = Tano::locateResource("/lang/" + locale + ".qm").replace("/" + locale + ".qm", "");
    _translator->load(QString(locale), langPath);
}
