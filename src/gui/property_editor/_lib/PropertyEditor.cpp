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

#include "PropertyEditor_p.hpp"

#include "../../../core/form/Form.hpp"
#include "../../../undo/property_editor/EditProperty.hpp"

#include <QtGui/QVBoxLayout>
#include <QtCore/QMetaProperty>
#include <QDebug>

using namespace sfd::gui::property_editor;

PropertyEditor::PropertyEditor(QWidget *parent)
    : QWidget(parent)
    , p(new Private(this))
{
    setWindowTitle(tr("Property Editor"));

    //for testing
    connect(this, SIGNAL(propertyReset(QString, QVariant)),
            this, SLOT(testPropertyReset(QString, QVariant)));
    //for reset
    connect(p->propertyWidget, SIGNAL(propertyReset(QString, QVariant)),
            this, SIGNAL(propertyReset(QString, QVariant)));
    //for undo
    connect(this, SIGNAL(propertyChanged(QString, QVariant)),
            p, SLOT(onPropertyChanged(QString, QVariant)));
}

void PropertyEditor::setProject(sfd::core::project::Project* project)
{
    Q_ASSERT(project != 0);

    connect(project, SIGNAL(activeDocumentChanged(sfd::core::project::Document*)),
            this, SLOT(onActiveDocumentChanged(sfd::core::project::Document*)));

    onActiveDocumentChanged(project->activeDocument());
}

void PropertyEditor::setObject(const QObject* object)
{
    if (object == p->object) {
        return;
    }

    disconnect(p->propertyWidget, SIGNAL(propValueChanged(QString, QVariant)),
               this, SIGNAL(propertyChanged(QString, QVariant)));

    p->object = const_cast<QObject*> (object);
    p->propertyWidget->clear();

    if (object == 0) {
        return;
    }

    const QMetaObject* meta = p->object->metaObject();
    int propCount = meta->propertyCount();
    if (propCount == 0) {
        return;
    }
    
    for (int i = 0; i < propCount; ++i) {
        QMetaProperty prop = meta->property(i);
        if (prop.isDesignable(p->object) && prop.isWritable()) {
            p->propertyWidget->addProperty((int)prop.type(),
                prop.name(), prop.read(object));
        }
    }
    connect(p->propertyWidget, SIGNAL(propValueChanged(QString, QVariant)),
            this, SIGNAL(propertyChanged(QString, QVariant)));
}

void PropertyEditor::testPropertyReset(const QString& name, const QVariant& value)
{
    qDebug() << "Reset called on:" << name << value;

    //testing property where reset button works
    if (name == "objectName") {
        p->object->setProperty("objectName", QVariant("testCheckBox"));
        reloadPropertyValue(p->object, name);
    }
}

void PropertyEditor::reloadPropertyValue(const QObject* object, const QString& name)
{
    Q_ASSERT(object == p->object);
    Q_ASSERT(object != 0);

    disconnect(p->propertyWidget, SIGNAL(propValueChanged(QString, QVariant)),
               this, SIGNAL(propertyChanged(QString, QVariant)));

    p->propertyWidget->setValue(name, object->property(qPrintable(name)));

    connect(p->propertyWidget, SIGNAL(propValueChanged(QString, QVariant)),
            this, SIGNAL(propertyChanged(QString, QVariant)));
}

void PropertyEditor::onActiveDocumentChanged(sfd::core::project::Document* document)
{
    sfd::core::form::Form* form =
        qobject_cast<sfd::core::form::Form*> (document);
    p->document = form;

    if (p->document == 0) {
        p->onLeaderChanged(0);
        return;
    }

    connect(form->selection(), SIGNAL(leaderChanged(QWidget*)),
            p, SLOT(onLeaderChanged(QWidget*)));

    p->onLeaderChanged(form->selection()->leader());
}




PropertyEditor::Private::Private(PropertyEditor *publ)
    : p(publ)
    , propertyWidget(new TreePropertyWidget(p))
{
    QVBoxLayout* vl = new QVBoxLayout(p);
    vl->setMargin(0);
    vl->setSpacing(0);
    vl->addWidget(propertyWidget);
    p->setLayout(vl);
}

void PropertyEditor::Private::onLeaderChanged(QWidget* leader)
{
    p->setObject(leader);
}

void PropertyEditor::Private::onPropertyChanged(const QString& name,
    const QVariant& value)
{
    using sfd::undo::property_editor::EditProperty;

    if (document == 0)
        return;

    QUndoStack* stack = document->undoStack();
    Q_ASSERT(stack != 0);
    Q_ASSERT(object != 0);
    const QVariant oldVal = object->property(qPrintable(name));

    EditProperty* command = new EditProperty(object.data(), name, oldVal, value);

    connect(command, SIGNAL(propertyChanged(const QObject*, const QString&)),
            p, SLOT(reloadPropertyValue(const QObject*, const QString&)));

    stack->push(command);
}




#include "PropertyEditor_p.moc"
