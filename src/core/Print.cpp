/****************************************************************************
* Print.cpp: Main print class
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#include <QtCore/QDate>
#include <QtGui/QPrinter>
#include <QtGui/QPrintPreviewDialog>
#include <QtGui/QTextFrame>
#include <QtGui/QTextTable>

#include "Print.h"

Print::Print()
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

Print::~Print()
{

}

void Print::channelList(const QString &name, PlaylistWidget *widget)
{
	QTextCursor cursor(textCursor());
	cursor.movePosition(QTextCursor::Start);
	QTextFrame *topFrame = cursor.currentFrame();

	cursor.insertText(name, _titleFormat);
	cursor.insertBlock();

	QTextTable *channelsTable = cursor.insertTable(1, 7, _tableFormat);

	cursor = channelsTable->cellAt(0, 0).firstCursorPosition();
	cursor.insertText(tr("#"), _boldFormat);
	cursor = channelsTable->cellAt(0, 1).firstCursorPosition();
	cursor.insertText(tr("Channel"), _boldFormat);
	cursor = channelsTable->cellAt(0, 2).firstCursorPosition();
	cursor.insertText(tr("Url"), _boldFormat);
	cursor = channelsTable->cellAt(0, 3).firstCursorPosition();
	cursor.insertText(tr("Categories"), _boldFormat);
	cursor = channelsTable->cellAt(0, 4).firstCursorPosition();
	cursor.insertText(tr("Language"), _boldFormat);
	cursor = channelsTable->cellAt(0, 5).firstCursorPosition();
	cursor.insertText(tr("EPG"), _boldFormat);
	cursor = channelsTable->cellAt(0, 6).firstCursorPosition();
	cursor.insertText(tr("Logo"), _boldFormat);

	for (int i = 0; i < widget->treeWidget()->topLevelItemCount(); ++i) {
		int row = channelsTable->rows();
		channelsTable->insertRows(row, 1);

		cursor = channelsTable->cellAt(row, 0).firstCursorPosition();
		cursor.insertText(widget->channelRead(widget->treeWidget()->topLevelItem(i))->numberString(), _textFormat);
		cursor = channelsTable->cellAt(row, 1).firstCursorPosition();
		cursor.insertText(widget->channelRead(widget->treeWidget()->topLevelItem(i))->name(), _textFormat);
		cursor = channelsTable->cellAt(row, 2).firstCursorPosition();
		cursor.insertText(widget->channelRead(widget->treeWidget()->topLevelItem(i))->url(), _textFormat);
		cursor = channelsTable->cellAt(row, 3).firstCursorPosition();
		cursor.insertText(widget->channelRead(widget->treeWidget()->topLevelItem(i))->categories().join(", "), _textFormat);
		cursor = channelsTable->cellAt(row, 4).firstCursorPosition();
		cursor.insertText(widget->channelRead(widget->treeWidget()->topLevelItem(i))->language(), _textFormat);
		cursor = channelsTable->cellAt(row, 5).firstCursorPosition();
		cursor.insertText(widget->channelRead(widget->treeWidget()->topLevelItem(i))->epg(), _textFormat);
		cursor = channelsTable->cellAt(row, 6).firstCursorPosition();
		cursor.insertText(widget->channelRead(widget->treeWidget()->topLevelItem(i))->logo(), _textFormat);
	}

	cursor.setPosition(topFrame->lastPosition());
	cursor.insertBlock();
	cursor.insertBlock();

	cursor.insertText(tr("Exported with Tano on %1.").arg(QDate::currentDate().toString("d MMMM yyyy")), _textFormat);
	cursor.insertBlock();

	start();
}

void Print::start()
{
#ifndef QT_NO_PRINTER
	QPrinter printer(QPrinter::HighResolution);
	QPrintPreviewDialog preview(&printer, this);
	connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(preview(QPrinter*)));
	preview.exec();
#endif
}

void Print::preview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
	Q_UNUSED(printer);
#else
	print(printer);
#endif
}
