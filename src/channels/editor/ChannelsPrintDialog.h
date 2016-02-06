/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_CHANNELSPRINTDIALOG_H_
#define TANO_CHANNELSPRINTDIALOG_H_

#include <QtWidgets/QDialog>

namespace Ui {
    class ChannelsPrintDialog;
}

class QAbstractButton;

class ChannelsModel;
class ChannelsPrint;

class ChannelsPrintDialog : public QDialog
{
    Q_OBJECT
public:
    ChannelsPrintDialog(const QString &name,
                        ChannelsModel *model,
                        QWidget *parent = 0);
    ~ChannelsPrintDialog();

protected:
    void changeEvent(QEvent *e);

private slots:
    void action(QAbstractButton *button);

private:
    void print();

    Ui::ChannelsPrintDialog *ui;

    ChannelsPrint *_print;

    QString _name;
    ChannelsModel *_model;
};

#endif // TANO_CHANNELSPRINTDIALOG_H_
