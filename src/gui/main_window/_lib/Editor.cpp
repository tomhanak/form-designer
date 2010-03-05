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

#include "Editor_p.hpp"

#include <QtGui/QAction>

using namespace sfd::gui::main_window;

Editor::Editor(QWidget *parent)
    : QWidget(parent)
    , p(new Private(this))
{
}

/*! If your Editor class support magnification (AKA zooming), override this 
 * method to return associated instance of Magnifier.
 *
 * Default implementation returns \c 0.
 */
Editor::Magnifier *Editor::magnifier() const
{
    return 0;
}


Editor::Private::Private(Editor *publ)
    : p(publ)
{
}

/*!
 * \class sfd::gui::main_window::Editor::Magnifier
 */

Editor::Magnifier::Magnifier(QObject *parent)
    : QObject(parent)
    , p(new Private(this))
{
}

void Editor::Magnifier::setScale(int scale)
{
    Q_ASSERT(scale >= MIN_SCALE);
    Q_ASSERT(scale <= MAX_SCALE);

    p->scale = scale;

    p->scaleInAction->setEnabled(scale < MAX_SCALE);
    p->scaleOutAction->setEnabled(scale > MIN_SCALE);

    Q_EMIT scaleChanged(scale);
}

void Editor::Magnifier::scaleIn()
{
    const int newScale = static_cast<int>(scale() * 1.25);
    setScale(qMin(newScale, static_cast<int>(MAX_SCALE)));
}

void Editor::Magnifier::scaleOut()
{
    const int newScale = static_cast<int>(scale() * 0.75);
    setScale(qMax(newScale, static_cast<int>(MIN_SCALE)));
}


Editor::Magnifier::Private::Private(Editor::Magnifier *publ)
    : p(publ)
    , scale(100)
    , scaleInAction(new QAction(tr("Zoom in"), p))
    , scaleOutAction(new QAction(tr("Zoom out"), p))
{
    connect(scaleInAction, SIGNAL(triggered()),
            p, SLOT(scaleIn()));
    connect(scaleOutAction, SIGNAL(triggered()),
            p, SLOT(scaleOut()));
}
