/****************************************************************************
* QtKeySequenceEdit.h: A class for Keyboard input sequence editing
* This file is also part of the tools applications of the Qt Toolkit.
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
* Contact: Qt Software Information (qt-info@nokia.com)
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

#include <QtGui/QAction>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QKeyEvent>
#include <QtGui/QMenu>
#include <QtGui/QWidget>

class QtKeySequenceEdit : public QWidget
{
Q_OBJECT
public:
	QtKeySequenceEdit(QWidget *parent = 0);

	QKeySequence keySequence() const;
	bool eventFilter(QObject *o, QEvent *e);
public Q_SLOTS:
	void setKeySequence(const QKeySequence &sequence);
Q_SIGNALS:
	void keySequenceChanged(const QKeySequence &sequence);
protected:
	void focusInEvent(QFocusEvent *e);
	void focusOutEvent(QFocusEvent *e);
	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);
	bool event(QEvent *e);
private slots:
	void slotClearShortcut();
private:
	void handleKeyEvent(QKeyEvent *e);
	int translateModifiers(Qt::KeyboardModifiers state, const QString &text) const;

	int m_num;
	QKeySequence m_keySequence;
	QLineEdit *m_lineEdit;
};
