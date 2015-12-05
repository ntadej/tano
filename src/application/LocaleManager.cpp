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

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QLocale>

#include "application/LocaleManager.h"
#include "common/Resources.h"
#include "settings/Settings.h"

LocaleManager::LocaleManager(QObject *parent)
    : QObject(parent)
{
    _translator = new QTranslator();
    _locale = "";
    QCoreApplication::installTranslator(_translator);
    setLocale();
}

LocaleManager::~LocaleManager()
{
    QCoreApplication::removeTranslator(_translator);
    delete _translator;
}

QStringList LocaleManager::loadLocales()
{
    QDir dir(Tano::Resources::path("/i18n/sl.qm"));
    QStringList list;
    list << QLocale(QLocale::English, QLocale::UnitedStates).name();

    foreach (const QString &fileName, dir.entryList(QDir::Files)) {
        if (fileName.contains(".qm") && !fileName.contains("source")) {
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
    if(settings->locale().isEmpty()) {
        locale = QLocale::system().name();

        if (Tano::Resources::resource("/i18n/" + locale + ".qm").isEmpty())
            locale = locale.split("_")[0];
    } else {
        locale = settings->locale();
    }

    qDebug() << "Using locale" << locale;

    QString langPath = Tano::Resources::path("/i18n/" + locale + ".qm");
    _translator->load(locale, langPath);
    _locale = locale;

    emit localeChanged();
}
