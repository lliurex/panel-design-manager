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

#include "window.hpp"

#include <QHBoxLayout>
#include <QCheckBox>

#include <iostream>

using namespace lliurex::pdm;
using namespace std;

Window::Window(QWidget* parent) : QMainWindow(parent)
{
    
    setFixedSize(QSize(670, 400));
    
    // main vertical layout
    vbox = new QVBoxLayout();
    
    // dummy central widget (for god sake!)
    QWidget *dummy = new QWidget();
    dummy->setLayout(vbox);
    this->setCentralWidget(dummy);
    
    // banner step
    bannerPixmap = QPixmap(":/panel_banner.png");
    banner = new QLabel();
    banner->setPixmap(bannerPixmap);
    
    vbox->addWidget(banner);
    vbox->setAlignment(banner,Qt::AlignHCenter);
    
    // add stacked
    stack = new QStackedWidget();
    vbox->addWidget(stack);
    vbox->setAlignment(stack,Qt::AlignHCenter);
    
    
    // add default view
    loginWidget = new LoginWidget();
    connect(loginWidget,SIGNAL(connected()),this,SLOT(onConnected()));
    stack->addWidget(loginWidget);
    
    setupWidget = new SetupWidget();
    stack->addWidget(setupWidget);
}

Window::~Window()
{
}

void Window::onConnected()
{
    clog<<"Connected!!!"<<endl;
    stack->setCurrentWidget(setupWidget);
    setupWidget->pull();
}