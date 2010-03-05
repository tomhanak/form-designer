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

#include "UnifiedTranslTable_p.hpp"

#include <QtCore/QStringList>

using namespace sfd::core::transl_table;
using namespace sfd::core;

UnifiedTranslTable::UnifiedTranslTable (QObject *parent)
    : project::Document(parent)
    , p(new Private(this))
{
}

QList<int> UnifiedTranslTable::uniqueRowIds() const
{
    return p->rowIds.keys();
}

int UnifiedTranslTable::uniqueRowId(const QString &id, const QString &context) 
    const
{
    QPair<QString, QString> row(id, context);
    Q_ASSERT(p->rowIds.values().contains(row));
    return p->rowIds.key(row);
}

bool UnifiedTranslTable::contains(const QString &id, const QString &context) 
    const
{
    QPair<QString, QString> row(id, context);
    return p->rowIds.values().contains(row);
}

QString UnifiedTranslTable::translId(int upid) const
{
    Q_ASSERT(p->rowIds.keys().contains(upid));
    return p->rowIds.value(upid).first;
}

QString UnifiedTranslTable::context(int upid) const
{
    Q_ASSERT(p->rowIds.keys().contains(upid));
    return p->rowIds.value(upid).second;
}

QList<int> UnifiedTranslTable::uniqueLanguageIds() const
{
    return p->languages.keys();
}

int UnifiedTranslTable::uniqueLanguageId(const QString &language) const
{
    Q_ASSERT(p->languages.values().contains(language));
    return p->languages.key(language);
}

QString UnifiedTranslTable::language(int ulid) const
{
    Q_ASSERT(p->languages.keys().contains(ulid));
    return p->languages.value(ulid);
}

QStringList UnifiedTranslTable::languages() const
{
    return p->languages.values();
}

QString UnifiedTranslTable::text(int upid, int ulid) const
{
    Q_ASSERT(p->texts.keys().contains(upid));
    Q_ASSERT(p->languages.keys().contains(ulid));
    return p->texts[upid][ulid];
}

project::Document::Type UnifiedTranslTable::type() const
{
    static Type type = Document::registerType();
    return type;
}

void UnifiedTranslTable::addId(const QString &id, const QString &context)
{
    QPair<QString, QString> row(id, context);
    Q_ASSERT(!p->rowIds.values().contains(row));
    const int upid = p->newUid();
    Q_EMIT aboutToAddId(upid);
    p->rowIds[upid] = row;
    p->texts[upid] = QMap<int, QString>();
    Q_FOREACH(int ulid, p->languages.keys()) {
        p->texts[upid][ulid] = QString();
    }
    Q_EMIT idAdded(upid);
}

void UnifiedTranslTable::renameId(int upid, const QString &newId, const QString 
        &newContext)
{
    QPair<QString, QString> row(newId, newContext);
    Q_ASSERT(p->rowIds.keys().contains(upid));
    if (translId(upid) == newId && context(upid) == newContext) {
        return;
    }
    Q_ASSERT(!p->rowIds.values().contains(row));
    Q_EMIT aboutToRenameId(upid, newId, newContext);
    const QString oldId = translId(upid);
    const QString oldContext = context(upid);
    p->rowIds[upid] = row;
    Q_EMIT idRenamed(upid, oldId, oldContext);
}

void UnifiedTranslTable::removeId(int upid)
{
    Q_ASSERT(p->texts.keys().contains(upid));
    Q_EMIT aboutToRemoveId(upid);
    p->texts.remove(upid);
    p->rowIds.remove(upid);
    Q_EMIT idRemoved(upid);
}

void UnifiedTranslTable::addLanguage(const QString &language)
{
    Q_ASSERT(!p->languages.values().contains(language));
    const int ulid = p->newUid();
    Q_EMIT aboutToAddLanguage(ulid);
    p->languages[ulid] = language;
    Q_FOREACH(int upid, p->texts.keys()) {
        p->texts[upid][ulid] = QString();
    }
    Q_EMIT languageAdded(ulid);
}

void UnifiedTranslTable::renameLanguage(int ulid, const QString &newLanguage)
{
    Q_ASSERT(p->languages.keys().contains(ulid));
    if (language(ulid) == newLanguage) {
        return;
    }
    Q_ASSERT(!p->languages.values().contains(newLanguage));
    Q_EMIT aboutToRenameLanguage(ulid, newLanguage);
    const QString oldLanguage = p->languages[ulid];
    p->languages[ulid] = newLanguage;
    Q_EMIT languageRenamed(ulid, oldLanguage);
}

void UnifiedTranslTable::removeLanguage(int ulid)
{
    Q_ASSERT(p->languages.keys().contains(ulid));
    Q_EMIT aboutToRemoveLanguage(ulid);
    p->languages.remove(ulid);
    Q_FOREACH(int upid, p->texts.keys()) {
        p->texts[upid].remove(ulid);
    }
    Q_EMIT languageRemoved(ulid);
}

void UnifiedTranslTable::setText(const QString &text, int upid, int ulid)
{
    Q_ASSERT(p->texts.keys().contains(upid));
    Q_ASSERT(p->languages.keys().contains(ulid));
    if (p->texts[upid][ulid] == text) {
        return;
    }
    p->texts[upid][ulid] = text;
    Q_EMIT textChanged(upid, ulid);
}


UnifiedTranslTable::Private::Private(UnifiedTranslTable *publ)
    : p(publ)
{
}
