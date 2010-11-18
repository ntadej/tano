/****************************************************************************
* LocaleManager.cpp: Translations manager
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
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
	QDir dir(Tano::locateResource("/lang/tano_sl_SI.qm").replace("/tano_sl_SI.qm", ""));
	QStringList list;
	QLocale locale = QLocale::English;
	list << QLocale::languageToString(locale.language());

	foreach (QString fileName, dir.entryList(QDir::Files)) {
		if(fileName.contains(".qm")) {
			locale = QLocale(fileName.replace("tano_",""));
			list << locale.name();
		}
	}

	return list;
}

void LocaleManager::setLocale()
{
	QString locale;
	Settings *settings = new Settings();
	if(settings->language().isEmpty())
		locale = QLocale::system().name();
	else
		locale = QLocale(settings->language()).name();
	delete settings;

	QString langPath = Tano::locateResource("/lang/tano_" + locale + ".qm").replace("/tano_" + locale + ".qm", "");
	_translator->load(QString("tano_" + locale), langPath);
}
