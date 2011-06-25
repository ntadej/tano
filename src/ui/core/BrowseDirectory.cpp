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

#include <QtCore/QDir>
#include <QtGui/QFileDialog>
#include <QtGui/QHBoxLayout>

#include "BrowseDirectory.h"

BrowseDirectory::BrowseDirectory(QWidget *parent)
    : QWidget(parent),
      _resetValue("")
{
    _edit = new QLineEdit(this);
    _browse = new QToolButton(this);
    _browse->setText("...");
    _reset = new QPushButton(this);
    _reset->setText(tr("Reset"));
    _reset->setIconSize(QSize(24, 24));
    _reset->setIcon(QIcon(":/icons/24x24/refresh.png"));
    _reset->setMaximumHeight(24);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(_edit);
    layout->addWidget(_browse);
    layout->addWidget(_reset);
    setLayout(layout);

    connect(_browse, SIGNAL(clicked()), this, SLOT(browse()));
    connect(_reset, SIGNAL(clicked()), this, SLOT(reset()));
}

BrowseDirectory::~BrowseDirectory()
{
    delete _edit;
    delete _browse;
    delete _reset;
}

void BrowseDirectory::browse()
{
    QString dir;
    if(_edit->text().isEmpty())
        dir = QDir::homePath();
    else
        dir = _edit->text();
    QString file = QFileDialog::getExistingDirectory(this, tr("Open directory"),
                                                    dir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(file.isEmpty())
        return;

    _edit->setText(file);
}

void BrowseDirectory::reset()
{
    _edit->setText(_resetValue);
}

void BrowseDirectory::setValue(const QString &value)
{
    _edit->setText(value);
}

QString BrowseDirectory::value() const
{
    return _edit->text();
}
