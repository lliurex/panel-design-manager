/*
    panel design mananger

    Copyright (C) 2019  Enrique Medina Gremaldos <quiqueiii@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "locale.hpp"
#include "loginwidget.hpp"
#include "n4dcontext.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

#include <iostream>

using namespace lliurex::pdm;
using namespace std;

LoginWidget::LoginWidget() : QWidget()
{
    QVBoxLayout* vbox = new QVBoxLayout();
    
    QHBoxLayout* hbox = new QHBoxLayout();
    
    QLabel* label = new QLabel(T("user"));
    QLabel* icon = new QLabel();
    icon->setPixmap(QIcon::fromTheme("user-online").pixmap(16,16));
    editUser = new QLineEdit();
    
    hbox->setContentsMargins(0,0,200,0);
    hbox->addWidget(icon);
    hbox->addWidget(label);
    hbox->addWidget(editUser);
    hbox->setAlignment(icon,Qt::AlignRight);
    hbox->setAlignment(label,Qt::AlignRight);
    hbox->setAlignment(editUser,Qt::AlignRight);
    
    vbox->addLayout(hbox);
    
    hbox = new QHBoxLayout();
    label = new QLabel(T("password"));
    icon = new QLabel();
    icon->setPixmap(QIcon::fromTheme("lock").pixmap(16,16));
    editPass = new QLineEdit();
    editPass->setEchoMode(QLineEdit::Password);
    
    hbox->setContentsMargins(0,0,200,0);
    hbox->addWidget(icon);
    hbox->addWidget(label);
    hbox->addWidget(editPass);
    hbox->setAlignment(icon,Qt::AlignRight);
    hbox->setAlignment(label,Qt::AlignRight);
    hbox->setAlignment(editPass,Qt::AlignRight);
    vbox->addLayout(hbox);

    hbox = new QHBoxLayout();
    label = new QLabel(T("server"));
    icon = new QLabel();
    icon->setPixmap(QIcon::fromTheme("folder-cloud").pixmap(16,16));
    editServer = new QLineEdit("server");
    
    hbox->setContentsMargins(0,0,200,0);
    hbox->addWidget(icon);
    hbox->addWidget(label);
    hbox->addWidget(editServer);
    hbox->setAlignment(icon,Qt::AlignRight);
    hbox->setAlignment(label,Qt::AlignRight);
    hbox->setAlignment(editServer,Qt::AlignRight);
    vbox->addLayout(hbox);
    
    QPushButton* button = new QPushButton(T("Login"));
    button->setMaximumWidth(128);
    connect(button, SIGNAL (released()), this, SLOT (onLoginClicked()));
    
    vbox->addWidget(button);
    vbox->setAlignment(button,Qt::AlignHCenter);
    
    setMinimumWidth(600);
    setLayout(vbox);
}

LoginWidget::~LoginWidget()
{
}

void LoginWidget::onLoginClicked()
{
    string user = editUser->text().toStdString();
    string pass = editPass->text().toStdString();
    string server = editServer->text().toStdString();
    
    N4DContext* n4d=N4DContext::context();
    n4d->setUrl(server,9779);
    n4d->setLogin(user,pass);
    
    try {
        n4d->pull();
        
        emit connected();
    }
    catch(N4DError e) {
        
        QString emsg;
        
        switch(e) {
            
            case N4DError::Connection:
                emsg=T("Connection error");
            break;
            
            case N4DError::BadResponse:
                emsg=T("Bad response");
            break;
            
            case N4DError::MessageFormat:
                emsg=T("Bad message format");
            break;
            
            case N4DError::User:
                emsg=T("Login failed");
            break;
            
            case N4DError::Group:
                emsg=T("User not allowed");
            break;
            
            case N4DError::Get:
                emsg=T("Error reading settings");
            break;
            
            case N4DError::Set:
                emsg=T("Error writting settings");
            break;
        }
        
        QMessageBox msg;
        msg.setText(emsg);
        msg.exec();
        
    }
    
}