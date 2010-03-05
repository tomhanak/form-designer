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

#include "MainWindow_p.hpp"

#include <QtGui/QUndoGroup>
#include <QtGui/QUndoView>

#include "ui_MainWindow.hpp"
#include "core/project/Project.hpp"

using namespace sfd::gui::main_window;

MainWindow::MainWindow(EditorFromDocumentCreator *editorFromDocumentCreator, 
        QWidget *parent)
    : QMainWindow(parent)
    , p(new Private(this, editorFromDocumentCreator))
{
}

void MainWindow::dockPropertyEditor(QWidget* propertyEditor)
{
    Q_ASSERT(propertyEditor != 0);
    p->ui->propertyEditorDock->setWidget(propertyEditor);
}

void MainWindow::dockDocumentSwitch(QWidget* documentSwitch)
{
    Q_ASSERT(documentSwitch != 0);
    p->ui->documentsDock->setWidget(documentSwitch);
}

void MainWindow::dockControlBox(QWidget* controlBox)
{
    Q_ASSERT(controlBox != 0);
    p->ui->pluginsDock->setWidget(controlBox);
}

void MainWindow::dockUndoStack(QWidget* undoStack)
{
    Q_ASSERT(undoStack != 0);
    p->ui->undoStackDock->setWidget(undoStack);
}

void MainWindow::setProject(core::project::Project *project)
{
    Q_ASSERT(project != p->project);

    if (p->project != 0) {
        Q_ASSERT_X(false, __func__, "Project closing not implemented yet!");
    }

    p->project = project;
    p->mdi->setProject(project);

    connect(p->project, SIGNAL(activeDocumentChanged(sfd::core::project::Document*)),
            this, SLOT(onActiveDocumentChanged(sfd::core::project::Document*)));
}

void MainWindow::onActiveDocumentChanged(sfd::core::project::Document *document)
{
    Q_ASSERT(document != 0);
    p->undoGroup->setActiveStack(document->undoStack());
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        p->ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

MainWindow::Private::Private(MainWindow *publ, EditorFromDocumentCreator 
        *editorFromDocumentCreator)
    : QObject(publ)
    , p(publ)
    , ui(new Ui::MainWindow)
    , mdi(new MDI(editorFromDocumentCreator))
    , undoGroup(new QUndoGroup(publ))
    , undoView(new QUndoView(publ))

{
    ui->setupUi(p);

    QAction *action = undoGroup->createUndoAction(p);
    action->setIcon(ui->actionUndo->icon());
    action->setShortcuts(ui->actionUndo->shortcuts());
    action->setToolTip(ui->actionUndo->toolTip());
    action->setWhatsThis(ui->actionUndo->whatsThis());
    ui->menuEdit->insertAction(ui->actionUndo, action);
    ui->actionUndo->deleteLater();

    action = undoGroup->createRedoAction(p);
    action->setIcon(ui->actionRedo->icon());
    action->setShortcuts(ui->actionRedo->shortcuts());
    action->setToolTip(ui->actionRedo->toolTip());
    action->setWhatsThis(ui->actionRedo->whatsThis());
    ui->menuEdit->insertAction(ui->actionRedo, action);
    ui->actionRedo->deleteLater();

    undoView->setGroup(undoGroup);
    ui->undoStackDock->setWidget(undoView);

    connect(ui->showAboutDialog, SIGNAL(triggered()),
            this, SLOT(showAboutDialog()));

    connect(ui->showSettingsDialog, SIGNAL(triggered()),
            this, SLOT(showSettingsDialog()));

    p->setCentralWidget(mdi);
}

void MainWindow::Private::showAboutDialog()
{
    if (aboutDialog == 0) {
        aboutDialog = new AboutDialog;
        aboutDialog->setAttribute(Qt::WA_DeleteOnClose);
    }

    aboutDialog->show();
    aboutDialog->raise();
    aboutDialog->activateWindow();
}

void MainWindow::Private::showSettingsDialog()
{
    if (settingsPage == 0) {
        settingsPage = new SettingsPage;
    }

    settingsPage->show();
    settingsPage->raise();
    settingsPage->activateWindow();
}

#include "MainWindow_p.moc"
