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

#include <QtCore/QMap>
#include <QtCore/QPair>

#include "../UnifiedTranslTable.hpp"

namespace sfd {
namespace core {
namespace transl_table {

class UnifiedTranslTable::Private
{
    friend class UnifiedTranslTable;
    p_ptr<UnifiedTranslTable> p;

private:
    Private(UnifiedTranslTable *publ);

private:
    static int newUid() {
        static int counter = 0;
        return counter++;
    }

private:
    //   upid       translID context
    QMap<int, QPair<QString, QString> > rowIds;
    //   ulid language
    QMap<int, QString> languages;
    //   upid      ulid text
    QMap<int, QMap<int, QString> > texts;
};

PIMPL_DEFINE_DESTRUCTOR(UnifiedTranslTable)

}}} // namespace
