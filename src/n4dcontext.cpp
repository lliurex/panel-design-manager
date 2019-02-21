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

#include "n4dcontext.hpp"

#include <iostream>
#include <vector>
#include <map>
#include <stdexcept>

using namespace lliurex::pdm;
using namespace std;

//static global object
N4DContext ctx;


N4DContext* N4DContext::context()
{
     return &ctx;
}

N4DContext::N4DContext()
{
    valid=false;
}

bool N4DContext::isValid()
{
    return valid;
}

xmlrpc_c::value N4DContext::execute(string method,xmlrpc_c::paramList extraParams)
{
    string fullUrl = "https://"+url+":"+std::to_string(port);
    
    xmlrpc_c::clientXmlTransport_curl transport(
                xmlrpc_c::clientXmlTransport_curl::constrOpt()
                .no_ssl_verifyhost(true)
                .no_ssl_verifypeer(true)
    );
    xmlrpc_c::client_xml n4d(&transport);
    
    xmlrpc_c::paramList params;
    vector<xmlrpc_c::value> login;
    
    login.push_back(xmlrpc_c::value_string(user));
    login.push_back(xmlrpc_c::value_string(pass));
    
    params.add(xmlrpc_c::value_array(login));
    params.add(xmlrpc_c::value_string("VariablesManager"));
    params.add(xmlrpc_c::value_string("PANELDESIGNMANAGER"));
    
    for (int n=0;n<extraParams.size();n++) {
        params.add(extraParams[n]);
    }
    
    xmlrpc_c::rpcPtr rpcp(method, params);
    xmlrpc_c::carriageParm_curl0 carriage(fullUrl);
    rpcp->call(&n4d, &carriage);
    
    xmlrpc_c::value result = rpcp->getResult();
    
    return result;
}

void N4DContext::setUrl(string url,int port)
{
    this->url=url;
    this->port=port;
}

void N4DContext::setLogin(string user,string pass)
{
    this->user=user;
    this->pass=pass;
}

void N4DContext::pull()
{
    xmlrpc_c::paramList params;
    xmlrpc_c::value result = execute("get_variable",params);
    
    if (!result.isInstantiated()) {
        throw runtime_error("bad response");
    }
    
    if (result.type()!=xmlrpc_c::value::TYPE_STRUCT) {
        throw runtime_error("bad format (expecting struct)");
    }
    
    map<string,xmlrpc_c::value> settings = xmlrpc_c::value_struct(result);
    
    //sanity check
    if (settings.find("status")==settings.end()) {
        throw runtime_error("missing status property");
    }
    if (settings.find("replicate")==settings.end()) {
        throw runtime_error("missing replicate property");
    }
    if (settings.find("date")==settings.end()) {
        throw runtime_error("missing date property");
    }
    
    xmlrpc_c::value tmp = settings["status"];
    if (tmp.type()!=xmlrpc_c::value::TYPE_BOOLEAN) {
        throw runtime_error("bad status format");
    }
    this->status=xmlrpc_c::value_boolean(tmp).cvalue();
    
    xmlrpc_c::value tmp = settings["replicate"];
    if (tmp.type()!=xmlrpc_c::value::TYPE_BOOLEAN) {
        throw runtime_error("bad replicate format");
    }
    this->replicate=xmlrpc_c::value_boolean(tmp).cvalue();
    
    xmlrpc_c::value tmp = settings["date"];
    if (tmp.type()!=xmlrpc_c::value::TYPE_STRING) {
        throw runtime_error("bad date format");
    }
    this->date=xmlrpc_c::value_string(tmp).cvalue();
    
    valid=true;
}

void N4DContext::push()
{
    xmlrpc_c::paramList params;
    
    map<string,xmlrpc_c::value> variable;
    
    variable["status"]=xmlrpc_c::value_boolean(this->status);
    variable["date"]=xmlrpc_c::value_string(this->date);
    variable["replicate"]=xmlrpc_c::value_boolean(this->replicate);
    variable["kconfig"]=xmlrpc_c::value_struct(this->kconfig);
    
    params.add(xmlrpc_c::value_struct(variable));
    
    xmlrpc_c::value result = execute("set_variable",params);
    
    if (!result.isInstantiated()) {
        throw runtime_error("bad response");
    }
    
    if (result.type()==xmlrpc_c::value::TYPE_STRUCT) {
        map<string,xmlrpc_c::value> res = xmlrpc_c::value_struct(result).cvalue();
        
        for (auto a:res) {
            clog<<"* "<<a.first<<":";
            
            if (a.second.type()==xmlrpc_c::value::TYPE_STRING) {
                clog<<xmlrpc_c::value_string(a.second).cvalue();
            }
            
            clog<<endl;
        }
    }
}