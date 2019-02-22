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

#ifndef PDM_N4DCONTEXT
#define PDM_N4DCONTEXT

#include <xmlrpc-c/client_simple.hpp>
#include <QByteArray>
#include <QString>

#include <string>

namespace lliurex
{
    namespace pdm
    {
        enum class N4DError 
        {
            Connection,
            BadResponse,
            MessageFormat
        };
        
        class N4DContext
        {
            private:
            
            std::string url;
            int port;
            std::string user;
            std::string pass;
            
            xmlrpc_c::value execute(std::string method,xmlrpc_c::paramList extraParams);
            
            bool valid;
            
            public:
            
            bool status;
            bool replicate;
            std::string date;
            std::map<std::string,std::string> kconfig;
            
            N4DContext();
            
            
            static N4DContext* context();
            
            bool isValid();
            
            void setUrl(std::string url,int port);
            void setLogin(std::string user,std::string pass);
            
            void pull();
            void push();
        };
    }
}

#endif