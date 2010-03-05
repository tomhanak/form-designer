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

#include <QStringList>
#include <QtDesigner/QDesignerCustomWidgetInterface>

namespace sfd {
namespace core {
namespace control_loader {

class ControlPlugin : public QDesignerCustomWidgetInterface
{
public:
    static const QByteArray MIME_TYPE;

public:
    ControlPlugin();
    ~ControlPlugin();

public:
    //! Verifies that all mandatory properties are set on the given widget
    static bool isControl(QWidget *widget);

    static QString pluginName(QWidget *control);

    static void setTranslId(QWidget *control, const QString &translId);
    static QString translId(QWidget *control);

public:
    //! Ensures that widget returned by createWidget() isControl()
    QWidget *createWidgetSafe(QWidget *parent);

public:
    //! One class can be provided by two plugins (with differrent constraints)
    virtual QString pluginName() const = 0;
    //! Non visual controls cannot be placed directly on a form
    virtual bool isVisual() const = 0;
    //! User is not allowed to see/modify every Q_PROPERTY
    virtual QStringList userEditableProperties() const = 0;

protected:
    //! Updates \a widget's dynamic properties to pass the isControl() test
    static QWidget *controlize(QWidget* widget, QString pluginName);

protected:
    //! Wrapped by createWidgetSafe()
    using QDesignerCustomWidgetInterface::createWidget;

private:
    class Private;
    p_ptr<Private> p;
};

}}} // namespace

#if CONFIG__PIMPL_RELEASE_MODE
# include "_lib/ControlPlugin_p.hpp"
#endif
