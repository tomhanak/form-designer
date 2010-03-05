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

#include "../FormEditor.hpp"

#include <QtCore/QPointer>
#include <QtCore/QSet>
#include <QtGui/QAction>

namespace sfd {
namespace gui {
namespace form_editor {

class GeometryEditor;

class FormEditor::Private
{
    friend class FormEditor;
    p_ptr<FormEditor> p;

private:
    Private(FormEditor *publ, core::form::Form *form, Grid *grid, 
            core::control_loader::ControlPluginLoader *controlPluginLoader);

private:
    const QPointer<core::form::Form> form;
    const QPointer<Grid> grid;
    const QPointer<core::control_loader::ControlPluginLoader> 
        controlPluginLoader;

    QPointer<GeometryEditor> geometryEditor;
};

class FormEditor::Grid::Private
{
    friend class Grid;
    p_ptr<Grid> p;

private:
    Private(Grid *publ);

private:
    static int align(int value, int step);

private:
    int xStep;
    int yStep;
    const QPointer<QAction> visibleAction;
    const QPointer<QAction> xEnabledAction;
    const QPointer<QAction> yEnabledAction;
};

PIMPL_DEFINE_DESTRUCTOR(FormEditor)
PIMPL_DEFINE_GETTER(core::form::Form*, FormEditor, form)
PIMPL_DEFINE_GETTER(FormEditor::Grid*, FormEditor, grid)

PIMPL_INLINE core::form::Form *FormEditor::document() const
{
    return p->form;
}

/*!
 * \class FormEditor::Grid
 */

PIMPL_DEFINE_DESTRUCTOR2(FormEditor, Grid)
PIMPL_DEFINE_GETTER(int, FormEditor::Grid, xStep)
PIMPL_DEFINE_GETTER(int, FormEditor::Grid, yStep)
PIMPL_DEFINE_GETTER(QAction*, FormEditor::Grid, visibleAction)
PIMPL_DEFINE_GETTER(QAction*, FormEditor::Grid, xEnabledAction)
PIMPL_DEFINE_GETTER(QAction*, FormEditor::Grid, yEnabledAction)

PIMPL_INLINE bool FormEditor::Grid::isVisible() const
{
    return p->visibleAction->isChecked();
}

PIMPL_INLINE bool FormEditor::Grid::isXEnabled() const
{
    return p->xEnabledAction->isChecked();
}

PIMPL_INLINE bool FormEditor::Grid::isYEnabled() const
{
    return p->yEnabledAction->isChecked();
}

PIMPL_INLINE QPoint FormEditor::Grid::align(const QPoint &point) const
{
    return QPoint(
            isXEnabled() ? Private::align(point.x(), p->xStep) : point.x(),
            isYEnabled() ? Private::align(point.y(), p->yStep) : point.y());
}

PIMPL_INLINE bool FormEditor::Grid::diff(const QPoint &point1, const QPoint 
        &point2) const
{
    return qAbs(point1.x() - point2.x()) > p->xStep/2 ||
        qAbs(point1.y() - point2.y()) > p->yStep/2;
}

inline int FormEditor::Grid::Private::align(int value, int step)
{
    return (value - value%step) + (value%step < step/2 ? 0 : step);
}

}}} // namespace
