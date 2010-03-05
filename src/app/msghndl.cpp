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

#include "msghndl.hpp"

#include <QString>
#include <QStringList>
#include <QRegExp>

#include <iostream>
#include <cstdlib>

namespace sfd {

void coloredMsgHandler(QtMsgType type, const char *msg)
{
    QString prefix;
    int prefixRendCode = 0;

    switch (type)
    {
    case QtDebugMsg:
        prefix = "Debug   ";
        prefixRendCode = 32;
        break;
    case QtWarningMsg:
        prefix = "Warning ";
        prefixRendCode = 36;
        break;
    case QtCriticalMsg:
        prefix = "Critical";
        prefixRendCode = 35;
        break;
    case QtFatalMsg:
        prefix = "Fatal   ";
        prefixRendCode = 31;
        break;
    }

    QString message(msg);

    QStringList splitted(message.split(QRegExp("\\)( const)?:")));

    if (splitted.count() > 1)
    {
        QString funcName(splitted.first());
        splitted.removeFirst();
        QString msgText(splitted.join("):"));

        message = "\x1B[1m" + funcName + ")\x1B[0m: " + msgText;
    }

    QString prefixedMessage = QString("\x1B[%1m%2\x1B[0m: %3")
            .arg(prefixRendCode).arg(prefix).arg(message);

    std::cerr << qPrintable(prefixedMessage) <<std::endl;

    if (type == QtFatalMsg)
        std::abort();
}

} //namespace sfd
