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

#include "core/LocaleManager.h"
#include "core/Resources.h"
#include "core/settings/Settings.h"

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
    QDir dir(Tano::Resources::path("/lang/sl.qm"));
    QStringList list;
    list << QLocale::languageToString(QLocale(QLocale::English).language());

    foreach (const QString &fileName, dir.entryList(QDir::Files)) {
        if(fileName.contains(".qm") && !fileName.contains("source")) {
            list << localeName(fileName);
        }
    }

    return list;
}

QString LocaleManager::localeName(const QString &file)
{
    QLocale locale = QLocale(file);
    QString name = locale.name();
    QString f = file;
    f = f.replace(".qm", "");

    if (name != f)
        name = f;

    return name;
}

void LocaleManager::setLocale()
{
    QString locale;
    QScopedPointer<Settings> settings(new Settings());
    if(settings->language().isEmpty()) {
        locale = QLocale::system().name();

        if (Tano::Resources::resource("/lang/" + locale + ".qm").isEmpty())
            locale = locale.split("_")[0];
    } else {
        locale = settings->language();
    }

    QString langPath = Tano::Resources::path("/lang/" + locale + ".qm");
    _translator->load(locale, langPath);
}
