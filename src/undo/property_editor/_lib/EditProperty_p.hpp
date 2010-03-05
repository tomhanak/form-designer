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

#include "../EditProperty.hpp"

namespace sfd {
namespace undo {
namespace property_editor {

class EditProperty::Private
{
    friend class EditProperty;
    p_ptr<EditProperty> p;

private:
    Private(EditProperty *publ, QObject* object, const QString& propName,
        const QVariant& oldValue, const QVariant& newValue);

private:
    QPointer<QObject> object;
    QString propName;
    QVariant oldValue;
    QVariant newValue;
};

PIMPL_DEFINE_DESTRUCTOR(EditProperty)

}}} // namespace
