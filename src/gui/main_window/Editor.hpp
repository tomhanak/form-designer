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

#include <QtGui/QWidget>

#include "tools/AFromBCreator.hpp"

namespace sfd {

namespace core {
namespace project {
    class Document;
}}

namespace gui {
namespace main_window {

typedef tools::AFromBCreator<class Editor, core::project::Document*> 
    EditorFromDocumentCreator;

class Editor : public QWidget
{
    Q_OBJECT

public:
    class Magnifier;

public:
    Editor(QWidget *parent = 0);
    ~Editor();

public:
    virtual core::project::Document *document() const = 0;
    //! Returns Magnifier object \e if the editor provides such a functionality
    virtual Magnifier *magnifier() const;

private:
    class Private;
    p_ptr<Private> p;
};

class Editor::Magnifier : public QObject
{
    Q_OBJECT

public:
    enum
    {
        MIN_SCALE = 10,   //!< Limit scaling out to 10%
        MAX_SCALE = 1000, //!< Limit scaling in to 1000%
    };

public:
    Magnifier(QObject *parent = 0);
    ~Magnifier();

public:
    int scale() const;
    QAction *scaleInAction() const;
    QAction *scaleOutAction() const;

public Q_SLOTS:
    void setScale(int scale);
    void scaleIn();
    void scaleOut();

Q_SIGNALS:
    void scaleChanged(int scale);

private:
    class Private;
    p_ptr<Private> p;
};

}}} // namespace

#if CONFIG__PIMPL_RELEASE_MODE
# include "_lib/Editor_p.hpp"
#endif
