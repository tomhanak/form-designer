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

#include <QtGui/QAbstractItemDelegate>
#include <QtGui/QTableView>

class QContextMenuEvent;

namespace sfd {

namespace core {
namespace transl_table {
    class UnifiedTranslTable;
}}

namespace gui {
namespace transl_table {

class TranslTableView : public QTableView
{
    Q_OBJECT

public:
    TranslTableView(core::transl_table::UnifiedTranslTable *table, QWidget 
            *parent = 0);
    ~TranslTableView();

protected Q_SLOTS:
    //! \reimpl QAbstractItemView
    //@{
    void commitData(QWidget *editor);
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
    //@}

private:
    class Private;
    p_ptr<Private> p;
};

}}} // namespace

#if CONFIG__PIMPL_RELEASE_MODE
# include "_lib/TranslTableView_p.hpp"
#endif
