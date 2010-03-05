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

#include "SettingsPage.hpp"

#include <QtGui/QAbstractButton>

#include "ui_SettingsPage.hpp"

using namespace sfd::gui::main_window;

SettingsPage::SettingsPage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsPage)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    
    connect(this, SIGNAL(accepted()),
            this, SLOT(apply()));

    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)),
            this, SLOT(onButtonClicked(QAbstractButton*)));
}

void SettingsPage::apply()
{
    //! \todo Apply application settings
}

void SettingsPage::onButtonClicked(QAbstractButton *button)
{
    if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole) {
        apply();
    }
}
