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

#include <QtCore/QList>
#include <QtCore/QObject>

#include "Document.hpp"

namespace sfd {
namespace core {
namespace project {

class TerminalInfo;

class Project : public QObject
{
    Q_OBJECT

public:
    Project(TerminalInfo *terminalInfo, QObject *parent = 0);
    ~Project();

public:
    TerminalInfo *terminalInfo() const;

public:
    QList<Document*> documents() const;
    QList<Document*> documents(Document::Type type) const;

    Document *activeDocument() const;

public Q_SLOTS:
    void addDocument(sfd::core::project::Document *document);
    void removeDocument(sfd::core::project::Document *document);

    void setActiveDocument(sfd::core::project::Document *document);

Q_SIGNALS:
    void documentAdded(sfd::core::project::Document *document);
    void aboutToRemoveDocument(sfd::core::project::Document *document);

    void activeDocumentChanged(sfd::core::project::Document *document);

private:
    class Private;
    p_ptr<Private> p;
};

}}} // namespace

#if CONFIG__PIMPL_RELEASE_MODE
# include "_lib/Project_p.hpp"
#endif
