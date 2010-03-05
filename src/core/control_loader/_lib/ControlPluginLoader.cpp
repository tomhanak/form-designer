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

#include "ControlPluginLoader_p.hpp"

#include "../ControlPlugin.hpp"

using namespace sfd::core::control_loader;

ControlPluginLoader::ControlPluginLoader(QObject *parent)
    : QObject(parent)
    , p(new Private(this))
{
}

void ControlPluginLoader::fakeLoad(ControlPlugin *plugin)
{
    Q_ASSERT(plugin != 0);
    Q_ASSERT(!p->plugins.contains(plugin->pluginName()));

    p->plugins.insert(plugin->pluginName(), plugin);
}


ControlPluginLoader::Private::Private(ControlPluginLoader *publ)
    : p(publ)
{
}
