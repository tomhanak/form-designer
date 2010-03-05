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

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtGui/QWidget>

#include "core/project/Document.hpp"

namespace sfd {
namespace core {
namespace form {

class Form : public project::Document
{
    Q_OBJECT

public:
    class Selection;

public:
    Form(QObject *parent = 0);
    ~Form();

public:
    void addControl(QWidget *control);
    void removeControl(QWidget *control);
    QList<QWidget*> controls() const;

    QWidget *widget() const;

public:
    Selection *selection() const;

public:
    //! \reimpl Document
    //@{
    virtual Type type() const;
    //@}

private:
    class Private;
    p_ptr<Private> p;
};

class Form::Selection : public QObject, private QSet<QWidget*>
{
    Q_OBJECT

public:
    Selection(QObject *parent = 0);

public:
    void include(QWidget *widget);
    void exclude(QWidget *widget);
    void setLeader(QWidget *widget);
    QWidget *leader() const;
    void clear();

    using QSet<QWidget*>::contains;
    using QSet<QWidget*>::isEmpty;

    using QSet<QWidget*>::constBegin;
    using QSet<QWidget*>::constEnd;

public:
    //! To be used with Q_FOREACH macro
    const QSet<QWidget*> &toConstSet() const;

Q_SIGNALS:
    void included(QWidget *widget);
    void aboutToExclude(QWidget *widget);
    void leaderChanged(QWidget *leader);

private:
    QPointer<QWidget> p_leader;
};

}}} // namespace

#if CONFIG__PIMPL_RELEASE_MODE
# include "_lib/Form_p.hpp"
#endif
