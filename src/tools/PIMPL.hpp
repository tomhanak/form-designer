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
 * \author Martin Kampas <martin.kampas@tieto.com>, 01/2010
 */

#pragma once

#include <cassert>

#if !defined(CONFIG__PIMPL_RELEASE_MODE)
# define CONFIG__PIMPL_RELEASE_MODE (false)
#endif

#if CONFIG__PIMPL_RELEASE_MODE
# define PIMPL_INLINE inline
#else
# define PIMPL_INLINE /* nothing */
#endif

#define PIMPL_DEFINE_DESTRUCTOR(CLASS) \
    PIMPL_INLINE CLASS::~CLASS() { delete p.detach(); }

#define PIMPL_DEFINE_DESTRUCTOR2(CLASS, NESTED_CLASS) \
    PIMPL_INLINE CLASS::NESTED_CLASS::~NESTED_CLASS() { delete p.detach(); }

#define PIMPL_DEFINE_GETTER(TYPE, CLASS, MEMBER) \
    PIMPL_INLINE TYPE CLASS::MEMBER() const { return p->MEMBER; }

namespace sfd {
namespace tools {

template <class T>
class p_ptr
{
public:
    p_ptr(T *p)
        : p(p)
    {
        assert(p != 0);
    }

public:
    T *operator->()
    {
        return p;
    }

    const T *operator->() const
    {
        return p;
    }

    operator T *()
    {
        return p;
    }

    operator const T *() const
    {
        return p;
    }

    T *detach()
    {
        assert(p != 0);

        T *const p = this->p;
        this->p = 0;

        return p;
    }

private:
    T *p;
};

}} //namespace
