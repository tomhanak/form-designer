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

#include "../TranslTableModel.hpp"

namespace sfd {

namespace core {
namespace transl_table {
    class UnifiedTranslTable;
}}

namespace mva {
namespace transl_table {

class TranslTableModel::Private : QObject
{
    Q_OBJECT

    friend class TranslTableModel;
    p_ptr<TranslTableModel> p;

private:
    enum IdentifierType { IdentifierId, IdentifierContext };

private:
    Private(TranslTableModel *publ, core::transl_table::UnifiedTranslTable 
            *table);

private Q_SLOTS:
    void onAboutToAddId(int upid);
    void onIdAdded(int upid);
    void onIdRenamed(int upid, const QString &oldId, const QString &oldContext);
    void onAboutToRemoveId(int upid);
    void onIdRemoved(int upid);

    void onAboutToAddLanguage(int ulid);
    void onLanguageAdded(int ulid);
    void onLanguageRenamed(int ulid, const QString &oldLanguage);
    void onAboutToRemoveLanguage(int ulid);
    void onLanguageRemoved(int ulid);

    void onTextChanged(int upid, int ulid);

private:
    const QPointer<core::transl_table::UnifiedTranslTable> table;
};

PIMPL_DEFINE_DESTRUCTOR(TranslTableModel)

}}} // namespace
