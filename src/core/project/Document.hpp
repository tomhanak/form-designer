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
#include <QUndoStack>

namespace sfd {
namespace core {
namespace project {

class Document : public QObject
{
    Q_OBJECT

public:
    //! For now, int is enought; later it will be extended most likely
    typedef int Type;

public:
    Document(QObject *parent = 0);
    ~Document();

public:
    //! Call registerType() in your implementation to get a Type id
    virtual Type type() const = 0;
    QUndoStack* undoStack();
    void setName(QString name);
    QString name() const;

public:
    static QSet<Type> registeredTypes();

protected:
    static Type registerType();

private:
    class Private;
    p_ptr<Private> p;
};

}}} // namespace

#if CONFIG__PIMPL_RELEASE_MODE
# include "_lib/Document_p.hpp"
#endif
