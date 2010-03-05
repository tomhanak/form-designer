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

#include "../Editor.hpp"

#include <QtCore/QPointer>

namespace sfd {
namespace gui {
namespace main_window {

class Editor::Private
{
    friend class Editor;
    p_ptr<Editor> p;

private:
    Private(Editor *publ);

private:
};

class Editor::Magnifier::Private
{
    friend class Magnifier;
    p_ptr<Magnifier> p;

private:
    Private(Magnifier *publ);

private:
    int scale;
    const QPointer<QAction> scaleInAction;
    const QPointer<QAction> scaleOutAction;
};

PIMPL_DEFINE_DESTRUCTOR(Editor)

PIMPL_DEFINE_DESTRUCTOR2(Editor, Magnifier)
PIMPL_DEFINE_GETTER(int, Editor::Magnifier, scale)
PIMPL_DEFINE_GETTER(QAction*, Editor::Magnifier, scaleInAction)
PIMPL_DEFINE_GETTER(QAction*, Editor::Magnifier, scaleOutAction)

}}} // namespace
