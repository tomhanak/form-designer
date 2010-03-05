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

#include <QtGui/QTabWidget>
#include <QPointer>

#include "../Editor.hpp"
#include "../../../core/project/Project.hpp"
#include "../../../core/project/Document.hpp"

namespace sfd {
namespace gui {
namespace main_window {

class MDI : public QTabWidget
{
    Q_OBJECT

public:
    MDI(EditorFromDocumentCreator *editorFromDocumentCreator, QWidget *parent = 
            0);

public:
    void setProject(core::project::Project* project);

public:
    //! \reimpl QObject
    //@{
    virtual bool eventFilter(QObject *watched, QEvent *event);
    //@}

private:
    class TabBar;

private:
    void activateDocument(QWidget* maybeEditorWidget);

private Q_SLOTS:
    void addDocument(sfd::core::project::Document *document);
    void removeDocument(sfd::core::project::Document *document);
    void raiseDocument(sfd::core::project::Document *document);
    void detachCurrent();
    void encapsulate(Editor *editor);
    void onCurrentChanged(int index);
    void onFocusChanged(QWidget* old, QWidget* now);


private:
    EditorFromDocumentCreator *const p_editorFromDocumentCreator;
    QMap<core::project::Document*, Editor*> p_documents;
    QPointer<core::project::Project> p_project;
};

}}} // namespace
