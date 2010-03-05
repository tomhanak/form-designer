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

#include "TranslTableModel_p.hpp"

#include "core/transl_table/UnifiedTranslTable.hpp"
#include "undo/transl_table/EditText.hpp"
#include "undo/transl_table/RenameId.hpp"

using namespace sfd::mva::transl_table;

TranslTableModel::TranslTableModel(core::transl_table::UnifiedTranslTable 
        *table, QObject *parent)
    : QAbstractTableModel(parent)
    , p(new Private(this, table))
{
}

QString TranslTableModel::translId(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QString();
    }
    Q_ASSERT(index.row() < p->table->uniqueRowIds().size());
    return p->table->translId(p->table->uniqueRowIds().at(index.row()));
}

QString TranslTableModel::context(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QString();
    }
    Q_ASSERT(index.row() < p->table->uniqueRowIds().size());
    return p->table->context(p->table->uniqueRowIds().at(index.row()));
}

int TranslTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return p->table->uniqueRowIds().size();
}

int TranslTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    // One column for ID, one for Context + one per language
    return 2 + p->table->uniqueLanguageIds().size();
}

QVariant TranslTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        Q_ASSERT(index.column() < columnCount(QModelIndex()));
        Q_ASSERT(index.row() < rowCount(QModelIndex()));

        const int upid = p->table->uniqueRowIds().at(index.row());
        int ulid;
        switch (index.column()) {
        case 0:
            return p->table->translId(upid);
        case 1:
            return p->table->context(upid);
        default:
            ulid = p->table->uniqueLanguageIds().at(index.column() - 2);
            return p->table->text(upid, ulid);
        }
    }
    return QVariant();
}

QVariant TranslTableModel::headerData(int section, Qt::Orientation orientation, 
        int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Vertical) {
            return QString::number(section + 1); // First row number is 1
        }
        else {
            Q_ASSERT(section < columnCount(QModelIndex()));
            switch (section) {
            case 0:
                return tr("Transl ID");
            case 1:
                return tr("Context");
            default:
                return p->table->language(
                        p->table->uniqueLanguageIds().at(section - 2));
            }
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

bool TranslTableModel::setData(const QModelIndex &index, const QVariant &value, 
        int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        Q_ASSERT(index.row() < p->table->uniqueRowIds().size());
        Q_ASSERT(index.column() - 2 < p->table->uniqueLanguageIds().size());
        QUndoStack *stack = p->table->undoStack();
        Q_ASSERT(stack != 0);
        QString newText = value.toString();
        const int upid = p->table->uniqueRowIds().at(index.row());
        int ulid;
        switch (index.column()) {
        case 0:
            if (!p->table->contains(newText, p->table->context(upid))) {
                using undo::transl_table::RenameId;
                RenameId *command = new RenameId(p->table, 
                        p->table->translId(upid), newText, 
                        p->table->context(upid), p->table->context(upid));

                const QString text(
                        tr("Transl ID changed (from: %1, to: %2, context: %3)")
                        .arg(p->table->translId(upid)).arg(newText)
                        .arg(p->table->context(upid)));

                command->setText(text);
                stack->push(command);

                return true;
            }
            break;
        case 1:
            if (!p->table->contains(p->table->translId(upid), newText)) {
                using undo::transl_table::RenameId;
                RenameId *command = new RenameId(p->table, 
                        p->table->translId(upid), p->table->translId(upid), 
                        p->table->context(upid), newText);

                const QString text(
                        tr("Context changed (from: %1, to: %2, id: %3)")
                        .arg(p->table->context(upid)).arg(newText)
                        .arg(p->table->translId(upid)));

                command->setText(text);
                stack->push(command);

                return true;
            }
            break;
        default:
            ulid = p->table->uniqueLanguageIds().at(index.column() - 2);
            if (p->table->text(upid, ulid) != newText) {
                using undo::transl_table::EditText;
                EditText *command = new EditText(p->table, newText, 
                        p->table->language(ulid), p->table->translId(upid), 
                        p->table->context(upid));

                const QString text(tr("Text changed (from: %1, to: %2)")
                        .arg(p->table->text(upid, ulid)).arg(newText));

                command->setText(text);
                stack->push(command);
            }
            return true;
        }
    }
    return false;
}

Qt::ItemFlags TranslTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}


TranslTableModel::Private::Private(TranslTableModel *publ, 
        core::transl_table::UnifiedTranslTable *table)
    : QObject(publ)
    , p(publ)
    , table(table)
{
    connect(table, SIGNAL(aboutToAddId(int)),
            this, SLOT(onAboutToAddId(int)));
    connect(table, SIGNAL(idAdded(int)),
            this, SLOT(onIdAdded(int)));
    connect(table, SIGNAL(idRenamed(int,QString,QString)),
            this, SLOT(onIdRenamed(int,QString,QString)));
    connect(table, SIGNAL(aboutToRemoveId(int)),
            this, SLOT(onAboutToRemoveId(int)));
    connect(table, SIGNAL(idRemoved(int)),
            this, SLOT(onIdRemoved(int)));

    connect(table, SIGNAL(aboutToAddLanguage(int)),
            this, SLOT(onAboutToAddLanguage(int)));
    connect(table, SIGNAL(languageAdded(int)),
            this, SLOT(onLanguageAdded(int)));
    connect(table, SIGNAL(languageRenamed(int,QString)),
            this, SLOT(onLanguageRenamed(int,QString)));
    connect(table, SIGNAL(aboutToRemoveLanguage(int)),
            this, SLOT(onAboutToRemoveLanguage(int)));
    connect(table, SIGNAL(languageRemoved(int)),
            this, SLOT(onLanguageRemoved(int)));

    connect(table, SIGNAL(textChanged(int,int)),
            this, SLOT(onTextChanged(int,int)));
}

void TranslTableModel::Private::onAboutToAddId(int upid)
{
    Q_UNUSED(upid);
    // New ID will be added at the end, upid isn't registered yet
    const int index = table->uniqueRowIds().count();
    p->beginInsertRows(QModelIndex(), index, index);
}

void TranslTableModel::Private::onIdAdded(int upid)
{
    Q_UNUSED(upid);
    p->endInsertRows();
}

void TranslTableModel::Private::onIdRenamed(int upid, const QString &oldId, 
        const QString &oldContext)
{
    Q_UNUSED(oldId);
    Q_UNUSED(oldContext);
    const int row = table->uniqueRowIds().indexOf(upid);
    const QModelIndex indexId = p->index(row, 0);
    const QModelIndex indexContext = p->index(row, 1);
    Q_EMIT p->dataChanged(indexId, indexContext);
}

void TranslTableModel::Private::onAboutToRemoveId(int upid)
{
    const int index = table->uniqueRowIds().indexOf(upid);
    p->beginRemoveRows(QModelIndex(), index, index);
}

void TranslTableModel::Private::onIdRemoved(int upid)
{
    Q_UNUSED(upid);
    p->endRemoveRows();
}

void TranslTableModel::Private::onAboutToAddLanguage(int ulid)
{
    Q_UNUSED(ulid);
    // One column for ID, one for Context + one per language
    // New language will be added at the end, ulid isn't registered yet
    const int index = 2 + table->uniqueLanguageIds().count();
    p->beginInsertColumns(QModelIndex(), index, index);
}

void TranslTableModel::Private::onLanguageAdded(int ulid)
{
    Q_UNUSED(ulid);
    p->endInsertColumns();
}

void TranslTableModel::Private::onLanguageRenamed(int ulid, const QString 
        &oldLanguage)
{
    Q_UNUSED(oldLanguage);
    // One column for ID, one for Context + one per language
    const int index = 2 + table->uniqueLanguageIds().indexOf(ulid);
    Q_EMIT p->headerDataChanged(Qt::Horizontal, index, index);
}

void TranslTableModel::Private::onAboutToRemoveLanguage(int ulid)
{
    // One column for ID, one for Context + one per language
    const int index = 2 + table->uniqueLanguageIds().indexOf(ulid);
    p->beginRemoveColumns(QModelIndex(), index, index);
}

void TranslTableModel::Private::onLanguageRemoved(int ulid)
{
    Q_UNUSED(ulid);
    p->endRemoveColumns();
}

void TranslTableModel::Private::onTextChanged(int upid, int ulid)
{
    const int row = table->uniqueRowIds().indexOf(upid);
    const int column = 2 + table->uniqueLanguageIds().indexOf(ulid);
    const QModelIndex index = p->index(row, column);
    Q_EMIT p->dataChanged(index, index);
}

#include "TranslTableModel_p.moc"
