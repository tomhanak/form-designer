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

#include "../GraphicsViewMagnifier.hpp"

#include <QtCore/QPointer>

class QGraphicsScene;
class QGraphicsView;
class QGraphicsProxyWidget;

namespace sfd {
namespace gui {
namespace main_window {

class GraphicsViewMagnifier::Private : public QObject
{
    Q_OBJECT

    friend class GraphicsViewMagnifier;
    p_ptr<GraphicsViewMagnifier> p;

private:
    Private(GraphicsViewMagnifier *publ, Editor *plainEditor);

private Q_SLOTS:
    void scaleTo(int scale);

private:
    const QPointer<Editor> plainEditor;
    const QPointer<Magnifier> magnifier;
    const QPointer<QGraphicsScene> scene;
    const QPointer<QGraphicsView> view;
    const QPointer<QGraphicsProxyWidget> proxyWidget;
};

PIMPL_DEFINE_DESTRUCTOR(GraphicsViewMagnifier)
PIMPL_DEFINE_GETTER(Editor*, GraphicsViewMagnifier, plainEditor)
PIMPL_DEFINE_GETTER(Editor::Magnifier*, GraphicsViewMagnifier, magnifier)

}}} // namespace
