/****************************************************************************
* Print.h: Main print class
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

#ifndef TANO_PRINT_H_
#define TANO_PRINT_H_

#include <QtGui/QTextEdit>

#include "ui/PlaylistWidget.h"

class Print : public QTextEdit
{
Q_OBJECT
public:
    Print();
	~Print();

	void channelList(const QString &name, PlaylistWidget *widget);

private slots:
	void preview(QPrinter *printer);

private:
	void start();

	QTextCharFormat _textFormat;
	QTextCharFormat _boldFormat;
	QTextCharFormat _titleFormat;
	QTextTableFormat _tableFormat;
};

#endif // TANO_PRINT_H_
