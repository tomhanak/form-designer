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

#include "TreePropertyWidget.hpp"
#include "ResetButton.hpp"

#include <QtGui/QHBoxLayout>

using namespace sfd::gui::property_editor;

TreePropertyWidget::TreePropertyWidget(QWidget* parent)
    : QtTreePropertyBrowser(parent)
    , m_factory(new QtVariantEditorFactory(this))
    , m_manager(new QtVariantPropertyManager(this))
{
    setFactoryForManager(m_manager, m_factory);
    setPropertiesWithoutValueMarked(true);
    setIndentation(12);
    setResizeMode(QtTreePropertyBrowser::Interactive);

    connect(m_manager, SIGNAL(valueChanged(QtProperty*,QVariant)),
            this, SLOT(propertyChanged(QtProperty*,QVariant)));
    clear();
}

void TreePropertyWidget::clear()
{
    Q_ASSERT(m_manager != 0);
    m_manager->clear();
    m_propGroup = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(),
                    QString(tr("Properties")));
    addProperty(m_propGroup);
}

QtVariantProperty* TreePropertyWidget::addProperty(int propType, const QString&name,
    const QVariant& value)
{
    Q_ASSERT(m_propGroup != 0);
    
    QtVariantProperty* prop;
    prop = m_manager->addProperty(propType, name);

    if (prop != 0) {
        m_propGroup->addSubProperty(prop);
        TreePropertyWidget::setExpanded(prop, false);
        prop->setValue(value);
    }
    else {
        qWarning() << "Property type is not supported:" << 
            QVariant::typeToName((QVariant::Type)propType);
    }

    return prop;
}

void TreePropertyWidget::setValue(const QString& name, const QVariant& value)
{
    QtVariantProperty* property = findProperty(name);
    property->setValue(value);
}

void TreePropertyWidget::setExpanded(QtProperty* property, bool expanded)
{
    Q_ASSERT(property != 0);
    QList<QtBrowserItem*> topItems = topLevelItems();

    Q_ASSERT(topItems.size() == 1);
    QList<QtBrowserItem*> browserItems = topItems.at(0)->children();

    foreach (QtBrowserItem* item, browserItems) {
        if (item->property() == property) {
            QtTreePropertyBrowser::setExpanded(item, expanded);
            break;
        }
    }
}

QtVariantProperty* TreePropertyWidget::findProperty(const QString& name) const
{
    QtVariantProperty* found = 0;

    foreach (QtProperty* item, m_propGroup->subProperties()) {
        if (item->propertyName() == name) {
            found = dynamic_cast<QtVariantProperty*>(item);
            break;
        }
    }

    Q_ASSERT(found != 0);
    return found;
}

QWidget* TreePropertyWidget::createEditor(QtProperty* property, QWidget* parent)
{
    QWidget* editor = QtTreePropertyBrowser::createEditor(property, parent);
    //is 0 when property has attributes and user clicks on root item
    if (editor == 0) {
        return 0;
    }
    QWidget* w = new QWidget(parent);
    editor->setParent(w);
    ResetButton* resetBtn = new ResetButton(property, w);
    resetBtn->setFocusPolicy(Qt::StrongFocus);
    editor->setFocusPolicy(Qt::StrongFocus);
    connect(resetBtn, SIGNAL(clicked()), this, SLOT(resetClicked()));
    
    QHBoxLayout* hl = new QHBoxLayout(w);
    hl->addWidget(editor);
    hl->addWidget(resetBtn);
    hl->setSpacing(0);
    hl->setMargin(0);
    w->setLayout(hl);
    return w;
}

void TreePropertyWidget::resetClicked()
{
/* Problems happends if some object has properties and subproperties with
 * same name like QWidget has property Width and property Size with
 * subproperties Width/Height. Therefore we cannot search by property name.
 * Root property group must be removed from property set to avoid troubles.
 * Highest priority has searching via subproperties, if exist
 * the parent must be taken.
 */
    const ResetButton* resetBtn = qobject_cast<const ResetButton*>(sender());
    Q_ASSERT(resetBtn != 0);

    QtProperty* subPropToFind = resetBtn->property();
    QtProperty* propFound = 0;
    QtProperty* parentFound = 0;

    QSet<QtProperty*> allProp = m_manager->properties();
    allProp.remove(m_propGroup);

    foreach (QtProperty* prop, allProp) {
        if (prop == subPropToFind) {
            propFound = prop;
        }
        if (prop->subProperties().contains(subPropToFind)) {
            parentFound = prop;
            break;
        }
    }

    if (parentFound != 0)
        propFound = parentFound;

    QtVariantProperty* varProp = dynamic_cast<QtVariantProperty*>(propFound);
    Q_ASSERT(varProp != 0);
    emit propertyReset(varProp->propertyName(), varProp->value());
}

void TreePropertyWidget::propertyChanged(QtProperty* property, const QVariant& value)
{
    bool isSubProperty = false;
    QSet<QtProperty*> allProp = m_manager->properties();
    allProp.remove(m_propGroup);

    foreach (QtProperty* prop, allProp) {
        if (prop->subProperties().contains(property)) {
            isSubProperty = true;
            break;
        }
    }

    if (!isSubProperty)
        emit propValueChanged(property->propertyName(), value);
}
