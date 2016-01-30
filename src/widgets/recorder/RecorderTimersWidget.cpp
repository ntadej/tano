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

#include <QtCore/QEvent>
#include <QtWidgets/QAction>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolBar>

#include "timers/containers/Timer.h"
#include "timers/models/TimersFilterModel.h"

#include "style/FancyLineEdit.h"

#include "RecorderTimersWidget.h"

RecorderTimersWidget::RecorderTimersWidget(QWidget *parent)
    : QMainWindow(parent),
      _model(0)
{
    _filterModel = new TimersFilterModel(this);
    _filterModel->setDynamicSortFilter(true);

    _list = new QListView(this);
    _list->setModel(_filterModel);
    _list->setAlternatingRowColors(true);
    _list->setEditTriggers(QListView::NoEditTriggers);
    _list->setFrameStyle(QFrame::NoFrame);
    _list->setAttribute(Qt::WA_MacShowFocusRect, false);
    setCentralWidget(_list);

    _toolbarTop = new QToolBar(this);
    _toolbarTop->setMovable(false);
    _toolbarTop->setFloatable(false);
    _toolbarTop->setIconSize(QSize(16, 16));
    _toolbarTop->setMinimumHeight(24);
    _toolbarTop->setMaximumHeight(24);
    _toolbarTop->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    addToolBar(Qt::TopToolBarArea, _toolbarTop);

    _toolbarBottomSearch = new QToolBar(this);
    _toolbarBottomSearch->setMovable(false);
    _toolbarBottomSearch->setFloatable(false);
    _toolbarBottomSearch->setProperty("topBorder", true);
    _toolbarBottomSearch->setMinimumHeight(24);
    _toolbarBottomSearch->setMaximumHeight(24);
    addToolBar(Qt::BottomToolBarArea, _toolbarBottomSearch);

    _labelRecorder = new QLabel(this);
    _labelRecorder->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _labelRecorder->setAlignment(Qt::AlignCenter);
    _toolbarTop->addWidget(_labelRecorder);

    _actionQuick = new QAction(QIcon(":/icons/actions/16/media-record.png"), tr("Quick"), this);
    _toolbarTop->addAction(_actionQuick);

    _actionNew = new QAction(QIcon(":/icons/actions/16/list-add.png"), tr("New"), this);
    _toolbarTop->addAction(_actionNew);

    _search = new FancyLineEdit(this);
    _search->setButtonPixmap(FancyLineEdit::Left, QIcon(":/icons/actions/16/edit-find.png").pixmap(16));
    _search->setButtonPixmap(FancyLineEdit::Right, QIcon(":/icons/actions/16/edit-clear-all.png").pixmap(16));
    _search->setButtonVisible(FancyLineEdit::Left, true);
    _search->setButtonVisible(FancyLineEdit::Right, true);
    _search->setAutoHideButton(FancyLineEdit::Right, true);
    _toolbarBottomSearch->addWidget(_search);

    connect(_list, SIGNAL(activated(QModelIndex)), this, SLOT(timerClicked(QModelIndex)));
    connect(_search, SIGNAL(textChanged(QString)), this, SLOT(processFilters()));
    connect(_search, SIGNAL(rightButtonClicked()), _search, SLOT(clear()));
    connect(_actionNew, SIGNAL(triggered()), this, SIGNAL(requestNew()));
    connect(_actionQuick, SIGNAL(triggered()), this, SIGNAL(requestQuick()));
}

RecorderTimersWidget::~RecorderTimersWidget() { }

void RecorderTimersWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        _labelRecorder->setText(tr("Recorder"));
        _actionQuick->setText(tr("Quick"));
        _actionNew->setText(tr("New"));
        break;
    default:
        break;
    }
}

void RecorderTimersWidget::processFilters()
{
    QRegExp regExp(_search->text(), Qt::CaseInsensitive);
    _filterModel->setFilterRegExp(regExp);
}

void RecorderTimersWidget::setCurrentTimer(Timer *timer)
{
    _current = timer;
    _list->setCurrentIndex(_model->indexFromItem(timer));
}

void RecorderTimersWidget::setModel(TimersModel *model)
{
    _model = model;
    _filterModel->setSourceModel(model);
}

void RecorderTimersWidget::timerClicked(const QModelIndex &index)
{
    _current = _model->row(_filterModel->mapToSource(index).row());

    if (_current->state() == Timer::Finished)
        emit recordingSelected(_current);
    else if (_current->state() != Timer::Recording)
        emit timerSelected(_current);
}
