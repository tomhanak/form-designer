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

/*! \file
 * \brief This is an automatically included header file.
 *
 * This file should mostly contain imports of things defined somewhere else.  
 * Usual piece of code put in this file is supposed to looks like
 *
 * \code
#include "path/to/some/header/file.hpp"
namespace sfd {
    using some::symbol;
    using some::other_symbol;
}
 * \endcode
 *
 * \author Martin Kampas <martin.kampas@tieto.com>, 01/2010
 */

#pragma once

//////////////////////////////////////////////////////////////////////////////
//                                 IMPORTS

#include "tools/PIMPL.hpp"
namespace sfd {
    using tools::p_ptr;
}

//////////////////////////////////////////////////////////////////////////////
//                               OTHER STUFF

//! Redefinition to make use of GCC's __PRETTY_FUNCTION__
#define __func__ __PRETTY_FUNCTION__

//! Use it to mark deprecated symbols
#define SFD_DEPRECATED __attribute__((deprecated))

#include <QtCore/QDebug>
