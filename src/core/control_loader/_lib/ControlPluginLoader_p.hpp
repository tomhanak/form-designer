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

#include "../ControlPluginLoader.hpp"

namespace sfd {
namespace core {
namespace control_loader {

class ControlPluginLoader::Private
{
    friend class ControlPluginLoader;
    p_ptr<ControlPluginLoader> p;

private:
    Private(ControlPluginLoader *publ);

private:
    QMap<QString, ControlPlugin*> plugins;
};

PIMPL_DEFINE_DESTRUCTOR(ControlPluginLoader)

PIMPL_INLINE QList<ControlPlugin*> ControlPluginLoader::plugins() const
{
    return p->plugins.values();
}

PIMPL_INLINE ControlPlugin* ControlPluginLoader::plugin(const QString 
        &pluginName) const
{
    return p->plugins.value(pluginName);
}

}}} // namespace
