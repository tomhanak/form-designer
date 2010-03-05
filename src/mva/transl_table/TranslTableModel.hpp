/*
 * Copyright (C) 2010 Tieto Czech, s.r.o.
 * All rights reserved.
 * Contact: Tomáš Hanák <tomas.hanak@tieto.com>
 *          Radek Valášek <radek.valasek@tieto.com>
 *          Martin Kampas <martin.kampas@tieto.com>
 *          Jiří Litomyský <jiri.litomysky@tieto.com>
 *
 * This file is part of sfd [Simple Form Designer].
 *
 * SFD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <QtCore/QAbstractTableModel>

namespace sfd {

namespace core {
namespace transl_table {
    class UnifiedTranslTable;
}}

namespace mva {
namespace transl_table {

class TranslTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TranslTableModel(core::transl_table::UnifiedTranslTable *table, QObject 
            *parent = 0);
    ~TranslTableModel();

public:
    QString translId(const QModelIndex &index) const;
    QString context(const QModelIndex &index) const;

public:
    //! \reimpl QAbstractItemModel
    //@{
    // Read only model
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) 
        const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int 
            role = Qt::DisplayRole) const;
    // Editable model
    virtual bool setData(const QModelIndex &index, const QVariant &value, int 
            role = Qt::EditRole);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    //@}

private:
    class Private;
    p_ptr<Private> p;
};

}}} // namespace

#if CONFIG__PIMPL_RELEASE_MODE
# include "_lib/TranslTableModel_p.hpp"
#endif
