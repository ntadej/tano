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

#ifndef TANO_EPGSHOW_H_
#define TANO_EPGSHOW_H_

#include <QtGui/QWidget>

namespace Ui
{
	class EpgShow;
}

class GetFile;
class XmltvCrewFilterModel;
class XmltvProgramme;

class EpgShow : public QWidget
{
Q_OBJECT
public:
	EpgShow(QWidget *parent = 0);
	~EpgShow();

protected:
	void changeEvent(QEvent *e);

public slots:
	void display(XmltvProgramme *programme);

signals:
	void requestNext(XmltvProgramme *);
	void requestPrevious(XmltvProgramme *);
	void requestRecord(XmltvProgramme *);

private slots:
	void image(const QString &image);
	void next();
	void previous();
	void processFilters(const int &type);
	void record();

private:
	Ui::EpgShow *ui;

	XmltvProgramme *_current;
	XmltvCrewFilterModel *_crew;

	GetFile *_image;
};

#endif // TANO_EPGSHOW_H_
