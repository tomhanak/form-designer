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

#include "GraphicsViewMagnifier_p.hpp"

#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QHBoxLayout>

using namespace sfd::gui::main_window;
using namespace sfd;

GraphicsViewMagnifier::GraphicsViewMagnifier(Editor *plainEditor, QWidget 
        *parent)
    : Editor(parent)
    , p(new Private(this, plainEditor))
{
}

core::project::Document *GraphicsViewMagnifier::document() const
{
    return p->plainEditor->document();
}


GraphicsViewMagnifier::Private::Private(GraphicsViewMagnifier *publ, Editor 
        *plainEditor)
    : p(publ)
    , plainEditor(plainEditor)
    , magnifier(new Magnifier)
    , scene(new QGraphicsScene)
    , view(new QGraphicsView(scene, p))
    , proxyWidget(scene->addWidget(plainEditor))
{
    Q_ASSERT(plainEditor != 0);

    p->setLayout(new QHBoxLayout);
    p->layout()->setContentsMargins(0, 0, 0, 0);

    p->layout()->addWidget(view);

    view->setFrameShape(QFrame::NoFrame);
    view->setBackgroundBrush(Qt::gray);

    connect(magnifier, SIGNAL(scaleChanged(int)),
            this, SLOT(scaleTo(int)));
}

void GraphicsViewMagnifier::Private::scaleTo(int scale)
{
    view->setTransform(QTransform().scale(scale/100.0, scale/100.0));
}

#include "GraphicsViewMagnifier_p.moc"
