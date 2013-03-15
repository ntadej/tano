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

#ifndef TANO_XMLTVCREWFILTERMODEL_H_
#define TANO_XMLTVCREWFILTERMODEL_H_

#include "CoreSharedExport.h"

#if QT_VERSION >= 0x050000
    #include <QtCore/QSortFilterProxyModel>
#else
    #include <QtGui/QSortFilterProxyModel>
#endif

#include "xmltv/containers/XmltvCrewMember.h"

class XmltvCrewModel;

/*!
    \class XmltvCrewFilterModel XmltvCrewFilterModel.h core/xmltv/models/XmltvCrewFilterModel.h
    \brief XMLTV Crew Filter model

    Reimplemented QSortFilterProxyModel to be able to filter XMLTV Crew Members
*/
class TANO_CORE_EXPORT XmltvCrewFilterModel : public QSortFilterProxyModel
{
Q_OBJECT
public:
    XmltvCrewFilterModel(QObject *parent = 0);
    ~XmltvCrewFilterModel();

    /*!
        \brief Get current crew member type
        \return current crew member type (XmltvCrewMember::Type)
    */
    inline XmltvCrewMember::Type type() const { return _type; }

    /*!
        \brief Set crew member type
        \param type crew member type (XmltvCrewMember::Type)
    */
    void setType(const XmltvCrewMember::Type &type = XmltvCrewMember::All);

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
        \brief Set Crew model to filter
        \param model Crew model (XmltvCrewModel)
    */
    void setCrewModel(XmltvCrewModel *model);

private:
    XmltvCrewModel *_model;

    XmltvCrewMember::Type _type;
};

#endif // TANO_XMLTVCREWFILTERMODEL_H_
