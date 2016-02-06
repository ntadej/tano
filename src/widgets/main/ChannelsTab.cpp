/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#include <QtCore/QEvent>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolBar>

#include "channels/widgets/ChannelsDisplayWidget.h"

#include "style/FancyLineEdit.h"
#include "style/StyledBar.h"

#include "ChannelsTab.h"

ChannelsTab::ChannelsTab(QWidget *parent)
    : QMainWindow(parent)
{
    _channelsWidget = new ChannelsDisplayWidget(this);
    _channelsWidget->playMode();
    setCentralWidget(_channelsWidget);

    _toolbarTop = new QToolBar(this);
    _toolbarTop->setMovable(false);
    _toolbarTop->setFloatable(false);
    _toolbarTop->setIconSize(QSize(16, 16));
    _toolbarTop->setMinimumHeight(24);
    _toolbarTop->setMaximumHeight(24);
    addToolBar(Qt::TopToolBarArea, _toolbarTop);

    _toolbarBottomSearch = new QToolBar(this);
    _toolbarBottomSearch->setMovable(false);
    _toolbarBottomSearch->setFloatable(false);
    _toolbarBottomSearch->setProperty("topBorder", true);
    _toolbarBottomSearch->setMinimumHeight(24);
    _toolbarBottomSearch->setMaximumHeight(24);
    addToolBar(Qt::BottomToolBarArea, _toolbarBottomSearch);
    insertToolBarBreak(_toolbarBottomSearch);

    _toolbarBottomType = new QToolBar(this);
    _toolbarBottomType->setMovable(false);
    _toolbarBottomType->setFloatable(false);
    _toolbarBottomType->setProperty("topBorder", true);
    _toolbarBottomType->setMinimumHeight(24);
    _toolbarBottomType->setMaximumHeight(24);
    addToolBar(Qt::BottomToolBarArea, _toolbarBottomType);
    insertToolBarBreak(_toolbarBottomType);

    _labelChannels = new QLabel(this);
    _labelChannels->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _labelChannels->setAlignment(Qt::AlignCenter);
    _toolbarTop->addWidget(_labelChannels);

    _selectType = new QComboBox(this);
    _selectType->addItem(tr("All"));
    _selectType->addItem(tr("TV"));
    _selectType->addItems(Channel::types());
    _selectType->setProperty("hideborder", true);
    _selectType->setProperty("drawleftborder", true);
    _toolbarTop->addWidget(_selectType);

    _selectCategory = new QComboBox(this);
    _selectCategory->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _selectLanguage = new QComboBox(this);
    _selectLanguage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _selectLanguage->setProperty("hideborder", true);
    _toolbarBottomType->addWidget(_selectCategory);
    _toolbarBottomType->addWidget(_selectLanguage);

    _search = new FancyLineEdit(this);
    _search->setButtonPixmap(FancyLineEdit::Left, QIcon(":/icons/actions/16/edit-find.png").pixmap(16));
    _search->setButtonPixmap(FancyLineEdit::Right, QIcon(":/icons/actions/16/edit-clear-all.png").pixmap(16));
    _search->setButtonVisible(FancyLineEdit::Left, true);
    _search->setButtonVisible(FancyLineEdit::Right, true);
    _search->setAutoHideButton(FancyLineEdit::Right, true);
    _toolbarBottomSearch->addWidget(_search);

    connect(_selectType, SIGNAL(currentIndexChanged(QString)), this, SLOT(processFilters()));
    connect(_selectCategory, SIGNAL(currentIndexChanged(QString)), this, SLOT(processFilters()));
    connect(_selectLanguage, SIGNAL(currentIndexChanged(QString)), this, SLOT(processFilters()));
    connect(_search, SIGNAL(textChanged(QString)), this, SLOT(processFilters()));
    connect(_search, SIGNAL(rightButtonClicked()), _search, SLOT(clear()));
}

ChannelsTab::~ChannelsTab() { }

void ChannelsTab::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        _selectType->setItemText(0, tr("All"));
        _selectType->setItemText(1, tr("TV"));
        for (int i = 2; i < _selectType->count(); i++)
            _selectType->setItemText(i, Channel::types()[i-2]);
        _selectCategory->setItemText(0, tr("All categories"));
        _selectLanguage->setItemText(0, tr("All languages"));
        processFilters();
        break;
    default:
        break;
    }
}

void ChannelsTab::processFilters()
{
    QList<Channel::Type> list;
    switch (_selectType->currentIndex())
    {
    case 1:
        list << Channel::SD
             << Channel::HD;
        break;
    case 2:
        list << Channel::SD;
        break;
    case 3:
        list << Channel::HD;
        break;
    case 4:
        list << Channel::Radio;
        break;
    case 0:
    default:
        list << Channel::SD
             << Channel::HD
             << Channel::Radio;
        break;
    }

    _channelsWidget->setFilters(_search->text(),
                                _selectCategory->currentIndex() == 0 ? "" : _selectCategory->currentText(),
                                _selectLanguage->currentIndex() == 0 ? "" : _selectLanguage->currentText(),
                                list);
}

void ChannelsTab::setFilters(const QStringList &categories,
                             const QStringList &languages)
{
    _selectCategory->clear();
    _selectCategory->insertItem(0, tr("All categories"));
    _selectCategory->insertItems(1, categories);

    _selectLanguage->clear();
    _selectLanguage->insertItem(0, tr("All languages"));
    _selectLanguage->insertItems(1, languages);
}

void ChannelsTab::setChannelsName(const QString &name)
{
    _labelChannels->setText(name);
}
