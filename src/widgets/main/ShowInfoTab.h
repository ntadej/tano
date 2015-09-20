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

#ifndef TANO_SHOWINFOTAB_H_
#define TANO_SHOWINFOTAB_H_

#include <QtCore/QFile>
#include <QtWidgets/QMainWindow>

namespace Ui
{
    class ShowInfoTab;
}

class QLabel;

class NetworkDownload;
class XmltvCrewFilterModel;
class XmltvProgramme;

class ShowInfoTab : public QMainWindow
{
Q_OBJECT
public:
    ShowInfoTab(QWidget *parent = 0);
    ~ShowInfoTab();

protected:
	void changeEvent(QEvent *e);

public slots:
    void clear();
	void display(XmltvProgramme *programme);

signals:
    void changeTo(QWidget *);
    void requestNext(const QString &id,
                     const QString &channel);
    void requestPrevious(const QString &id,
                         const QString &channel);
    void requestRecord(const QString &id);

private slots:
    void image(QFile *file);
	void next();
	void previous();
	void record();

private:
    Ui::ShowInfoTab *ui;

	XmltvProgramme *_current;
    XmltvCrewFilterModel *_crew;

	NetworkDownload *_image;

    QLabel *_labelTitle;
};

#endif // TANO_SHOWINFOTAB_H_
