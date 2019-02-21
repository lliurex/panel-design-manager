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

#ifndef PDM_WINDOW
#define PDM_WINDOW

#include "loginwidget.hpp"
#include "setupwidget.hpp"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QPixmap>

namespace lliurex
{
    namespace pdm
    {
        class Window : public QMainWindow
        {
            Q_OBJECT
            
            private:
            QVBoxLayout* vbox;
            QLabel* banner;
            QPixmap bannerPixmap;
            QStackedWidget* stack;
            
            LoginWidget* loginWidget;
            SetupWidget* setupWidget;
            
            private slots:
            void onConnected();
            
            public:
            Window (QWidget* parent=0);
            virtual ~Window();
        };
    }
}


#endif