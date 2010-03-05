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

#include "Form_p.hpp"

using namespace sfd::core::form;
using namespace sfd::core;

Form::Form(QObject *parent)
    : project::Document(parent)
    , p(new Private(this))
{
}

void Form::addControl(QWidget *control)
{
    Q_ASSERT(control != 0);
    Q_ASSERT(!p->controls.contains(control));

    p->controls.append(control);
    control->setParent(p->widget);
    control->show();
}

void Form::removeControl(QWidget *control)
{
    Q_ASSERT(control != 0);
    Q_ASSERT(p->controls.contains(control));

    if (p->selection->contains(control)) {
        p->selection->exclude(control);
    }

    p->controls.removeOne(control);
    control->setParent(0);
}

project::Document::Type Form::type() const
{
    static Type type = Document::registerType();
    return type;
}


Form::Private::Private(Form *publ)
    : p(publ)
    , widget(new QWidget)
    , selection(new Selection(p))
{
    widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    widget->resize(300, 300);
}

/*!
 * \class sfd::core::form::Form::Selection
 */

void Form::Selection::include(QWidget *widget)
{
    Q_ASSERT(widget != 0);
    Q_ASSERT(!contains(widget));

    insert(widget);

    Q_EMIT included(widget);

    setLeader(widget);
}

void Form::Selection::exclude(QWidget *widget)
{
    Q_ASSERT(widget != 0);
    Q_ASSERT(contains(widget));

    if (p_leader == widget) {
        if (size() > 1) {
            if (*begin() != widget) {
                setLeader(*begin());
            }
            else {
                Q_ASSERT(*(++begin()) != widget);
                setLeader(*(++begin()));
            }
        }
        else {
            setLeader(0);
        }
    }

    Q_EMIT aboutToExclude(widget);

    remove(widget);
}

void Form::Selection::setLeader(QWidget *widget)
{
    Q_ASSERT(!(widget != 0) ||
            contains(widget));

    p_leader = widget;

    Q_EMIT leaderChanged(p_leader);
}

void Form::Selection::clear()
{
    setLeader(0);

    while (!isEmpty()) {
        exclude(*begin());
    }
}
