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

#include "EditText_p.hpp"

#include "core/transl_table/UnifiedTranslTable.hpp"

using namespace sfd::undo::transl_table;

EditText::EditText(core::transl_table::UnifiedTranslTable *table, const QString 
        &text, const QString &language, const QString &id, const QString 
        &context, QUndoCommand *parent)
    : QUndoCommand(parent)
    , p(new Private(this, table, text, language, id, context))
{
}

void EditText::undo()
{
    const int upid = p->table->uniqueRowId(p->id, p->context);
    const int ulid = p->table->uniqueLanguageId(p->language);
    p->table->setText(p->oldText, upid, ulid);
}

void EditText::redo()
{
    const int upid = p->table->uniqueRowId(p->id, p->context);
    const int ulid = p->table->uniqueLanguageId(p->language);
    p->table->setText(p->newText, upid, ulid);
}


EditText::Private::Private(EditText *publ, 
        core::transl_table::UnifiedTranslTable *table, const QString &text, 
        const QString &language, const QString &id, const QString &context)
    : p(publ)
    , table(table)
    , language(language)
    , id(id)
    , context(context)
    , newText(text)
{
    Q_ASSERT(table != 0);
    const int upid = table->uniqueRowId(id, context);
    const int ulid = table->uniqueLanguageId(language);
    oldText = table->text(upid, ulid);
}
