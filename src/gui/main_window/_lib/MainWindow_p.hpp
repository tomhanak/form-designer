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

#include "../MainWindow.hpp"

#include <QtCore/QPointer>

#include "AboutDialog.hpp"
#include "MDI.hpp"
#include "SettingsPage.hpp"

class QUndoGroup;
class QUndoView;

namespace sfd {
namespace gui {
namespace main_window {

namespace Ui {
    class MainWindow;
}

class MainWindow::Private : public QObject
{
    Q_OBJECT

    friend class MainWindow;
    p_ptr<MainWindow> p;

private:
    Private(MainWindow *publ, EditorFromDocumentCreator 
            *editorFromDocumentCreator);

private Q_SLOTS:
    void showAboutDialog();
    void showSettingsDialog();

private:
    Ui::MainWindow *ui;

    const QPointer<MDI> mdi;
    QPointer<core::project::Project> project;
    QPointer<SettingsPage> settingsPage;
    QPointer<AboutDialog> aboutDialog;
    QPointer<QUndoGroup> undoGroup;
    QPointer<QUndoView> undoView;
};

PIMPL_DEFINE_DESTRUCTOR(MainWindow)

}}} // namespace
