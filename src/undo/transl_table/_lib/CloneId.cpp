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

#include "CloneId_p.hpp"

#include "core/transl_table/UnifiedTranslTable.hpp"

using namespace sfd::undo::transl_table;

CloneId::CloneId(core::transl_table::UnifiedTranslTable *table, const QString 
        &originId, const QString &newId, const QString &originContext, const 
        QString &newContext, QUndoCommand *parent)
    : QUndoCommand(parent)
    , p(new Private(this, table, originId, newId, originContext, newContext))
{
}

void CloneId::undo()
{
    const int upid = p->table->uniqueRowId(p->newId, p->newContext);
    p->table->removeId(upid);
}

void CloneId::redo()
{
    p->table->addId(p->newId, p->newContext);
    const int upid = p->table->uniqueRowId(p->newId, p->newContext);
    Q_FOREACH(int ulid, p->table->uniqueLanguageIds()) {
        p->table->setText(p->texts[p->table->language(ulid)], upid, ulid);
    }
}


CloneId::Private::Private(CloneId *publ, core::transl_table::UnifiedTranslTable 
        *table, const QString &originId, const QString &newId, const QString 
        &originContext, const QString &newContext)
    : p(publ)
    , table(table)
    , originId(originId)
    , newId(newId)
    , originContext(originContext)
    , newContext(newContext)
{
    Q_ASSERT(table != 0);
    const int upid =  table->uniqueRowId(originId, originContext);
    Q_FOREACH(int ulid, table->uniqueLanguageIds()) {
        texts[table->language(ulid)] = table->text(upid, ulid);
    }
}
