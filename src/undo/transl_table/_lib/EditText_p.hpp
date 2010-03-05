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

#include <QtCore/QPointer>

#include "../EditText.hpp"

namespace sfd {

namespace core {
namespace transl_table {
    class UnifiedTranslTable;
}}

namespace undo {
namespace transl_table {

class EditText::Private
{
    friend class EditText;
    p_ptr<EditText> p;

private:
    Private(EditText *publ, core::transl_table::UnifiedTranslTable *table, 
            const QString &text, const QString &language, const QString &id, 
            const QString &context);

private:
    const QPointer<core::transl_table::UnifiedTranslTable> table;
    const QString language;
    const QString id;
    const QString context;
    const QString newText;
    QString oldText;
};

PIMPL_DEFINE_DESTRUCTOR(EditText)

}}} // namespace
