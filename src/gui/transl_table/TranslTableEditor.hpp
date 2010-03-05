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

#include <QtCore/QObject>

#include "core/transl_table/UnifiedTranslTable.hpp"
#include "gui/main_window/Editor.hpp"

namespace sfd {

namespace core {

namespace transl_table {
    class UnifiedTranslTable;
}

namespace project {
    class Document;
}}

namespace gui {
namespace transl_table {

class TranslTableEditor : public main_window::Editor
{
    Q_OBJECT

public:
    class Creator;

public:
    TranslTableEditor(core::transl_table::UnifiedTranslTable *table, QWidget 
            *parent = 0);
    ~TranslTableEditor();

public:
    //! \reimpl Editor
    //@{
    virtual core::transl_table::UnifiedTranslTable *document() const;
    //@}

private:
    class Private;
    p_ptr<Private> p;
};

class TranslTableEditor::Creator : public main_window::EditorFromDocumentCreator
{
public:
    Creator();

protected:
    //! \reimpl AFromBCreator
    //@{
    virtual main_window::Editor *tryToCreate(core::project::Document *document) 
        const;
    //@}
};

}}} // namespace

#if CONFIG__PIMPL_RELEASE_MODE
# include "_lib/TranslTableEditor_p.hpp"
#endif
