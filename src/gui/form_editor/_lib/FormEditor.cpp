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

#include "FormEditor_p.hpp"

#include <QtGui/QPainter>
#include <QtGui/QResizeEvent>

#include "GeometryEditor.hpp"
#include "core/control_loader/ControlPluginLoader.hpp"
#include "core/control_loader/ControlPlugin.hpp"
#include "core/form/Form.hpp"

#if !defined(CONFIG__DISABLE_FORM_EDITOR_MAGNIFIER)
# define CONFIG__DISABLE_FORM_EDITOR_MAGNIFIER (false)
#endif

using namespace sfd::gui::form_editor;
using namespace sfd::gui::main_window;

FormEditor::FormEditor(core::form::Form *form, Grid *grid, 
        core::control_loader::ControlPluginLoader *controlPluginLoader, QWidget 
        *parent)
    : Editor(parent)
    , p(new Private(this, form, grid, controlPluginLoader))
{
}

void FormEditor::paintEvent(QPaintEvent *event)
{
    if (p->grid->isVisible()) {
        QPainter painter(this);
        grid()->paint(&painter, event->rect());
    }
}

void FormEditor::resizeEvent(QResizeEvent *event)
{
    p->geometryEditor->resize(event->size());
}

void FormEditor::mousePressEvent(QMouseEvent *event)
{
    QWidget *const w = p->form->widget()->childAt(event->pos());

    if (w == 0) {
        p->form->selection()->clear();
        return;
    }

    if (!p->form->selection()->isEmpty() && 
            (*p->form->selection()->constBegin())->parent() != w->parent()) {
        p->form->selection()->clear();
    }

    if (!p->form->selection()->contains(w)) {
        if (!(event->modifiers() & Qt::ControlModifier)) {
            p->form->selection()->clear();
        }

        p->form->selection()->include(w);
    }
    else {
        if (event->modifiers() & Qt::ControlModifier) {
            p->form->selection()->exclude(w);
        }
        else {
            p->form->selection()->setLeader(w);
        }
    }
}

void FormEditor::dragEnterEvent(QDragEnterEvent *event)
{
    using core::control_loader::ControlPlugin;

    if (event->mimeData()->hasFormat(ControlPlugin::MIME_TYPE))
        event->acceptProposedAction();
}

void FormEditor::dropEvent(QDropEvent *event)
{
    using core::control_loader::ControlPlugin;

    const QString type(event->mimeData()->data(ControlPlugin::MIME_TYPE));

    ControlPlugin *const plugin = p->controlPluginLoader->plugin(type);
    QWidget *const control = plugin->createWidgetSafe(0);
    control->move(event->pos());
    p->form->addControl(control);

    event->acceptProposedAction();
}

void FormEditor::keyPressEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Delete)) {
        const QSet<QWidget*> selection(p->form->selection()->toConstSet());
        Q_FOREACH(QWidget *control, selection) {
#if defined(QT_DEBUG)
            if (!p->form->controls().contains(control)) {
                qWarning("%s: It seems you have a nested control in this form "
                        "-- as this is possible for testing only, delete "
                        "request will be ignored to prevent the application "
                        "to crash on failed assertion in Form.", __func__);
                continue;
            }
#endif
            p->form->removeControl(control);
            delete control;
        }

        event->accept();
    }
    else {
        Editor::keyPressEvent(event);
    }
}


FormEditor::Private::Private(FormEditor *publ, core::form::Form *form, Grid 
        *grid, core::control_loader::ControlPluginLoader *controlPluginLoader)
    : p(publ)
    , form(form)
    , grid(grid)
    , controlPluginLoader(controlPluginLoader)
    , geometryEditor(new GeometryEditor(*form, p))
{
    Q_ASSERT(form != 0);
    Q_ASSERT(grid != 0);
    Q_ASSERT(controlPluginLoader != 0);

    p->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    p->resize(form->widget()->size());

    form->widget()->setParent(p);

    geometryEditor->resize(p->size());
    geometryEditor->raise();

    p->setAcceptDrops(true);
    p->setFocusPolicy(Qt::StrongFocus);
}

/*!
 * \class sfd::gui::form_editor::FormEditor::Grid
 */

FormEditor::Grid::Grid(QObject *parent)
    : QObject(parent)
    , p(new Private(this))
{
}

void FormEditor::Grid::setXStep(int xStep)
{
    Q_ASSERT(xStep > 1);

    p->xStep = xStep;

    Q_EMIT xStepChanged(xStep);
}

void FormEditor::Grid::setYStep(int yStep)
{
    Q_ASSERT(yStep > 1);

    p->yStep = yStep;

    Q_EMIT yStepChanged(yStep);
}

void FormEditor::Grid::setVisible(bool visible)
{
    Q_ASSERT(visible != p->visibleAction->isChecked());

    p->visibleAction->setChecked(visible);
}

void FormEditor::Grid::setXEnabled(bool xEnabled)
{
    Q_ASSERT(xEnabled != p->xEnabledAction->isChecked());

    p->xEnabledAction->setChecked(xEnabled);
}

void FormEditor::Grid::setYEnabled(bool yEnabled)
{
    Q_ASSERT(yEnabled != p->yEnabledAction->isChecked());

    p->yEnabledAction->setChecked(yEnabled);
}

void FormEditor::Grid::paint(QPainter *painter, const QRect &rect) const
{
    for (int x = (rect.x() / xStep()) * xStep(); x <= rect.right(); x += 
            xStep()) {
        for (int y = (rect.y() / yStep()) * yStep(); y <= rect.bottom(); y += 
                yStep()) {
            painter->drawPoint(x, y);
        }
    }
}


FormEditor::Grid::Private::Private(FormEditor::Grid *publ)
    : p(publ)
    , xStep(10)
    , yStep(10)
    , visibleAction(new QAction(tr("Show grid"), p))
    , xEnabledAction(new QAction(tr("Snap to X grid"), p))
    , yEnabledAction(new QAction(tr("Snap to Y grid"), p))
{
    visibleAction->setCheckable(true);
    visibleAction->setChecked(false);
    connect(visibleAction, SIGNAL(toggled(bool)),
            p, SIGNAL(visibleChanged(bool)));

    xEnabledAction->setCheckable(true);
    xEnabledAction->setChecked(false);
    connect(xEnabledAction, SIGNAL(toggled(bool)),
            p, SIGNAL(xEnabledChanged(bool)));

    yEnabledAction->setCheckable(true);
    yEnabledAction->setChecked(false);
    connect(yEnabledAction, SIGNAL(toggled(bool)),
            p, SIGNAL(yEnabledChanged(bool)));
}


/*!
 * \class sfd::gui::form_editor::FormEditor::Creator
 */

FormEditor::Creator::Creator(Grid *grid, 
    core::control_loader::ControlPluginLoader *controlPluginLoader)
    : p_grid(grid)
    , p_controlPluginLoader(controlPluginLoader)
{
    Q_ASSERT(grid != 0);
    Q_ASSERT(controlPluginLoader != 0);
}

Editor *FormEditor::Creator::tryToCreate(core::project::Document *document) 
    const
{
    using core::form::Form;

    Form *const formDocument = qobject_cast<Form*>(document);
    if (formDocument == 0) {
        return 0;
    }

    Editor *const editor = new FormEditor(formDocument, p_grid, 
            p_controlPluginLoader);

    if (CONFIG__DISABLE_FORM_EDITOR_MAGNIFIER) {
        return editor;
    }

    return new GraphicsViewMagnifier(editor);
}
