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

#include "Project_p.hpp"

#include "../TerminalInfo.hpp"

using namespace sfd::core;
using namespace sfd::core::project;

Project::Project(TerminalInfo *terminalInfo, QObject *parent)
    : QObject(parent)
    , p(new Private(this, terminalInfo))
{
}

QList<Document*> Project::documents() const
{
    return p->documents.values();
}

QList<Document*> Project::documents(Document::Type type) const
{
    return p->documents.values(type);
}

void Project::addDocument(Document *document)
{
    Q_ASSERT(document != 0);
    Q_ASSERT(!p->documents.values(document->type()).contains(document));

    p->documents.insertMulti(document->type(), document);

    Q_EMIT documentAdded(document);
}

void Project::removeDocument(Document *document)
{
    Q_ASSERT(document != 0);
    Q_ASSERT(p->documents.values().contains(document));

    Q_EMIT aboutToRemoveDocument(document);

    p->documents.remove(document->type(), document);
}

void Project::setActiveDocument(Document *document)
{
    Q_ASSERT(document != 0);
    //Q_ASSERT(document != p->activeDocument);

    if (document == p->activeDocument)
        return;

    p->activeDocument = document;
    Q_EMIT activeDocumentChanged(p->activeDocument);
}


Project::Private::Private(Project *publ, TerminalInfo *terminalInfo)
    : p(publ)
    , terminalInfo(terminalInfo)
    , activeDocument(0)
{
    Q_ASSERT(terminalInfo != 0);
}
