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

#ifndef TANO_LOCALEMANAGER_H_
#define TANO_LOCALEMANAGER_H_

#include <QtCore/QString>
#include <QtCore/QTranslator>

class LocaleManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString R READ retranslateQml NOTIFY localeChanged)
    Q_PROPERTY(QString locale READ locale NOTIFY localeChanged)
public:
    LocaleManager(QObject *parent = 0);
	~LocaleManager();

    static QStringList loadLocales();
	static QString localeName(const QString &file);

signals:
    void localeChanged();

public slots:
	void setLocale();

private:
    inline QString retranslateQml() { return ""; }
    inline QString locale() { return _locale; }

	QTranslator *_translator;
    QString _locale;
};

#endif // TANO_LOCALEMANAGER_H_
