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

namespace sfd {
namespace tools {

//! Chain of responsibility to create a concrete instance of A based on B
/*!
 * think about two abstract classes -- Document and Editor -- and a couple of 
 * pairs of its implementation -- ImageDocument + ImageEditor, TextDocument + 
 * TextEditor etc. Given an instance of Document, your mission is to create 
 * appropriate instance of Editor, based on the document's concrete type. This 
 * is where AFromBCreator (and AFromBCreatorFunction) can be usefull.
 *
\code
typedef AFromBCreator<Editor, Document*> EditorCreator;
typedef AFromBCreatorFunction<Editor, Document*> EditorCreatorFunction;

class ImageEditorCreator : public EditorCreator
{
    // ...

protected:
    virtual ImageEditor *tryToCreate(Document *document)
    {
        ImageDocument *const imageDocument = 
            qobject_cast<ImageDocument*>(document);

        return imageDocument == 0 ? 0 : ImageEditor(imageDocument);
    }
};

TextEditor *createTextEditorIfItIsATextDocument(Document *document)
{
    TextDocument *const textDocument = qobject_cast<TextDocument*>(document);

    return textDocument == 0 ? 0 : TextEditor(textDocument);
}

// ...

EditorCreator *imgEditorCreator = new ImageEditorCreator;
EditorCreator *txtEditorCreator = new 
    EditorCreatorFunction(createTextEditorIfItIsATextDocument);

EditorCreator *editorCreator = imgEditorCreator->setNext(txtEditorCreator);

Document *document = ...; // Got an image or text document? ..don't know

Editor *editor = editorCreator->create(document);
if (editor == 0) {
    errorAppropriateEditorNotRegistered();
}
\endcode
 *
 */
template<class A, class B>
class AFromBCreator
{
public:
    AFromBCreator()
        : p_next(0)
    {
    }

public:
    A* create(B b) const
    {
        A *const a = tryToCreate(b);
        return (a != 0 || p_next == 0) ? a : p_next->create(b);
    }

    AFromBCreator *setNext(AFromBCreator *next)
    {
        Q_ASSERT(p_next == 0);
        Q_ASSERT(next != 0);

        return p_next = next;
    }

protected:
    virtual A* tryToCreate(B b) const = 0;

private:
    AFromBCreator *p_next;
};

//! Doesn't require to inherit -- accepts pointer to a creator function
/*!
 * \see AFromBCreator
 */
template<class A, class B>
class AFromBCreatorFunction : AFromBCreator<A, B>
{
public:
    typedef A* (*CreatorFunction)(B b);

public:
    AFromBCreatorFunction(CreatorFunction creatorFunction)
        : p_creatorFunction(creatorFunction)
    {
        Q_ASSERT(creatorFunction != 0);
    }

protected:
    //! \reimpl AFromBCreator
    //@{
    virtual A* tryToCreate(B b) const
    {
        return (*p_creatorFunction)(b);
    }
    //@}

private:
    CreatorFunction p_creatorFunction;
};

}} // namespace

#if CONFIG__PIMPL_RELEASE_MODE
# include "_lib/Factory_p.hpp"
#endif
