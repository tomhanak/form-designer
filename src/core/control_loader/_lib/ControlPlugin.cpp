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

#include "ControlPlugin_p.hpp"

#include <QtCore/QVariant>
#include <QtGui/QWidget>

using namespace sfd::core::control_loader;

const QByteArray ControlPlugin::MIME_TYPE = "application/x-sfd-control-plugin";

ControlPlugin::ControlPlugin()
    : QDesignerCustomWidgetInterface()
    , p(new Private(this))
{
}

bool ControlPlugin::isControl(QWidget *widget)
{
    static const QList<QByteArray> mandatoryProperties = QList<QByteArray>()
        << "pluginName";
        //<< "translId";

    Q_FOREACH(QByteArray mandatoryProperty, mandatoryProperties) {
        if (widget->dynamicPropertyNames().indexOf(mandatoryProperty) == -1) {
            return false;
        }
    }

    return true;
}

QString ControlPlugin::pluginName(QWidget *control)
{
    Q_ASSERT(isControl(control));

    return control->property("pluginName").toString();
}

void ControlPlugin::setTranslId(QWidget *control, const QString &translId)
{
    Q_ASSERT(isControl(control));

    control->setProperty("translId", translId);
}

QString ControlPlugin::translId(QWidget *control)
{
    Q_ASSERT(isControl(control));

    return control->property("translId").toString();
}

QWidget* ControlPlugin::createWidgetSafe(QWidget *parent)
{
    QWidget *const widget = createWidget(parent);
    if (isControl(widget)) {
        return widget;
    }
    else {
        qCritical("%s: Invalid plugin loaded - created widget is not a valid "
                "control widget", __func__);
        delete widget;
        return 0;
    }
}

QWidget* ControlPlugin::controlize(QWidget* widget, QString pluginName)
{
    widget->setProperty("pluginName", pluginName);

    return widget;
}


ControlPlugin::Private::Private(ControlPlugin *publ)
    : p(publ)
{
}
