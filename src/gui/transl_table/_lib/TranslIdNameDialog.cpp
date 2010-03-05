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

#include "TranslIdNameDialog.hpp"

#include <QtCore/QStringList>
#include <QtGui/QPushButton>

#include "core/transl_table/UnifiedTranslTable.hpp"

#include "ui_TranslIdNameDialog.hpp"

using namespace sfd::gui::transl_table;

TranslIdNameDialog::TranslIdNameDialog(
        core::transl_table::UnifiedTranslTable *table, const QString &translId, 
        const QString &context, QWidget *parent)
    : QDialog(parent)
    , p_table(table)
    , p_ui(new Ui::TranslIdNameDialog)
{
    Q_ASSERT(table != 0);
    p_ui->setupUi(this);
    p_ui->translId->setText(translId);
    p_ui->context->setText(context);
    p_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(p_ui->translId, SIGNAL(textChanged(QString)),
            this, SLOT(validate()));
    connect(p_ui->context, SIGNAL(textChanged(QString)),
            this, SLOT(validate()));
    connect(p_ui->translId, SIGNAL(returnPressed()),
            this, SLOT(onReturnPressed()));
    connect(p_ui->context, SIGNAL(returnPressed()),
            this, SLOT(onReturnPressed()));
}

QString TranslIdNameDialog::translId() const
{
    return p_ui->translId->text();
}

QString TranslIdNameDialog::context() const
{
    return p_ui->context->text();
}

void TranslIdNameDialog::validate()
{
    bool isInvalid = p_table->contains(translId(), context())
        || translId().isEmpty();
    p_ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(isInvalid);
}

void TranslIdNameDialog::onReturnPressed()
{
    validate();
    if (p_ui->buttonBox->button(QDialogButtonBox::Ok)->isEnabled()) {
        this->accept();
    }
}
