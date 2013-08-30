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

#ifndef TANO_PLAYLISTEXPORTTVHEADEND_H_
#define TANO_PLAYLISTEXPORTTVHEADEND_H_

#if defined(Qt5)
    #include <QtWidgets/QDialog>
#elif defined(Qt4)
    #include <QtGui/QDialog>
#endif

namespace Ui {
	class PlaylistExportTvheadend;
}

class QAbstractButton;

class PlaylistExportTvheadend : public QDialog
{
Q_OBJECT
public:
	PlaylistExportTvheadend(QWidget *parent = 0);
	~PlaylistExportTvheadend();

	QString interface() const;
	QString location() const;
	bool proceed() const { return _proceed; }

protected:
	void changeEvent(QEvent *e);

private slots:
	void action(QAbstractButton *button);
    void browse();

private:
	Ui::PlaylistExportTvheadend *ui;

	bool _proceed;
};

#endif // TANO_PLAYLISTEXPORTTVHEADEND_H_
