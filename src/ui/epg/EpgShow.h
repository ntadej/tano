/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_EPGSHOW_H_
#define TANO_EPGSHOW_H_

#include <QtGui/QStackedWidget>

#include "core/Enums.h"

namespace Ui
{
	class EpgShow;
}

class EpgShowInfo;
class EpgSloveniaLoader;
class GetFile;

class EpgShow : public QStackedWidget
{
Q_OBJECT
public:
	EpgShow(QWidget *parent = 0);
	~EpgShow();

protected:
	void changeEvent(QEvent *e);

public slots:
	void get(const QString &id);
	void setEpgType(const Tano::EpgType type);

private slots:
	void display(const EpgShowInfo &info);
	void image(const QString &image);
	void next();
	void previous();

private:
	QString processUrl(const QString &url) const;

	Ui::EpgShow *ui;

	QString _epgNext;
	QString _epgPrevious;
	GetFile *_image;
	EpgSloveniaLoader *_slovenia;
	Tano::EpgType _type;
};

#endif // TANO_EPGSHOW_H_
