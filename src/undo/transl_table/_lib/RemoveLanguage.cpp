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

#include "RemoveLanguage_p.hpp"

#include "core/transl_table/UnifiedTranslTable.hpp"

using namespace sfd::undo::transl_table;

RemoveLanguage::RemoveLanguage(core::transl_table::UnifiedTranslTable *table, 
        const QString &language, QUndoCommand *parent)
    : QUndoCommand(parent)
    , p(new Private(this, table, language))
{
}

void RemoveLanguage::undo()
{
    p->table->addLanguage(p->language);
    const int ulid = p->table->uniqueLanguageId(p->language);
    Q_FOREACH(int upid, p->table->uniqueRowIds()) {
        const Private::Row row(p->table->translId(upid), 
                p->table->context(upid));
        p->table->setText(p->texts[row], upid, ulid);
    }
}

void RemoveLanguage::redo()
{
    const int ulid = p->table->uniqueLanguageId(p->language);
    p->table->removeLanguage(ulid);
}


RemoveLanguage::Private::Private(RemoveLanguage *publ, 
        core::transl_table::UnifiedTranslTable *table, const QString &language)
    : p(publ)
    , table(table)
    , language(language)
{
    Q_ASSERT(table != 0);
    const int ulid =  table->uniqueLanguageId(language);
    Q_FOREACH(int upid, table->uniqueRowIds()) {
        const Row row(table->translId(upid), table->context(upid));
        texts[row] = table->text(upid, ulid);
    }
}
