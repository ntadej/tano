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

#ifndef TANO_EPGLIST_H_
#define TANO_EPGLIST_H_

#include <QtCore/QMap>

#if defined(Qt5)
    #include <QtWidgets/QListWidget>
#elif defined(Qt4)
    #include <QtGui/QListWidget>
#endif

class QAction;
class QMenu;
class QMouseEvent;

class XmltvProgramme;

class EpgList : public QListWidget
{
Q_OBJECT
public:
	EpgList(QWidget *parent = 0);
	~EpgList();

public slots:
	void setEpg(const QList<XmltvProgramme *> &epg);

signals:
	void itemClicked(XmltvProgramme *);

protected:
	void mouseReleaseEvent(QMouseEvent *event);

private slots:
	void clicked(QListWidgetItem *item = 0);

private:
	QMap<QListWidgetItem *, XmltvProgramme *> _list;

	QAction *_info;
	QAction *_record;
	QMenu *_rightMenu;
};

#endif // TANO_EPGLIST_H_
