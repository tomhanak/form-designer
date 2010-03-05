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

#include <QtCore/QObject>

#include <QtTreePropertyBrowser>
#include <QtVariantEditorFactory>
#include <QtVariantPropertyManager>
#include <QtVariantProperty>

namespace sfd {
namespace gui {
namespace property_editor {

class TreePropertyWidget : public QtTreePropertyBrowser
{
    Q_OBJECT

public:
    TreePropertyWidget(QWidget* parent = 0);

public:
    using QtTreePropertyBrowser::addProperty;

    virtual QtVariantProperty* addProperty(int propType, const QString&name, const QVariant& value);
    //! sets value of existing QtPropert in manager
    void setValue(const QString& name, const QVariant& value);

public:
    //! \reimpl QtTreePropertyBrowser
    //@{
    virtual void clear();
    //@}

protected:
    //! \reimpl QtTreePropertyBrowser
    //@{
    virtual QWidget* createEditor(QtProperty* property, QWidget* parent);
    //@}

protected:
    virtual void setExpanded(QtProperty* property, bool expanded);
    virtual QtVariantProperty* findProperty(const QString& name) const;

protected Q_SLOTS:
    //! after reset btn clicked, it finds property and emit propertyReset(...)
    virtual void resetClicked();
    /*! \brief if property is not subproperty extract name of the property
     * and emit another signal */
    virtual void propertyChanged(QtProperty* property, const QVariant& value);

Q_SIGNALS:
    void propertyReset(const QString& name, const QVariant& value);
    void propValueChanged(const QString& name, const QVariant& value);

private:
    QtVariantEditorFactory* m_factory;
    QtVariantPropertyManager* m_manager;
    QtProperty* m_propGroup;
};

}}} // namespace
