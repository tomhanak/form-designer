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

#include "LanguageNameDialog.hpp"

#include <QtCore/QStringList>
#include <QtGui/QPushButton>

#include "core/transl_table/UnifiedTranslTable.hpp"

#include "ui_LanguageNameDialog.hpp"

using namespace sfd::gui::transl_table;

LanguageNameDialog::LanguageNameDialog(
        core::transl_table::UnifiedTranslTable *table, const QString &language, 
        QWidget *parent)
    : QDialog(parent)
    , p_table(table)
    , p_ui(new Ui::LanguageNameDialog)
{
    Q_ASSERT(table != 0);
    p_ui->setupUi(this);
    p_ui->language->setText(language);
    p_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(p_ui->language, SIGNAL(textChanged(QString)),
            this, SLOT(validate()));
    connect(p_ui->language, SIGNAL(returnPressed()),
            this, SLOT(onReturnPressed()));
}

QString LanguageNameDialog::language() const
{
    return p_ui->language->text();
}

void LanguageNameDialog::validate()
{
    bool isInvalid = p_table->languages().contains(language())
        || language().isEmpty();
    p_ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(isInvalid);
}

void LanguageNameDialog::onReturnPressed()
{
    validate();
    if (p_ui->buttonBox->button(QDialogButtonBox::Ok)->isEnabled()) {
        this->accept();
    }
}
