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

#include <QtCore/QPointer>

#include "../TranslTableView.hpp"

class QSortFilterProxyModel;

namespace sfd {

namespace core {
namespace transl_table {
    class UnifiedTranslTable;
}}

namespace mva {
namespace transl_table {
    class TranslTableModel;
}}

namespace gui {
namespace transl_table {

class TranslTableView::Private : QObject
{
    Q_OBJECT

    friend class TranslTableView;
    p_ptr<TranslTableView> p;

private:
    Private(TranslTableView *publ, core::transl_table::UnifiedTranslTable 
            *table);

private:
    void closePersistentEditors();
    void validateCurrentText(QWidget *editor);

    void createPopupMenus();

private Q_SLOTS:
    void idContextMenuRequest(const QPoint &pos);
    void languageContextMenuRequest(const QPoint &pos);

    void onLanguageAdded(int ulid);
    void onLanguageRemoved(int ulid);

    void addIdTriggered();
    void cloneIdTriggered();
    void removeIdTriggered();

    void addLanguageTriggered();
    void renameLanguageTriggered();
    void removeLanguageTriggered();

    void changeLanguageVisibility();

private:
    const QPointer<core::transl_table::UnifiedTranslTable> table;
    const QPointer<mva::transl_table::TranslTableModel> model;
    const QPointer<QSortFilterProxyModel> filter;

    QList<QModelIndex> persistentEditorIndexes;
    bool isLastTextValid;

    const QPointer<QAction> addId;
    const QPointer<QAction> cloneId;
    const QPointer<QAction> removeId;

    const QPointer<QAction> addLanguage;
    const QPointer<QAction> renameLanguage;
    const QPointer<QAction> removeLanguage;
    //   ulid action
    QMap<int, QPointer<QAction> > languageVisibilityActions;

    const QPointer<QMenu> idMenu;
    const QPointer<QMenu> languageMenu;
};

PIMPL_DEFINE_DESTRUCTOR(TranslTableView)

}}} // namespace
