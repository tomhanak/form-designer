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

#include "MDI.hpp"

#include <QtCore/QEvent>
#include <QtGui/QTabBar>
#include <QtGui/QMouseEvent>
#include <QtGui/QMenu>
#include <QtGui/QApplication>

using namespace sfd::gui::main_window;

class MDI::TabBar : public QTabBar
{
    Q_OBJECT

public:
    TabBar(QWidget *parent = 0);

Q_SIGNALS:
    void detachCurrentRequested();

protected:
    //! \reimpl QWidget
    //@{
    void mousePressEvent(QMouseEvent *event);
    //@}
};

MDI::MDI(EditorFromDocumentCreator *editorFromDocumentCreator, QWidget *parent)
    : QTabWidget(parent)
    , p_editorFromDocumentCreator(editorFromDocumentCreator)
{
    Q_ASSERT(editorFromDocumentCreator != 0);

    TabBar *tabBar = new TabBar(this);
    setTabBar(tabBar);
    qApp->installEventFilter(this);

    connect(tabBar, SIGNAL(detachCurrentRequested()),
            this, SLOT(detachCurrent()));
    connect(this, SIGNAL(currentChanged(int)),
            this, SLOT(onCurrentChanged(int)));
    connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*)),
            this, SLOT(onFocusChanged(QWidget*, QWidget*)));
}

void MDI::setProject(core::project::Project* project)
{
    Q_ASSERT(project != 0);
    
    p_project = project;

    if (p_project != 0) {
        connect(p_project, 
                SIGNAL(documentAdded(sfd::core::project::Document*)),
                this, SLOT(addDocument(sfd::core::project::Document*)));
        connect(p_project, 
                SIGNAL(aboutToRemoveDocument(sfd::core::project::Document*)),
                this, SLOT(removeDocument(sfd::core::project::Document*)));
        connect(p_project, 
                SIGNAL(activeDocumentChanged(sfd::core::project::Document*)),
                this, SLOT(raiseDocument(sfd::core::project::Document*)));
    }
}

bool MDI::eventFilter(QObject *watched, QEvent *event)
{
    Editor *const watchedEditor = qobject_cast<Editor*>(watched);

    if (watchedEditor != 0 && event->type() == QEvent::Close) {
        event->ignore();
        encapsulate(watchedEditor);
        return true;
    }
    else if ((watchedEditor != 0) && event->type() == QEvent::MouseButtonPress) {
        /* This handle switching between detached "Editors"
         * user must click inside of editor to activate the document.
         * If focus has some detached Editro and user clicks to MainWindow
         * to menu or other widget which is not Editor, it will not activate
         * Editor inside main window of active tabWidget, but keeps active
         * detached Editor as active to be able call menu actions for example.
         */
        QMouseEvent* mEvent = dynamic_cast<QMouseEvent*>(event);
        if (mEvent->button() == Qt::LeftButton) {
            sfd::core::project::Document* doc = watchedEditor->document();
            if (doc != p_project->activeDocument())
                p_project->setActiveDocument(doc);
        }
        return QTabWidget::eventFilter(watched, event);
    }
    else {
        return QTabWidget::eventFilter(watched, event);
    }
}

void MDI::activateDocument(QWidget* maybeEditorWidget)
{
    using sfd::core::project::Document;
    Editor* editor = qobject_cast<Editor*> (maybeEditorWidget);

    Document* doc = p_documents.key(qobject_cast<Editor*>(editor));
    if (doc == 0)
        return;

    p_project->setActiveDocument(doc);
}

void MDI::addDocument(core::project::Document *document)
{
    Q_ASSERT(document != 0);
    Q_ASSERT(!p_documents.contains(document));

    Editor *const editor = p_editorFromDocumentCreator->create(document);
    Q_ASSERT_X(editor != 0, __func__, "No editor for given document found");

    p_documents.insert(document, editor);

    editor->installEventFilter(this);

    encapsulate(editor);
}

void MDI::removeDocument(core::project::Document *document)
{
    Q_ASSERT(document != 0);
    Q_ASSERT(p_documents.contains(document));

    delete p_documents.take(document);
}

void MDI::raiseDocument(core::project::Document *document)
{
    Q_ASSERT(document != 0);
    Q_ASSERT(p_documents.contains(document));

    const int index = indexOf(p_documents.value(document));
    if (index != -1) {
        setCurrentIndex(index);
    }
    else {
        p_documents.value(document)->show();
        p_documents.value(document)->raise();
        p_documents.value(document)->activateWindow();
    }
}

void MDI::detachCurrent()
{
    Q_ASSERT(count() != 0);

    Editor *currentEditor = qobject_cast<Editor*>(currentWidget());

    removeTab(currentIndex());

    currentEditor->setParent(0);
    currentEditor->resize(currentEditor->size() / 1.5);
    currentEditor->show();
}

void MDI::encapsulate(Editor *editor)
{
    Q_ASSERT(p_documents.value(editor->document()) == editor);

    setCurrentIndex(addTab(editor, editor->document()->name()));
}

void MDI::onCurrentChanged(int index)
{
    activateDocument(widget(index));
}

void MDI::onFocusChanged(QWidget* old, QWidget* now)
{
    Q_UNUSED(old);
    Q_UNUSED(now);
    activateDocument(qApp->activeWindow());
}


/*!
 * \class MDI::TabBar
 */

MDI::TabBar::TabBar(QWidget *parent)
    : QTabBar(parent)
{
}

void MDI::TabBar::mousePressEvent(QMouseEvent *event)
{
    QTabBar::mousePressEvent(event);

    if (event->button() == Qt::RightButton && tabAt(event->pos()) == 
            currentIndex()) {
        QMenu menu;
        menu.addAction(tr("Detach"));
        if (menu.exec(event->globalPos()) != 0) {
            Q_EMIT detachCurrentRequested();
        }
    }
}

#include "MDI.moc"
