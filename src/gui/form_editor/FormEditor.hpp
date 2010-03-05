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
#include <QtCore/QSet>
#include <QtGui/QWidget>

#include "core/form/Form.hpp"
#include "gui/main_window/Editor.hpp"
#include "gui/main_window/GraphicsViewMagnifier.hpp"

namespace sfd {

namespace core {

namespace control_loader {
    class ControlPluginLoader;
}

namespace form {
    class Form;
}

namespace project {
    class Document;
}}

namespace gui {
namespace form_editor {

class FormEditor : public main_window::Editor
{
    Q_OBJECT

public:
    class Grid;
    class Creator;

public:
    FormEditor(core::form::Form *form, Grid *grid, 
            core::control_loader::ControlPluginLoader *controlPluginLoader, 
            QWidget *parent = 0);
    ~FormEditor();

public:
    core::form::Form *form() const;

public:
    Grid *grid() const;

public:
    //! \reimpl Editor
    //@{
    virtual core::form::Form *document() const;
    //@}

protected:
    //! \reimpl QWidget
    //@{
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    //@}

private:
    class Private;
    p_ptr<Private> p;
};

class FormEditor::Grid : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int xStep READ xStep WRITE setXStep NOTIFY xStepChanged)
    Q_PROPERTY(int yStep READ yStep WRITE setYStep NOTIFY yStepChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY 
            visibleChanged)
    Q_PROPERTY(bool xEnabled READ isXEnabled WRITE setXEnabled NOTIFY 
            xEnabledChanged)
    Q_PROPERTY(bool yEnabled READ isYEnabled WRITE setYEnabled NOTIFY 
            yEnabledChanged)

public:
    Grid(QObject *parent = 0);
    ~Grid();

public:
    int xStep() const;
    int yStep() const;

    bool isVisible() const;
    QAction *visibleAction() const;

    bool isXEnabled() const;
    QAction *xEnabledAction() const;

    bool isYEnabled() const;
    QAction *yEnabledAction() const;

public:
    void paint(QPainter *painter, const QRect &rect) const;
    QPoint align(const QPoint &point) const;
    bool diff(const QPoint &point1, const QPoint &point2) const;

public Q_SLOTS:
    void setXStep(int xStep);
    void setYStep(int yStep);

    void setVisible(bool visible);

    void setXEnabled(bool xEnabled);
    void setYEnabled(bool yEnabled);

Q_SIGNALS:
    void xStepChanged(int xStep);
    void yStepChanged(int yStep);

    void visibleChanged(bool visible);

    void xEnabledChanged(bool xEnabled);
    void yEnabledChanged(bool yEnabled);

private:
    class Private;
    p_ptr<Private> p;
};

class FormEditor::Creator : public main_window::EditorFromDocumentCreator
{
public:
    Creator(Grid *grid, core::control_loader::ControlPluginLoader 
            *controlPluginLoader);

protected:
    //! \reimpl AFromBCreator
    //@{
    virtual Editor *tryToCreate(core::project::Document *document) const;
    //@}

private:
    const QPointer<Grid> p_grid;
    const QPointer<core::control_loader::ControlPluginLoader> 
        p_controlPluginLoader;
};

}}} // namespace

#if CONFIG__PIMPL_RELEASE_MODE
# include "_lib/FormEditor_p.hpp"
#endif
