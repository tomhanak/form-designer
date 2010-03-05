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

#include "TranslTableView_p.hpp"

#include <QtGui/QAction>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QHeaderView>
#include <QtGui/QMenu>
#include <QtGui/QSortFilterProxyModel>

#include "core/transl_table/UnifiedTranslTable.hpp"
#include "mva/transl_table/TranslTableModel.hpp"
#include "undo/transl_table/AddId.hpp"
#include "undo/transl_table/CloneId.hpp"
#include "undo/transl_table/RemoveId.hpp"
#include "undo/transl_table/AddLanguage.hpp"
#include "undo/transl_table/RenameLanguage.hpp"
#include "undo/transl_table/RemoveLanguage.hpp"

#include "LanguageNameDialog.hpp"
#include "TranslIdNameDialog.hpp"

using namespace sfd::gui::transl_table;

TranslTableView::TranslTableView(core::transl_table::UnifiedTranslTable *table, 
        QWidget *parent)
    : QTableView(parent)
    , p(new Private(this, table))
{
}

void TranslTableView::commitData(QWidget *editor)
{
    p->validateCurrentText(editor);
    QTableView::commitData(editor);
}

void TranslTableView::currentChanged(const QModelIndex &current, const 
        QModelIndex &previous)
{
    if (p->filter->mapToSource(current).column() > 1) {
        p->closePersistentEditors();
    }
    else {
        if (current.row() != previous.row()) {
            p->closePersistentEditors();
        }
        else if (p->isLastTextValid) {
            closePersistentEditor(previous);
            p->persistentEditorIndexes.removeOne(previous);
        }
        openPersistentEditor(current);
        p->persistentEditorIndexes.append(current);
    }
    QTableView::currentChanged(current, previous);
    resizeColumnToContents(currentIndex().column());
    resizeRowToContents(currentIndex().row());
}


TranslTableView::Private::Private(TranslTableView *publ, 
        core::transl_table::UnifiedTranslTable *table)
    : p(publ)
    , table(table)
    , model(new mva::transl_table::TranslTableModel(table, p))
    , filter(new QSortFilterProxyModel(p))
    , isLastTextValid(true)
    , addId(new QAction(p))
    , cloneId(new QAction(p))
    , removeId(new QAction(p))
    , addLanguage(new QAction(p))
    , renameLanguage(new QAction(p))
    , removeLanguage(new QAction(p))
    , idMenu(new QMenu(p))
    , languageMenu(new QMenu(p))
{
    Q_ASSERT(table != 0);

    p->setEditTriggers(QAbstractItemView::AnyKeyPressed
            | QAbstractItemView::DoubleClicked
            | QAbstractItemView::EditKeyPressed
            | QAbstractItemView::SelectedClicked);
    p->setAlternatingRowColors(true);
    p->setCornerButtonEnabled(false);
    p->setSortingEnabled(true);
    p->setSelectionBehavior(QAbstractItemView::SelectRows);
    p->setSelectionMode(QAbstractItemView::ExtendedSelection);

    filter->setDynamicSortFilter(true);
    filter->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filter->setSourceModel(model);
    p->setModel(filter);

    createPopupMenus();
    connect(table, SIGNAL(languageAdded(int)),
            this, SLOT(onLanguageAdded(int)));
    connect(table, SIGNAL(languageRemoved(int)),
            this, SLOT(onLanguageRemoved(int)));

    connect(table, SIGNAL(languageAdded(int)),
            p, SLOT(resizeColumnsToContents()));
    connect(table, SIGNAL(idAdded(int)),
            p, SLOT(resizeRowsToContents()));

    p->resizeColumnsToContents();
    p->resizeRowsToContents();
}

void TranslTableView::Private::closePersistentEditors()
{
    Q_FOREACH(QModelIndex index, persistentEditorIndexes) {
        p->closePersistentEditor(index);
    }
    persistentEditorIndexes.clear();
}

void TranslTableView::Private::validateCurrentText(QWidget *editor)
{
    Q_ASSERT(editor != 0);

    const QModelIndex index = filter->mapToSource(p->currentIndex());
    if (index.column() < 2) {
        const QString oldText = model->data(index).toString();
        const QString oldId = model->translId(index);
        const QString oldContext = model->context(index);
        const QString newText = editor->property("text").toString();
        const QString newId = (index.column() == 0) ? newText : oldId;
        const QString newContext = (index.column() == 1) ? newText : oldContext;

        if ((oldId != newId || oldContext != newContext)
                && table->contains(newId, newContext)) {

            QPalette palette;
            palette.setColor(QPalette::Base, QColor(Qt::red));
            editor->setPalette(palette);

            editor->setToolTip(tr("<html><body>"
                        "The identifier with TranslID=\"%1\" and "
                        "Context=\"%2\" already exists!<br/>"
                        "The unchaged value is still \"<b>%3</b>\""
                        "</body></html>")
                    .arg(newId).arg(newContext).arg(oldText));

            isLastTextValid = false;
            return;
        }
    }
    editor->setPalette(QPalette());
    editor->setToolTip(QString());
    isLastTextValid = true;
}

void TranslTableView::Private::createPopupMenus()
{
    addId->setIcon(QIcon(":/gui/transl_table/icons/id_add.png"));
    addId->setText(tr("Add Transl ID..."));
    connect(addId, SIGNAL(triggered()),
            this, SLOT(addIdTriggered()));

    cloneId->setIcon(QIcon(":/gui/transl_table/icons/id_clone.png"));
    cloneId->setText(tr("Clone Transl ID..."));
    cloneId->setEnabled(false);
    connect(cloneId, SIGNAL(triggered()),
            this, SLOT(cloneIdTriggered()));

    removeId->setIcon(QIcon(":/gui/transl_table/icons/id_remove.png"));
    removeId->setText(tr("Remove Selected Transl IDs"));
    removeId->setEnabled(false);
    connect(removeId, SIGNAL(triggered()),
            this, SLOT(removeIdTriggered()));

    idMenu->addAction(addId);
    idMenu->addAction(cloneId);
    idMenu->addAction(removeId);

    p->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(p->verticalHeader(), SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(idContextMenuRequest(QPoint)));

    addLanguage->setIcon(QIcon(":/gui/transl_table/icons/language_add.png"));
    addLanguage->setText(tr("Add Language..."));
    connect(addLanguage, SIGNAL(triggered()),
            this, SLOT(addLanguageTriggered()));

    renameLanguage->setText(tr("Rename Language..."));
    renameLanguage->setEnabled(false);
    connect(renameLanguage, SIGNAL(triggered()),
            this, SLOT(renameLanguageTriggered()));

    removeLanguage->setIcon(
            QIcon(":/gui/transl_table/icons/language_remove.png"));
    removeLanguage->setText(tr("Remove Language"));
    removeLanguage->setEnabled(false);
    connect(removeLanguage, SIGNAL(triggered()),
            this, SLOT(removeLanguageTriggered()));

    languageMenu->addAction(addLanguage);
    languageMenu->addAction(renameLanguage);
    languageMenu->addAction(removeLanguage);
    languageMenu->addSeparator();
    Q_FOREACH(int ulid, table->uniqueLanguageIds()) {
        onLanguageAdded(ulid);
    }

    p->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(p->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(languageContextMenuRequest(QPoint)));
}

void TranslTableView::Private::idContextMenuRequest(const QPoint &pos)
{
    const int row = filter->mapToSource(p->indexAt(pos)).row();
    const bool rowValid = row >= 0 && row < model->rowCount();
    if (rowValid && p->selectionModel()->selectedRows().isEmpty()) {
        p->selectRow(row);
    }
    cloneId->setEnabled(rowValid);
    removeId->setEnabled(!p->selectionModel()->selectedRows().isEmpty());
    idMenu->setProperty("clickedRow", row);
    idMenu->popup(p->verticalHeader()->mapToGlobal(pos));
}

void TranslTableView::Private::languageContextMenuRequest(const QPoint &pos)
{
    const int column = filter->mapToSource(p->indexAt(pos)).column();
    renameLanguage->setDisabled(column < 2);
    removeLanguage->setDisabled(column < 2);
    languageMenu->setProperty("clickedColumn", column);
    languageMenu->popup(p->horizontalHeader()->mapToGlobal(pos));
}

void TranslTableView::Private::onLanguageAdded(int ulid)
{
    Q_ASSERT(!languageVisibilityActions.keys().contains(ulid));
    QAction *action = new QAction(table->language(ulid), this);
    action->setCheckable(true);
    action->setChecked(true);
    connect(action, SIGNAL(toggled(bool)),
            this, SLOT(changeLanguageVisibility()));
    languageVisibilityActions[ulid] = action;
    languageMenu->addAction(action);
}

void TranslTableView::Private::onLanguageRemoved(int ulid)
{
    Q_ASSERT(languageVisibilityActions.keys().contains(ulid));
    QAction *action = languageVisibilityActions.take(ulid);
    languageMenu->removeAction(action);
    delete action;
}

void TranslTableView::Private::addIdTriggered()
{
    TranslIdNameDialog dialog(table, QString(), QString(), p);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    const QString newId = dialog.translId();
    const QString newContext = dialog.context();

    QUndoStack *stack = table->undoStack();
    Q_ASSERT(stack != 0);

    using undo::transl_table::AddId;
    AddId *command = new AddId(table, newId, newContext);

    const QString text(tr("New Transl ID added (id: %1, context: %2)")
            .arg(newId).arg(newContext));

    command->setText(text);
    stack->push(command);
}

void TranslTableView::Private::cloneIdTriggered()
{
    const int row = idMenu->property("clickedRow").toInt();
    Q_ASSERT(row >= 0);
    Q_ASSERT(row < model->rowCount());

    const QString oldId = model->translId(model->index(row, 0));
    const QString oldContext = model->context(model->index(row, 1));

    TranslIdNameDialog dialog(table, oldId, oldContext, p);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    const QString newId = dialog.translId();
    const QString newContext = dialog.context();
    
    QUndoStack *stack = table->undoStack();
    Q_ASSERT(stack != 0);

    using undo::transl_table::CloneId;
    CloneId *command = new CloneId(table, oldId, newId, oldContext, newContext);

    const QString text(tr("Transl ID clonned (id: %1, context: %2)")
            .arg(newId).arg(newContext));

    command->setText(text);
    stack->push(command);
}

void TranslTableView::Private::removeIdTriggered()
{
    const QModelIndexList indexes = p->selectionModel()->selectedRows();
    Q_ASSERT(indexes.count() > 0);
    QList<int> upidsToRemove;
    Q_FOREACH(QModelIndex index, indexes) {
        index = filter->mapToSource(index);
        const QString translId = model->translId(index);
        const QString context = model->context(index);
        upidsToRemove << table->uniqueRowId(translId, context);
    }

    QUndoStack *stack = table->undoStack();
    Q_ASSERT(stack != 0);

    QUndoCommand *macro = new QUndoCommand;
    macro->setText((upidsToRemove.count() > 1)
            ? tr("%Ln transl ID(s) removed", "", upidsToRemove.count())
            : tr("Transl ID removed (id: %1, context: %2)")
                .arg(table->translId(upidsToRemove.at(0)))
                .arg(table->context(upidsToRemove.at(0))));
    Q_FOREACH(int upid, upidsToRemove) {
        new undo::transl_table::RemoveId(table, table->translId(upid), 
                table->context(upid), macro);
    }
    stack->push(macro);
}

void TranslTableView::Private::addLanguageTriggered()
{
    LanguageNameDialog dialog(table, QString(), p);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    const QString newLanguage = dialog.language();

    QUndoStack *stack = table->undoStack();
    Q_ASSERT(stack != 0);

    using undo::transl_table::AddLanguage;
    AddLanguage *command = new AddLanguage(table, newLanguage);

    const QString text(tr("New language added (%1)").arg(newLanguage));

    command->setText(text);
    stack->push(command);
}

void TranslTableView::Private::renameLanguageTriggered()
{
    const int column = languageMenu->property("clickedColumn").toInt();
    Q_ASSERT(column >= 2);
    Q_ASSERT(column < model->columnCount());
    const QString oldLanguage =
        model->headerData(column, Qt::Horizontal).toString();

    LanguageNameDialog dialog(table, oldLanguage, p);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    const QString newLanguage = dialog.language();

    QUndoStack *stack = table->undoStack();
    Q_ASSERT(stack != 0);

    using undo::transl_table::RenameLanguage;
    RenameLanguage *command = new RenameLanguage(table, oldLanguage, 
            newLanguage);

    const QString text(tr("Language renamed (from: %1, to: %2)")
            .arg(oldLanguage).arg(newLanguage));

    command->setText(text);
    stack->push(command);
}

void TranslTableView::Private::removeLanguageTriggered()
{
    const int column = languageMenu->property("clickedColumn").toInt();
    Q_ASSERT(column >= 2);
    Q_ASSERT(column < model->columnCount());
    const QString language =
        model->headerData(column, Qt::Horizontal).toString();

    QUndoStack *stack = table->undoStack();
    Q_ASSERT(stack != 0);

    using undo::transl_table::RemoveLanguage;
    RemoveLanguage *command = new RemoveLanguage(table, language);

    const QString text(tr("Language removed (%1)").arg(language));

    command->setText(text);
    stack->push(command);
}

void TranslTableView::Private::changeLanguageVisibility()
{
    QAction *action = qobject_cast<QAction *>(sender());
    Q_ASSERT(action != 0);
    Q_ASSERT(languageVisibilityActions.values().contains(action));
    const int ulid = languageVisibilityActions.key(action);
    int column = -1;
    for (int n = 2; n < model->columnCount(); ++n) {
        if (model->headerData(n, Qt::Horizontal).toString()
                == table->language(ulid)) {
            column = n;
            break;
        }
    }
    Q_ASSERT(column >= 2);
    p->setColumnHidden(column, !languageVisibilityActions[ulid]->isChecked());
}

#include "TranslTableView_p.moc"
