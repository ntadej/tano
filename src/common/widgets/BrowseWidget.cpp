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

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include "common/widgets/FileDialogs.h"

#include "BrowseWidget.h"

BrowseWidget::BrowseWidget(QWidget *parent)
    : QWidget(parent)
{
    _edit = new QLineEdit(this);
    _edit->setClearButtonEnabled(true);
    _edit->setMinimumWidth(225);
    _browse = new QPushButton(this);
    _browse->setText(tr("Browse…"));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(1);
    layout->setMargin(0);
    layout->addWidget(_edit);
    layout->addWidget(_browse);
    setLayout(layout);

    connect(_browse, &QPushButton::clicked, this, &BrowseWidget::browse);
    connect(_edit, &QLineEdit::textChanged, this, &BrowseWidget::valueChanged);
}

BrowseWidget::~BrowseWidget()
{
    delete _edit;
    delete _browse;
}

void BrowseWidget::browse()
{
    QString b = _edit->text().isEmpty() ? QDir::homePath() : _edit->text();
    QString file = FileDialogs::openByType(_type, b);

    if(file.isEmpty())
        return;

    _edit->setText(file);
}

void BrowseWidget::setValue(const QString &value)
{
    _edit->setText(value);
}

QString BrowseWidget::value() const
{
    return _edit->text();
}
