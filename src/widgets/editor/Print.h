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

#ifndef TANO_PRINT_H_
#define TANO_PRINT_H_

#include <QtWidgets/QTextEdit>

class QPrinter;

class PlaylistModel;

class Print : public QTextEdit
{
Q_OBJECT
public:
    Print(QWidget *parent = 0);
    ~Print();

    void channelList(const QString &name,
                     PlaylistModel *model,
                     const QList<int> &list);

private slots:
    void preview(QPrinter *printer);

private:
    void start();

    QTextCharFormat _boldFormat;
    QTextTableFormat _tableFormat;
    QTextCharFormat _textFormat;
    QTextCharFormat _titleFormat;
};

#endif // TANO_PRINT_H_
