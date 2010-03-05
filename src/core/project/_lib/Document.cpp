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

#include "Document_p.hpp"

using namespace sfd::core::project;

Document::Document(QObject *parent)
    : QObject(parent)
    , p(new Private(this))
{
}

QUndoStack* Document::undoStack()
{
    return p->undoStack;
}

void Document::setName(QString name)
{
    p->name = name;
}

QString Document::name() const
{
    return p->name;
}

QSet<Document::Type> Document::registeredTypes()
{
    static QSet<Document::Type> cached;

    if (cached.count() != Private::registeredTypes) {
        for (Type t = cached.count(); t < Private::registeredTypes; t++) {
            cached << t;
        }
    }

    return cached;
}

Document::Type Document::registerType()
{
    return Private::registeredTypes++;
}


Document::Type Document::Private::registeredTypes = 0;

Document::Private::Private(Document *publ)
    : p(publ)
    , undoStack(new QUndoStack(p))
{
}
