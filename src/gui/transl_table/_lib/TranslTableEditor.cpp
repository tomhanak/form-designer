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

#include "TranslTableEditor_p.hpp"

#include <QtGui/QGridLayout>

#include "../TranslTableView.hpp"

using namespace sfd::gui::transl_table;
using namespace sfd::gui::main_window;

TranslTableEditor::TranslTableEditor(core::transl_table::UnifiedTranslTable 
        *table, QWidget *parent)
    : Editor(parent)
    , p(new Private(this, table))
{
}


TranslTableEditor::Private::Private(TranslTableEditor *publ, 
        core::transl_table::UnifiedTranslTable *table)
    : p(publ)
    , table(table)
{
    Q_ASSERT(table != 0);

    p->setLayout(new QGridLayout(p));
    p->layout()->setContentsMargins(0, 0, 0, 0);
    p->layout()->addWidget(new TranslTableView(table, p));
}


/*!
 * \class sfd::gui::transl_table::TranslTableEditor::Creator
 */

TranslTableEditor::Creator::Creator()
{
}

Editor *TranslTableEditor::Creator::tryToCreate(core::project::Document 
        *document) const
{
    using core::transl_table::UnifiedTranslTable;

    UnifiedTranslTable *const translTableDocument = 
        qobject_cast<UnifiedTranslTable*>(document);
    if (translTableDocument == 0) {
        return 0;
    }

    Editor *const editor = new TranslTableEditor(translTableDocument);
    return editor;
}
