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

#include <QtCore/QPointer>
#include <QtCore/QMap>
#include <QtGui/QFrame>

#include "../FormEditor.hpp"

namespace sfd {

namespace core {
namespace form {
    class Form;
}}

namespace gui {
namespace form_editor {

//! Put on top of FormEditor, enables to modify selected controls' geometry
class GeometryEditor : public QFrame
{
    Q_OBJECT

public:
    GeometryEditor(const core::form::Form &form, FormEditor *formEditor);

public:
    FormEditor *formEditor() const;
    virtual bool eventFilter(QObject *watched, QEvent *event);


public Q_SLOTS:
    void startEditing(QWidget *widget);
    void stopEditing(QWidget *widget);

protected:
    //! \reimpl QWidget
    //@{
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    //@}

private:
    class ControlManipulator;

private:
    void mouseReleaseHandler(QMouseEvent *event);

private:
    QPointer<FormEditor> p_formEditor;
    QMap<QWidget*, ControlManipulator*> p_controlManipulators;

    QPoint p_lastCursorPos;
};

inline FormEditor *GeometryEditor::formEditor() const
{
    return p_formEditor;
}

}}} // namespace
