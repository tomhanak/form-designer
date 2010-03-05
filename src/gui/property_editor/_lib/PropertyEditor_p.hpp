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

#include "../PropertyEditor.hpp"
#include "TreePropertyWidget.hpp"
#include <QPointer>

namespace sfd {
namespace gui {
namespace property_editor {

class PropertyEditor::Private : QObject
{
    Q_OBJECT
    friend class PropertyEditor;
    p_ptr<PropertyEditor> p;

private:
    Private(PropertyEditor *publ);

private:
    //! It is here only for validation which objects properties are shown
    QPointer<QObject> object;
    TreePropertyWidget* propertyWidget;
    QPointer<sfd::core::project::Document> document;

private Q_SLOTS:
    //! we have to find plugin for the leader and get list of properties
    void onLeaderChanged(QWidget* leader);
    //! create undo command and put into a stack
    void onPropertyChanged(const QString& name, const QVariant& value);

};

PIMPL_DEFINE_DESTRUCTOR(PropertyEditor)

}}} // namespace
