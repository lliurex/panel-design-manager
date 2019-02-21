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

#include "setupwidget.hpp"
#include "n4dcontext.hpp"


#include <QProcess>

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <ctime>

using namespace lliurex::pdm;
using namespace std;

SetupWidget::SetupWidget() : QWidget()
{
    QVBoxLayout* vbox = new QVBoxLayout();
    chkStatus = new QCheckBox("Enable custom design"); 
    
    vbox->addWidget(chkStatus);
    
    QHBoxLayout* hbox = new QHBoxLayout();
    QPushButton* btn = new QPushButton("Copy");
    btn->setMaximumWidth(128);
    btn->setIcon(QIcon::fromTheme("edit-copy"));
    
    connect(btn,SIGNAL (released()), this, SLOT (onCopyClicked()));
    lblDate = new QLabel("No config");
    
    hbox->addWidget(btn);
    hbox->addWidget(lblDate);
    vbox->addLayout(hbox);
    
    chkReplicate = new QCheckBox("Replicate on clients");
    vbox->addWidget(chkReplicate);
    
    hbox = new QHBoxLayout();
    btn = new QPushButton("Apply");
    connect(btn,SIGNAL (released()), this, SLOT (onApplyClicked()));
    btn->setEnabled(false);
    QPushButton* btn2 = new QPushButton("Cancel");
    connect(btn2,SIGNAL (released()), this, SLOT (onCancelClicked()));
    
    hbox->addWidget(btn);
    hbox->addWidget(btn2);
    
    vbox->addLayout(hbox);
    vbox->setAlignment(hbox,Qt::AlignRight);
    
    setMinimumWidth(600);
    //setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    
    setLayout(vbox);
}

SetupWidget::~SetupWidget()
{
}

void SetupWidget::onCopyClicked()
{
    
    QStringList files = {"kwinrc","kdeglobals","plasma-org.kde.plasma.desktop-appletsrc","plasmashellrc","kcminputrc"};
    N4DContext* n4d=N4DContext::context();
    
    clog<<"copy local setup..."<<endl;
    
    QProcess child;
    child.setProgram("kconfig-dump");
    
    for (int n=0;n<files.size();n++) {
        clog<<"reading "<<files.at(n).toLocal8Bit().constData()<<endl;
        child.setArguments({files.at(n)});
        
        child.start();
        child.waitForFinished();
        
        n4d->kconfig[files.at(n)]=child.readAllStandardOutput();
    }
    
}

void SetupWidget::onApplyClicked()
{
    clog<<"pushing..."<<endl;
    
    N4DContext* n4d=N4DContext::context();
    
    char buff[32];
    time_t t=time(nullptr);
    strftime(buff, 32, "%Y-%m-%d %H:%M:%S", localtime(&t));
    
    clog<<buff<<endl;
    n4d->date=string(buff);
    
    n4d->push();
    
}

void SetupWidget::onCancelClicked()
{
    exit(0);
}

void SetupWidget::pull()
{
    N4DContext* n4d=N4DContext::context();
    
    chkStatus->setChecked(n4d->status);
    chkReplicate->setChecked(n4d->replicate);
    
    
    lblDate->setText(QString(n4d->date.c_str()));
}