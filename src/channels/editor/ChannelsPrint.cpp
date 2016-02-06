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

#include <QtCore/QDate>
#include <QtGui/QTextFrame>
#include <QtGui/QTextTable>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintPreviewDialog>

#include "application/Common.h"
#include "channels/containers/Channel.h"
#include "channels/models/ChannelsModel.h"

#include "ChannelsPrint.h"

ChannelsPrint::ChannelsPrint(QWidget *parent)
    : QTextEdit(parent)
{
    _boldFormat.setFontWeight(QFont::Bold);
    _titleFormat.setFontWeight(QFont::Bold);
    _titleFormat.setFontPointSize(20);

    _tableFormat.setCellSpacing(0);
    _tableFormat.setCellPadding(5);
    _tableFormat.setBorder(10); // Bug in printer
    _tableFormat.setBorderBrush(QBrush(QColor("black"), Qt::SolidPattern));
    _tableFormat.setBorderStyle(QTextTableFormat::BorderStyle_Solid);
}

ChannelsPrint::~ChannelsPrint() { }

void ChannelsPrint::channelList(const QString &name,
                        ChannelsModel *model,
                        const QList<int> &list)
{
    QTextCursor cursor(textCursor());
    cursor.movePosition(QTextCursor::Start);
    QTextFrame *topFrame = cursor.currentFrame();

    cursor.insertText(name, _titleFormat);
    cursor.insertBlock();

    int max = 0;
    foreach(int i, list) {
        if (i >= 0)
            max++;
    }

    QTextTable *channelsTable = cursor.insertTable(1, max, _tableFormat);

    if(list[0] >= 0) {
        cursor = channelsTable->cellAt(0, list[0]).firstCursorPosition();
        cursor.insertText(tr("#"), _boldFormat);
    }
    if(list[1] >= 0) {
        cursor = channelsTable->cellAt(0, list[1]).firstCursorPosition();
        cursor.insertText(tr("Channel"), _boldFormat);
    }
    if(list[2] >= 0) {
        cursor = channelsTable->cellAt(0, list[2]).firstCursorPosition();
        cursor.insertText(tr("Type"), _boldFormat);
    }
    if(list[3] >= 0) {
        cursor = channelsTable->cellAt(0, list[3]).firstCursorPosition();
        cursor.insertText(tr("Url"), _boldFormat);
    }
    if(list[4] >= 0) {
        cursor = channelsTable->cellAt(0, list[4]).firstCursorPosition();
        cursor.insertText(tr("Categories"), _boldFormat);
    }
    if(list[5] >= 0) {
        cursor = channelsTable->cellAt(0, list[5]).firstCursorPosition();
        cursor.insertText(tr("Language"), _boldFormat);
    }
    if(list[6] >= 0) {
        cursor = channelsTable->cellAt(0, list[6]).firstCursorPosition();
        cursor.insertText(tr("XMLTV ID"), _boldFormat);
    }
    if(list[7] >= 0) {
        cursor = channelsTable->cellAt(0, list[7]).firstCursorPosition();
        cursor.insertText(tr("Logo"), _boldFormat);
    }

    for (int i = 0; i < model->rowCount(); ++i) {
        int row = channelsTable->rows();
        channelsTable->insertRows(row, 1);

        if(list[0] >= 0) {
            cursor = channelsTable->cellAt(row, list[0]).firstCursorPosition();
            cursor.insertText(QString::number(model->row(i)->number()), _textFormat);
        }
        if(list[1] >= 0) {
            cursor = channelsTable->cellAt(row, list[1]).firstCursorPosition();
            cursor.insertText(model->row(i)->name(), _textFormat);
        }
        if(list[2] >= 0) {
            cursor = channelsTable->cellAt(row, list[2]).firstCursorPosition();
            cursor.insertText(Channel::types()[model->row(i)->type()], _textFormat);
        }
        if(list[3] >= 0) {
            cursor = channelsTable->cellAt(row, list[3]).firstCursorPosition();
            cursor.insertText(model->row(i)->url(), _textFormat);
        }
        if(list[4] >= 0) {
            cursor = channelsTable->cellAt(row, list[4]).firstCursorPosition();
            cursor.insertText(model->row(i)->categories().join(", "), _textFormat);
        }
        if(list[5] >= 0) {
            cursor = channelsTable->cellAt(row, list[5]).firstCursorPosition();
            cursor.insertText(model->row(i)->language(), _textFormat);
        }
        if(list[6] >= 0) {
            cursor = channelsTable->cellAt(row, list[6]).firstCursorPosition();
            cursor.insertText(model->row(i)->xmltvId(), _textFormat);
        }
        if(list[7] >= 0) {
            cursor = channelsTable->cellAt(row, list[7]).firstCursorPosition();
            cursor.insertText(model->row(i)->logo(), _textFormat);
        }
    }

    cursor.setPosition(topFrame->lastPosition());
    cursor.insertBlock();
    cursor.insertBlock();

    cursor.insertText(tr("Exported with %1 on %2.").arg(Tano::name(), QDate::currentDate().toString("d MMMM yyyy")), _textFormat);
    cursor.insertBlock();

    start();
}

void ChannelsPrint::start()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(preview(QPrinter*)));
    preview.exec();
#endif
}

void ChannelsPrint::preview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    print(printer);
#endif
}
