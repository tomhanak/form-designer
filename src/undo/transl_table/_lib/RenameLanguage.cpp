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

#include "RenameLanguage_p.hpp"

#include "core/transl_table/UnifiedTranslTable.hpp"

using namespace sfd::undo::transl_table;

RenameLanguage::RenameLanguage(core::transl_table::UnifiedTranslTable *table, 
        const QString &oldLanguage, const QString &newLanguage, QUndoCommand 
        *parent)
    : QUndoCommand(parent)
    , p(new Private(this, table, oldLanguage, newLanguage))
{
}

void RenameLanguage::undo()
{
    const int ulid = p->table->uniqueLanguageId(p->newLanguage);
    p->table->renameLanguage(ulid, p->oldLanguage);
}

void RenameLanguage::redo()
{
    const int ulid = p->table->uniqueLanguageId(p->oldLanguage);
    p->table->renameLanguage(ulid, p->newLanguage);
}


RenameLanguage::Private::Private(RenameLanguage *publ, 
        core::transl_table::UnifiedTranslTable *table, const QString 
        &oldLanguage, const QString &newLanguage)
    : p(publ)
    , table(table)
    , oldLanguage(oldLanguage)
    , newLanguage(newLanguage)
{
    Q_ASSERT(table != 0);
}
