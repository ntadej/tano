/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** Copyright (C) 2009-2010 Tadej Novak
**
** This file is part of the tools applications of the Qt Toolkit.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file.  Please review the following information
** to ensure GNU General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.  In addition, as a special
** exception, Nokia gives you certain additional rights. These rights
** are described in the Nokia Qt GPL Exception version 1.3, included in
** the file GPL_EXCEPTION.txt in this package.
**
****************************************************************************/

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
