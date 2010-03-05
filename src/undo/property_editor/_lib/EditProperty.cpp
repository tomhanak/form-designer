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

#include "EditProperty_p.hpp"

using namespace sfd::undo::property_editor;

EditProperty::EditProperty(QObject* object, const QString& propName,
        const QVariant& oldValue, const QVariant& newValue, QUndoCommand *parent)
    : QObject(0)
    , QUndoCommand(parent)
    , p(new Private(this, object, propName, oldValue, newValue))
{
    QString text;
    QDebug dbg(&text);
    dbg << propName << ":" << oldValue << "->" << newValue;
    setText(text);
}

void EditProperty::undo()
{
    setValue(p->oldValue);
}

void EditProperty::redo()
{
    setValue(p->newValue);
}

void EditProperty::setValue(const QVariant& value)
{
    Q_ASSERT(p->object != 0);
    p->object->setProperty(qPrintable(p->propName), value);
    emit propertyChanged(p->object, p->propName);
}

EditProperty::Private::Private(EditProperty *publ, QObject* object,
        const QString& propName, const QVariant& oldValue, const QVariant& newValue)
    : p(publ)
    , object(object)
    , propName(propName)
    , oldValue(oldValue)
    , newValue(newValue)
{
}
