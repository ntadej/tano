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

#ifndef TANO_TELETEXTWIDGET_H_
#define TANO_TELETEXTWIDGET_H_

#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

class TeletextWidget : public QWidget
{
Q_OBJECT
public:
    TeletextWidget(QWidget *parent = 0);
    ~TeletextWidget();

public slots:
    void toggleTeletext(const bool &on);

private slots:
    void page(const int &p);
    void teletext(const bool &on);
    void teletextTransparency(const bool &on);

private:
    QPushButton *_button;
    QPushButton *_buttonTransparency;
    QSpinBox *_page;
};

#endif // TANO_TELETEXTWIDGET_H_
