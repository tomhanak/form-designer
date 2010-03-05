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

#include "../Form.hpp"

#include <QtCore/QPointer>

namespace sfd {
namespace core {
namespace form {

class Form::Private
{
    friend class Form;
    p_ptr<Form> p;

private:
    Private(Form *publ);

private:
    QList<QWidget*> controls;
    const QPointer<QWidget> widget;
    const QPointer<Selection> selection;
};

PIMPL_DEFINE_DESTRUCTOR(Form)
PIMPL_DEFINE_GETTER(QList<QWidget*>, Form, controls)
PIMPL_DEFINE_GETTER(QWidget*, Form, widget)
PIMPL_DEFINE_GETTER(Form::Selection*, Form, selection)

/*!
 * \class Form::Selection
 */

PIMPL_INLINE Form::Selection::Selection(QObject *parent)
    : QObject(parent)
{
}

PIMPL_INLINE QWidget *Form::Selection::leader() const
{
    return p_leader;
}

PIMPL_INLINE const QSet<QWidget*> &Form::Selection::toConstSet() const
{
    return *this;
}

}}} // namespace
