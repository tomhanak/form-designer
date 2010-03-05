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

#include "core/project/Document.hpp"

namespace sfd {
namespace core {
namespace transl_table {

class UnifiedTranslTable : public project::Document
{
    Q_OBJECT

public:
    UnifiedTranslTable(QObject *parent = 0);
    ~UnifiedTranslTable();

public:
    //! A List of Unique Transl ID Identifiers (upid)
    QList<int> uniqueRowIds() const;
    //! Unique Transl ID Identifier (upid) for given \a id and \a context
    int uniqueRowId(const QString &id, const QString &context = QString()) 
        const;
    bool contains(const QString &id, const QString &context = QString()) 
        const;
    QString translId(int upid) const;
    QString context(int upid) const;

    //! A List of Unique Language Identifiers (ulid)
    QList<int> uniqueLanguageIds() const;
    //! Unique Language Identifier (ulid) for given \a language
    int uniqueLanguageId(const QString &language) const;
    QString language(int ulid) const;

    QStringList languages() const;

    QString text(int upid, int ulid) const;

public:
    //! \reimpl project::Document
    //@{
    virtual Type type() const;
    //@}

public Q_SLOTS:
    void addId(const QString &id, const QString &context = QString());
    void renameId(int upid, const QString &newId, const QString &newContext = 
            QString());
    void removeId(int upid);

    void addLanguage(const QString &language);
    void renameLanguage(int ulid, const QString &newLanguage);
    void removeLanguage(int ulid);

    void setText(const QString &text, int upid, int ulid);

Q_SIGNALS:
    void aboutToAddId(int upid);
    void idAdded(int upid);
    void aboutToRenameId(int upid, const QString &newId, const QString 
            &newContext);
    void idRenamed(int upid, const QString &oldId, const QString &oldContext);
    void aboutToRemoveId(int upid);
    void idRemoved(int upid);

    void aboutToAddLanguage(int ulid);
    void languageAdded(int ulid);
    void aboutToRenameLanguage(int ulid, const QString &newLanguage);
    void languageRenamed(int ulid, const QString &oldLanguage);
    void aboutToRemoveLanguage(int ulid);
    void languageRemoved(int ulid);

    void textChanged(int upid, int ulid);

private:
    class Private;
    p_ptr<Private> p;
};

}}} // namespace

#if CONFIG__PIMPL_RELEASE_MODE
# include "_lib/UnifiedTranslTable_p.hpp"
#endif
