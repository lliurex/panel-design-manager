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
    
    
    // user validation call
    xmlrpc_c::paramList vparams;
    vparams.add(xmlrpc_c::value_string(user));
    vparams.add(xmlrpc_c::value_string(pass));
    xmlrpc_c::rpcPtr rpcp("validate_user", vparams);
    xmlrpc_c::carriageParm_curl0 carriage(fullUrl);
    rpcp->call(&n4d, &carriage);
    
    xmlrpc_c::value vres = rpcp->getResult();
    xmlrpc_c::value_array validation(vres);
    vector<xmlrpc_c::value> vData = validation.cvalue();
    
    bool userOk=xmlrpc_c::value_boolean(vData[0]).cvalue();
    bool groupOk=false;
    
    vector<xmlrpc_c::value> groups = xmlrpc_c::value_array(vData[1]).cvalue();
    
    for (int n=0;n<groups.size();n++) {
        string g=xmlrpc_c::value_string(groups[n]).cvalue();
        
        if (g=="sudo") {
            groupOk=true;
            break;
        }
    }
    
    if (!userOk) {
        throw N4DError::User;
    }
    
    if (!groupOk) {
        throw N4DError::Group;
    }
    
    vector<xmlrpc_c::value> login;
    
    login.push_back(xmlrpc_c::value_string(user));
    login.push_back(xmlrpc_c::value_string(pass));
    
    xmlrpc_c::paramList params;
    params.add(xmlrpc_c::value_array(login));
    params.add(xmlrpc_c::value_string("PanelDesignManagerServer"));
    
    for (int n=0;n<extraParams.size();n++) {
        params.add(extraParams[n]);
    }
    
    rpcp=xmlrpc_c::rpcPtr(method,params);
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
    xmlrpc_c::value result;
    
    try {
        result = execute("get_configuration",params);
    }
    catch(girerr::error& e) {
        cerr<<e.what()<<endl;
        throw N4DError::Connection;
    }
    
    if (!result.isInstantiated()) {
        throw N4DError::BadResponse;
    }
    
    if (result.type()!=xmlrpc_c::value::TYPE_STRUCT) {
        cerr<<"Expected struct"<<endl;
        throw N4DError::MessageFormat;
    }
    
    map<string,xmlrpc_c::value> message = xmlrpc_c::value_struct(result);
    if (message.find("status")==message.end()) {
        cerr<<"missing status property"<<endl;
        throw N4DError::MessageFormat;
    }
    if (message.find("msg")==message.end()) {
        cerr<<"missing msg property"<<endl;
        throw N4DError::MessageFormat;
    }
    
    xmlrpc_c::value messageStatus = message["status"];
    if (messageStatus.type()!=xmlrpc_c::value::TYPE_BOOLEAN) {
        cerr<<"bad status format"<<endl;
        throw N4DError::MessageFormat;
    }
    
    xmlrpc_c::value messageMsg = message["msg"];
    
    if (!xmlrpc_c::value_boolean(messageStatus).cvalue()) {
        cerr<<"failed to get config"<<endl;
        
        if (messageMsg.type()==xmlrpc_c::value::TYPE_STRING) {
            xmlrpc_c::value_string emsg(messageMsg);
            cerr<<emsg.cvalue()<<endl;
        }
        
        throw N4DError::Get;
    }
    
    
    map<string,xmlrpc_c::value> settings = xmlrpc_c::value_struct(messageMsg);
    
    //sanity check
    if (settings.find("status")==settings.end()) {
        cerr<<"missing status property"<<endl;
        throw N4DError::MessageFormat;
    }
    if (settings.find("replicate")==settings.end()) {
        cerr<<"missing replicate property"<<endl;
        throw N4DError::MessageFormat;
    }
    if (settings.find("date")==settings.end()) {
        cerr<<"missing date property"<<endl;
        throw N4DError::MessageFormat;
    }
    if (settings.find("kconfig")==settings.end()) {
        cerr<<"missing kconfig property"<<endl;
        throw N4DError::MessageFormat;
    }
    
    xmlrpc_c::value tmp = settings["status"];
    if (tmp.type()!=xmlrpc_c::value::TYPE_BOOLEAN) {
        cerr<<"bad status format"<<endl;
        throw N4DError::MessageFormat;
    }
    this->status=xmlrpc_c::value_boolean(tmp).cvalue();
    
    xmlrpc_c::value tmp2 = settings["replicate"];
    if (tmp2.type()!=xmlrpc_c::value::TYPE_BOOLEAN) {
        cerr<<"bad replicate format"<<endl;
        throw N4DError::MessageFormat;
    }
    this->replicate=xmlrpc_c::value_boolean(tmp2).cvalue();
    
    xmlrpc_c::value tmp3 = settings["date"];
    if (tmp3.type()!=xmlrpc_c::value::TYPE_STRING) {
        cerr<<"bad date format"<<endl;
        throw N4DError::MessageFormat;
    }
    this->date=xmlrpc_c::value_string(tmp3).cvalue();
    
    xmlrpc_c::value tmp4 = settings["kconfig"];
    if (tmp4.type()!=xmlrpc_c::value::TYPE_STRUCT) {
        cerr<<"bad kconfig format"<<endl;
        throw N4DError::MessageFormat;
    }
    
    map<string,xmlrpc_c::value> raw = xmlrpc_c::value_struct(tmp4);
    
    for (auto r : raw) {
        this->kconfig[r.first]=xmlrpc_c::value_string(r.second).cvalue();
    }
    
    
    valid=true;
}

void N4DContext::push()
{
    xmlrpc_c::paramList params;
    
    map<string,xmlrpc_c::value> variable;
    
    variable["status"]=xmlrpc_c::value_boolean(this->status);
    variable["date"]=xmlrpc_c::value_string(this->date);
    variable["replicate"]=xmlrpc_c::value_boolean(this->replicate);
    
    map<string,xmlrpc_c::value> kc;
    
    for (auto a : this->kconfig) {
        kc[a.first]=xmlrpc_c::value_string(a.second);
    }
    
    variable["kconfig"]=xmlrpc_c::value_struct(kc);
    
    params.add(xmlrpc_c::value_struct(variable));
    
    xmlrpc_c::value result;
    
    try {
        result = execute("save_configuration",params);
    }
    catch(girerr::error& e) {
        cerr<<e.what()<<endl;
        throw N4DError::Connection;
    }
    
    if (!result.isInstantiated()) {
        throw N4DError::BadResponse;
    }
    
    if (result.type()!=xmlrpc_c::value::TYPE_STRUCT) {
        cerr<<"Expected struct"<<endl;
        throw N4DError::Set;
    }
    
    map<string,xmlrpc_c::value> res = xmlrpc_c::value_struct(result).cvalue();
    
    if (res.find("status")==res.end()) {
        cerr<<"Status not found"<<endl;
        throw N4DError::Set;
    }
    
    xmlrpc_c::value status = res["status"];
    
    if (status.type()!=xmlrpc_c::value::TYPE_BOOLEAN) {
        cerr<<"Unexepected status format"<<endl;
        throw N4DError::Set;
    }
    
    if (!xmlrpc_c::value_boolean(status).cvalue()) {
        
        //We are assuming msg field exists and it is a string :S
        cerr<<"Msg:"<<xmlrpc_c::value_string(res["msg"]).cvalue()<<endl;
        
        throw N4DError::Set;
    }
    
    
}