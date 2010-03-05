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

#include "../../core/project/Document.hpp"
#include "../../core/project/Project.hpp"

#include <QtCore/QObject>
#include <QtGui/QWidget>

namespace sfd {
namespace gui {
namespace property_editor {

class PropertyEditor : public QWidget
{
    Q_OBJECT

public:
    PropertyEditor(QWidget *parent = 0);
    ~PropertyEditor();

public:
    void setProject(sfd::core::project::Project* project);

Q_SIGNALS:
    void propertyChanged(const QString& name, const QVariant& value);
    void propertyReset(const QString& name, const QVariant& value);

public Q_SLOTS:
    //! set qobject which properties should be shown in p. editor
    void setObject(const QObject* object);
    //! \todo remove it later, its for testing only
    void testPropertyReset(const QString& name, const QVariant& value);
    //! check if object is shown in prop editor and reload value
    void reloadPropertyValue(const QObject* object, const QString& name);

    void onActiveDocumentChanged(sfd::core::project::Document* document);

private:
    class Private;
    p_ptr<Private> p;
};

}}} // namespace

#if CONFIG__PIMPL_RELEASE_MODE
# include "_lib/PropertyEditor_p.hpp"
#endif
