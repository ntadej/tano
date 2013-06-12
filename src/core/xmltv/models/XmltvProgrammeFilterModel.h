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

#ifndef TANO_XMLTVPROGRAMMEFILTERMODEL_H_
#define TANO_XMLTVPROGRAMMEFILTERMODEL_H_

#include <QtCore/QDate>

#if QT_VERSION >= 0x050000
    #include <QtCore/QSortFilterProxyModel>
#else
    #include <QtGui/QSortFilterProxyModel>
#endif

#include "CoreSharedExport.h"

class XmltvProgrammeModel;

/*!
    \class XmltvProgrammeFilterModel XmltvProgrammeFilterModel.h core/xmltv/models/XmltvProgrammeFilterModel.h
    \brief XMLTV Programme Filter model

    Reimplemented QSortFilterProxyModel to be able to filter XMLTV Programmes
*/
class TANO_CORE_EXPORT XmltvProgrammeFilterModel : public QSortFilterProxyModel
{
Q_OBJECT
public:
    XmltvProgrammeFilterModel(QObject *parent = 0);
    ~XmltvProgrammeFilterModel();

    /*!
        \brief Get current filtered date
        \return current date (QDate)
    */
    inline QDate date() const { return _date; }

    /*!
        \brief Set current filtered date
        \return date desired date (QDate)
    */
    void setDate(const QDate &date);

protected:
    /*!
        \brief Reimplemented filter function
        \param sourceRow row to process (int)
        \param sourceParent parent index (QModelIndex)
        \return true, if item to be displayed (bool)
    */
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &sourceParent) const;

public slots:
    /*!
        \brief Set Programme model to filter
        \param model Programme model (XmltvProgrammeModel)
    */
    void setProgrammeModel(XmltvProgrammeModel *model);

private:
    XmltvProgrammeModel *_model;

    QDate _date;
};

#endif // TANO_XMLTVPROGRAMMEFILTERMODEL_H_
