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

#include "GeometryEditor.hpp"

#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

#include "core/form/Form.hpp"
#include "core/project/Document.hpp"
#include "undo/property_editor/EditProperty.hpp"

using namespace sfd::gui::form_editor;

//! Draws border handlers and allowes to change size of a control
class GeometryEditor::ControlManipulator : public QWidget
{
    Q_OBJECT

public:
    ControlManipulator(QWidget *control, GeometryEditor *parent);

public:
    GeometryEditor *parentGeometryEditor() const;

public:
    //! \reimpl QObject
    //@{
    virtual bool eventFilter(QObject *watched, QEvent *event);
    //@}

public:
    void initOldGeometry();
    inline QRect oldGeometry();
    //! add new geometry to undo stack, and reset old value to QRect()
    void createUndoAction(const QRect& oldGeometry);

protected:
    //! \reimpl QWidget
    //@{
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    //@}

private:
    //! Tells how to adjust control's geometry while dragging its handle
    struct ResizeMask : private QRect
    {
        enum Edge
        {
            None   = 0x00,
            Left   = 0x01,
            Right  = 0x02,
            Top    = 0x04,
            Bottom = 0x08,
        };

        //! Pass a bitmask of Edge values to set it
        void set(int edgesToBeAdjusted);

        using QRect::x;
        using QRect::y;
        using QRect::width;
        using QRect::height;
        using QRect::isNull; //!< Use it to ask if is set
    };

private:
    static const int HANDLE_SIZE = 4;

private:
    inline void updatePos();
    inline void updateSize();
    inline void updateCursor(int x, int y);

    void updateCachedPath();

private:
    //! Maps values to handles; returns value for handle at [x, y] or XX
    template <typename T,
             T TL, T TC, T TR,
             T ML, T XX, T MR,
             T BL, T BC, T BR>
    T matchHandle(int x, int y);

private:
    QPointer<QWidget> p_control;
    QPainterPath p_cachedPath;
    ResizeMask p_resizeMask;
    QPoint p_lastCursorParentPos;
    QRect p_oldGeometry;
};

/*!
 * \param form Despite the FormEditor::form() getter, it has to be passed as an 
 * extra argument -- the FormEditor instance will have not been fully 
 * instantiated yet.
 * \param formEditor
 */
GeometryEditor::GeometryEditor(const core::form::Form &form, FormEditor 
        *formEditor)
    : QFrame(formEditor)
    , p_formEditor(formEditor)
{
    Q_ASSERT(formEditor != 0);

    connect(form.selection(), SIGNAL(included(QWidget*)),
            this, SLOT(startEditing(QWidget*)));
    connect(form.selection(), SIGNAL(aboutToExclude(QWidget*)),
            this, SLOT(stopEditing(QWidget*)));
    connect(form.selection(), SIGNAL(leaderChanged(QWidget*)),
            this, SLOT(repaint()));
}

bool GeometryEditor::eventFilter(QObject *watched, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonRelease: {
        QMouseEvent* me = dynamic_cast<QMouseEvent*> (event);
        Q_ASSERT(me != 0);
        mouseReleaseHandler(me);
        break;
        }

    default:
        ;
    }

    return QWidget::eventFilter(watched, event);
}

void GeometryEditor::startEditing(QWidget *widget)
{
    Q_ASSERT(widget != 0);
    Q_ASSERT(!p_controlManipulators.contains(widget));

    ControlManipulator *const co = new ControlManipulator(widget, this);
    p_controlManipulators[widget] = co;
    co->installEventFilter(this);
    co->show();
}

void GeometryEditor::stopEditing(QWidget *widget)
{
    Q_ASSERT(widget != 0);
    Q_ASSERT(p_controlManipulators.contains(widget));

    delete p_controlManipulators.take(widget);
}

void GeometryEditor::mouseMoveEvent(QMouseEvent *event)
{
    QFrame::mouseMoveEvent(event);

    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }

    if (!p_formEditor->form()->selection()->isEmpty()) {
        const QPoint diff = event->pos() - p_lastCursorPos;
        p_lastCursorPos = event->pos();

        Q_FOREACH(QWidget *w, p_formEditor->form()->selection()->toConstSet()) {
            w->move(w->pos() + diff);
        }

        event->accept();
    }
}

void GeometryEditor::mousePressEvent(QMouseEvent *event)
{
    QFrame::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        p_lastCursorPos = event->pos();
        Q_FOREACH(QWidget *w, p_formEditor->form()->selection()->toConstSet()) {
            p_controlManipulators.value(w)->initOldGeometry();
        }
        event->ignore(); // let the FormEditor to handle selection
    }
}

void GeometryEditor::mouseReleaseHandler(QMouseEvent *event)
{
    Q_UNUSED(event);

    Q_FOREACH(QWidget *w, p_formEditor->form()->selection()->toConstSet()) {
        ControlManipulator* co = p_controlManipulators.value(w);
        co->createUndoAction(co->oldGeometry());
    }
}


/*!
 * \class GeometryEditor::ControlManipulator
 */

GeometryEditor::ControlManipulator::ControlManipulator(QWidget *control, 
        GeometryEditor *parent)
    : QWidget(parent)
    , p_control(control)
{
    Q_ASSERT(control != 0);
    Q_ASSERT(parent != 0);

    setMouseTracking(true);

    control->installEventFilter(this);

    updatePos();
    updateSize();
    p_oldGeometry = QRect();
}

inline GeometryEditor 
    *GeometryEditor::ControlManipulator::parentGeometryEditor() const
{
    return qobject_cast<GeometryEditor*>(parent());
}

bool GeometryEditor::ControlManipulator::eventFilter(QObject *watched, QEvent *event)
{
    Q_ASSERT(watched == p_control);

    switch (event->type())
    {
    case QEvent::Move:
        updatePos();
        break;

    case QEvent::Resize:
        updateSize();
        break;

    case QEvent::FocusIn:
    case QEvent::FocusOut:
        repaint();
        break;

    default:
        ;
    }

    return QWidget::eventFilter(watched, event);
}

void GeometryEditor::ControlManipulator::initOldGeometry()
{
    Q_ASSERT(p_control != 0);

    p_oldGeometry = p_control->geometry();
}

inline QRect GeometryEditor::ControlManipulator::oldGeometry()
{
    return p_oldGeometry;
}

void GeometryEditor::ControlManipulator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.fillPath(p_cachedPath, parentGeometryEditor()-> 
            formEditor()->form()->selection()->leader() == p_control ?  
            palette().highlight() : palette().window());

    painter.setPen(palette().highlight().color());
    painter.drawPath(p_cachedPath);
}

void GeometryEditor::ControlManipulator::mousePressEvent(QMouseEvent *event)
{
    typedef ResizeMask RM;

    QWidget::mousePressEvent(event);

    const int edgesToBeAdjusted = matchHandle<int,
        RM::Top    | RM::Left,  RM::Top   ,  RM::Top    | RM::Right,
                     RM::Left,  RM::None  ,               RM::Right,
        RM::Bottom | RM::Left,  RM::Bottom,  RM::Bottom | RM::Right
            >(event->x(), event->y());

    if (p_resizeMask.set(edgesToBeAdjusted), !p_resizeMask.isNull()) {

        p_lastCursorParentPos = mapToParent(event->pos());

        // When snapping to grid, it is necessary to adjust 
        // p_lastCursorParentPos to the center of the handle.

        FormEditor::Grid *const grid = 
            parentGeometryEditor()->formEditor()->grid();
        const QPoint controlTopLeft(
                parentWidget()->mapFromGlobal(
                    p_control->parentWidget()->mapToGlobal(
                        p_control->geometry().topLeft())));
        const QPoint controlBottomRight(
                parentWidget()->mapFromGlobal(
                    p_control->parentWidget()->mapToGlobal(
                        p_control->geometry().bottomRight())));

        if (grid->isXEnabled()) {
            if (edgesToBeAdjusted & RM::Left) {
                p_lastCursorParentPos.setX(controlTopLeft.x());
            }
            else if (edgesToBeAdjusted & RM::Right) {
                p_lastCursorParentPos.setX(controlBottomRight.x());
            }
        }

        if (grid->isYEnabled()) {
            if (edgesToBeAdjusted & RM::Top) {
                p_lastCursorParentPos.setY(controlTopLeft.y());
            }
            else if (edgesToBeAdjusted & RM::Bottom) {
                p_lastCursorParentPos.setY(controlBottomRight.y());
            }
        }

        event->accept();
    }
}

void GeometryEditor::ControlManipulator::mouseReleaseEvent(QMouseEvent *event)
{
    if (!p_resizeMask.isNull()) {
        p_resizeMask.set(ResizeMask::None);
        event->accept();
    }
}

void GeometryEditor::ControlManipulator::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    if (!p_resizeMask.isNull()) {

        const QPoint gridAlignedParentPos = 
            parentGeometryEditor()->formEditor()->grid()-> 
            align(mapToParent(event->pos()));

        const QPoint diff = gridAlignedParentPos - p_lastCursorParentPos;

        if (diff != QPoint()) {
            p_control->setGeometry(
                    p_control->x()      +      p_resizeMask.x() * diff.x(),
                    p_control->y()      +      p_resizeMask.y() * diff.y(),
                    p_control->width()  +  p_resizeMask.width() * diff.x(),
                    p_control->height() + p_resizeMask.height() * diff.y());
            p_lastCursorParentPos += diff;
        }

        event->accept();
    }
    else if (event->buttons() == Qt::NoButton) {
        updateCursor(event->x(), event->y());
    }
}

inline void GeometryEditor::ControlManipulator::updatePos()
{
    move(parentWidget()->mapFromGlobal( p_control->parentWidget()->mapToGlobal( 
                    p_control->pos())) - QPoint(HANDLE_SIZE/2, HANDLE_SIZE/2));
}

inline void GeometryEditor::ControlManipulator::updateSize()
{
    resize(p_control->size() + QSize(HANDLE_SIZE, HANDLE_SIZE));
    updateCachedPath();
}

void GeometryEditor::ControlManipulator::updateCursor(int x, int y)
{
    const Qt::CursorShape NORMAL_CURSOR = Qt::ArrowCursor;

    const Qt::CursorShape shape = matchHandle<Qt::CursorShape,
        Qt::SizeFDiagCursor, Qt::SizeVerCursor, Qt::SizeBDiagCursor,
        Qt::SizeHorCursor,     NORMAL_CURSOR,   Qt::SizeHorCursor,
        Qt::SizeBDiagCursor, Qt::SizeVerCursor, Qt::SizeFDiagCursor
            >(x, y);

    if (shape != NORMAL_CURSOR) {
        setCursor(shape);
    }
    else {
        unsetCursor();
    }
}

void GeometryEditor::ControlManipulator::updateCachedPath()
{
    const int HS = HANDLE_SIZE;

    p_cachedPath = QPainterPath();

    p_cachedPath.addRect(               0,                 0, HS, HS);
    p_cachedPath.addRect(width()/2 - HS/2,                 0, HS, HS);
    p_cachedPath.addRect(width()   - HS-1,                 0, HS, HS);

    p_cachedPath.addRect(               0, height()/2 - HS/2, HS, HS);
    p_cachedPath.addRect(width()   - HS-1, height()/2 - HS/2, HS, HS);

    p_cachedPath.addRect(               0, height()   - HS-1, HS, HS);
    p_cachedPath.addRect(width()/2 - HS/2, height()   - HS-1, HS, HS);
    p_cachedPath.addRect(width()   - HS-1, height()   - HS-1, HS, HS);
}

void GeometryEditor::ControlManipulator::createUndoAction(const QRect& oldGeometry)
{
    using sfd::core::project::Document;
    using sfd::undo::property_editor::EditProperty;

    if (oldGeometry == p_control->geometry())
        return;

    Document* doc = parentGeometryEditor()->formEditor()->document();
    Q_ASSERT(doc != 0);

    EditProperty* undo = new EditProperty(p_control, "geometry", oldGeometry,
        p_control->geometry());

    doc->undoStack()->push(undo);
    p_oldGeometry = QRect();
}

template <typename T,
    T TL, T TC, T TR,
    T ML, T XX, T MR,
    T BL, T BC, T BR>
T GeometryEditor::ControlManipulator::matchHandle(int x, int y)
{
    Q_ASSERT(x >= 0 && x <= width());
    Q_ASSERT(y >= 0 && y <= height());

    const int HS = HANDLE_SIZE;

    if (y <= HS) {
        if (x <= HS) {
            return TL;
        }
        else if (x >= width()/2 - HS/2 && x <= width()/2 + HS/2) {
            return TC;
        }
        else if (x >= width() - HS) {
            return TR;
        }
    }
    else if (y >= height()/2 - HS/2 && y <= height()/2 + HS/2) {
        if (x <= HS) {
            return ML;
        }
        else if (x >= width() - HS) {
            return MR;
        }
    }
    else if (y >= height() - HS) {
        if (x <= HS) {
            return BL;
        }
        else if (x >= width()/2 - HS/2 && x <= width()/2 + HS/2) {
            return BC;
        }
        else if (x >= width() - HS) {
            return BR;
        }
    }

    return XX;
}

/*!
 * \class GeometryEditor::ControlManipulator::ResizeMask
 *
 * Consider a Control C, a ResizeMask RM and a mouse move of D = (x, y).  
 * Control's geometry will be adjusted as follows:
 *
\code
C.x      += RM.x      * D.x;
C.y      += RM.y      * D.y;
C.width  += RM.width  * D.x;
C.height += RM.height * D.y;
\endcode
 *
 */

void GeometryEditor::ControlManipulator::ResizeMask::set(int edgesToBeAdjusted)
{
    Q_ASSERT(!(edgesToBeAdjusted & Left) || !(edgesToBeAdjusted & Right));
    Q_ASSERT(!(edgesToBeAdjusted & Top) || !(edgesToBeAdjusted & Bottom));

    if (edgesToBeAdjusted & Left) {
        setX(1);
        setWidth(-1);
    }
    else if (edgesToBeAdjusted & Right) {
        setWidth(1);
    }
    else {
        setX(0);
        setWidth(0);
    }

    if (edgesToBeAdjusted & Top) {
        setY(1);
        setHeight(-1);
    }
    else if (edgesToBeAdjusted & Bottom) {
        setHeight(1);
    }
    else {
        setY(0);
        setHeight(0);
    }
}

#include "GeometryEditor.moc"
