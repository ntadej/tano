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

#include "ChannelsDisplayWidget.h"

#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>

#include "channels/models/ChannelsFilterModel.h"
#include "channels/models/ChannelsModel.h"

ChannelsDisplayWidget::ChannelsDisplayWidget(QWidget *parent)
    : QListView(parent),
      _current(0)
{
    setAlternatingRowColors(true);
    setEditTriggers(NoEditTriggers);

    _filterModel = new ChannelsFilterModel(this);
    _filterModel->setDynamicSortFilter(true);

    setModel(_filterModel);
    setContextMenuPolicy(Qt::CustomContextMenu);

    _rightMenu = new QMenu(this);
    _play = new QAction(QIcon(":/icons/actions/16/media-playback-start.png"), tr("Play"), this);
    _schedule = new QAction(QIcon(":/icons/apps/16/office-calendar.png"), tr("Schedule"), this);
    _rightMenu->addAction(_play);
    _rightMenu->addAction(_schedule);

    connect(this, SIGNAL(activated(QModelIndex)), this, SLOT(channelSelected(QModelIndex)));
    connect(_play, SIGNAL(triggered()), this, SLOT(play()));
    connect(_schedule, SIGNAL(triggered()), this, SLOT(schedule()));
}

ChannelsDisplayWidget::~ChannelsDisplayWidget()
{
    delete _filterModel;
}

void ChannelsDisplayWidget::channelSelected(Channel *channel)
{
    _current = channel;
    emit itemSelected(_current);
    updateSelection(_current);
}

void ChannelsDisplayWidget::channelSelected(const QModelIndex &index)
{
    _current = _model->row(_filterModel->mapToSource(index).row());
    emit itemSelected(_current);
    updateSelection(_current);
}

void ChannelsDisplayWidget::channelSelected(int channel)
{
    if (!channel) {
        _current = 0;
        emit itemSelected(_current);
        return;
    }

    _current = _model->number(channel);
    emit itemSelected(_current);
    updateSelection(_current);
}

void ChannelsDisplayWidget::channelSelected(const QString &xmltvId)
{
    _current = _model->xmltvId(xmltvId);
    emit itemSelected(_current);
    updateSelection(_current);
}

void ChannelsDisplayWidget::play()
{
    QModelIndex channel = indexAt(_currentPos);

    if (channel != QModelIndex())
        channelSelected(channel);
}

void ChannelsDisplayWidget::playMode()
{
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showMenu(QPoint)));
}

void ChannelsDisplayWidget::setFilters(const QString &search)
{
    QRegExp regExp(search, Qt::CaseInsensitive);
    _filterModel->setFilterRegExp(regExp);
}

void ChannelsDisplayWidget::setFilters(const QString &search,
                                       const QString &category,
                                       const QString &language,
                                       const QList<Channel::Type> &types)
{
    QRegExp regExp(search, Qt::CaseInsensitive);
    _filterModel->setFilterRegExp(regExp);
    _filterModel->setCategory(category);
    _filterModel->setLanguage(language);
    _filterModel->setTypes(types);
}

void ChannelsDisplayWidget::schedule()
{
    emit scheduleRequested(_model->row(_filterModel->mapToSource(indexAt(_currentPos)).row()));
}

void ChannelsDisplayWidget::setChannelsModel(ChannelsModel *model)
{
    _model = model;
    _filterModel->setSourceModel(model);
}

void ChannelsDisplayWidget::showMenu(const QPoint &pos)
{
    _currentPos = pos;

    if (indexAt(_currentPos) != QModelIndex())
        _rightMenu->exec(QCursor::pos());
}

void ChannelsDisplayWidget::updateSelection(Channel *channel)
{
    selectionModel()->select(_filterModel->mapFromSource(_model->indexFromItem(channel)), QItemSelectionModel::SelectCurrent);

    if (!visibleChannels().contains(channel))
        scrollTo(_filterModel->mapFromSource(_model->indexFromItem(channel)), QAbstractItemView::PositionAtTop);
}

QList<Channel *> ChannelsDisplayWidget::visibleChannels()
{
    QList<Channel *> list;

    QModelIndex index = indexAt(QPoint(0, 0));
    if (index.isValid()) {
        list << _model->row(_filterModel->mapToSource(index).row());
    }

    while (viewport()->rect().contains(QPoint(0, visualRect(index).y() + visualRect(index).height() + 1))) {
        index = indexAt(QPoint(0, visualRect(index).y() + visualRect(index).height() + 1));
        if (!index.isValid())
            break;
        list << _model->row(_filterModel->mapToSource(index).row());
    }

    return list;
}
