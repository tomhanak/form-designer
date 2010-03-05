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

#include "Editor.hpp"

namespace sfd {
namespace gui {
namespace main_window {

//! Encapsulating a \e plain Editor, extends it with zooming feature
class GraphicsViewMagnifier : public Editor
{
    Q_OBJECT

public:
    class Creator;

public:
    GraphicsViewMagnifier(Editor *plainEditor, QWidget *parent = 0);
    ~GraphicsViewMagnifier();

public:
    Editor *plainEditor() const;

public:
    //! \reimpl Editor
    //@{
    virtual core::project::Document *document() const;
    virtual Magnifier *magnifier() const;
    //@}

private:
    class Private;
    p_ptr<Private> p;
};

}}} // namespace

#if CONFIG__PIMPL_RELEASE_MODE
# include "_lib/GraphicsViewMagnifier_p.hpp"
#endif
