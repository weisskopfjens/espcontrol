#include "webserver.h"

extern Core core;

void handle_FILE_iro (AsyncWebServerRequest *request) {
  //request->send_P(200,"application/javascript",FILE_iro);
  AsyncWebServerResponse *response = request->beginResponse_P(200, F("application/javascript"), iro_js_gz, iro_js_gz_len);
  response->addHeader(F("Content-Encoding"), F("gzip"));
  request->send(response);
}

void handle_FILE_w3css (AsyncWebServerRequest *request) {
  //request->send_P(200,"text/css",FILE_w3css);
  AsyncWebServerResponse *response = request->beginResponse_P(200, F("text/css"), w3_css_gz, w3_css_gz_len);
  response->addHeader(F("Content-Encoding"), F("gzip"));
  request->send(response);
}

void handle_FILE_filemanager (AsyncWebServerRequest *request) {
  /*if(strlen(core.cfg.get("user")->getValue())>0 && strlen(core.cfg.get("password")->getValue())>0) {
    if(!request->authenticate(core.cfg.get("user")->getValue(), core.cfg.get("password")->getValue())) {
      return request->requestAuthentication();
    }
  }*/
  request->send_P(200,F("text/html"),SIDE_filemanager);
}

void handleFiles (AsyncWebServerRequest *request) {
  /*if(strlen(core.cfg.get("user")->getValue())>0 && strlen(core.cfg.get("password")->getValue())>0) {
    if(!request->authenticate(core.cfg.get("user")->getValue(), core.cfg.get("password")->getValue())) {
      return request->requestAuthentication();
    }
  }*/
  AsyncResponseStream *response = request->beginResponseStream(F("text/html"));
  response->printf_P(SIDE_begin,"Upload");
  response->printf_P(SIDE_upload);
  response->printf_P(SIDE_end);
  request->send(response);
}

void handleUpload (AsyncWebServerRequest *request) {
  request->redirect("/filemanager");
}

void handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  if(!index){
    core._file = SPIFFS.open(filename, "w");
    Serial.print(F("INFO: UploadStart:"));
    Serial.println(filename);
  }
  for(size_t i=0; i<len; i++){
    core._file.write(data[i]);
  }
  if(final){
    core._file.close();
    Serial.print(F("INFO: UploadEnd:"));
    Serial.println(filename+(String)(index+len));
    request->redirect("/filemanager");
  }
}

void handleConfig (AsyncWebServerRequest *request) {
  /*if(strlen(core.cfg.get("user")->getValue())>0 && strlen(core.cfg.get("password")->getValue())>0) {
    if(!request->authenticate(core.cfg.get("user")->getValue(), core.cfg.get("password")->getValue())) {
      return request->requestAuthentication();
    }
  }*/
  #ifdef DEBUGMODE
    Serial.println("DEBUG: cfg.length = "+(String)core.cfg.length());
  #endif
  bool formsend = false;
  config_result saveConfig_success;
  for(uint8 i=0;i<core.cfg.length();i++) {
    #ifdef DEBUGMODE
      Serial.println("DEBUG: cfg.key = "+(String)core.cfg.getIndex(i)->getKey());
    #endif
    if (request->hasParam(core.cfg.getIndex(i)->getKey(),true)) {
      core.cfg.getIndex(i)->setValue( request->getParam(core.cfg.getIndex(i)->getKey(),true)->value().c_str() );
      formsend = true;
    }
  }
  if( formsend ) {
    Serial.println(F("INFO: Saving config..."));
    saveConfig_success = core.cfg.write();
  }
  AsyncResponseStream *response = request->beginResponseStream(F("text/html"));
  response->printf_P(SIDE_begin,"Configuration");
  response->printf_P(SIDE_configbegin);
  for(uint8 i=0;i<core.cfg.length();i++) {
    if(strcmp(core.cfg.getIndex(i)->getKey(),"rules")==0) {
      response->printf_P(SIDE_textconfigitem,core.cfg.getIndex(i)->getKey(),core.cfg.getIndex(i)->getKey(),core.cfg.getIndex(i)->getValue());
    } else {
      response->printf_P(SIDE_configitem,core.cfg.getIndex(i)->getKey(),core.cfg.getIndex(i)->getKey(),core.cfg.getIndex(i)->getValue());
    }
  }
  if(luaresult.length()>0) {
    response->printf_P(SIDE_configerror,luaresult.c_str());
  }
  response->printf_P(SIDE_configend);
  if(formsend) {
    if(saveConfig_success==0) {
      response->printf_P(SIDE_configsuccess);
    } else {
      response->printf_P(SIDE_configerror,F("Configuration was not saved."));
    }
  }
  response->printf_P(SIDE_end);
  request->send(response);
}

void handleJSON (AsyncWebServerRequest *request) {
  AsyncResponseStream *response = request->beginResponseStream(F("application/json"));
  Plugin::jsonAll(request,response);
  request->send(response);
}

void handleControl (AsyncWebServerRequest *request) {
  /*if(strlen(core.cfg.get("user")->getValue())>0 && strlen(core.cfg.get("password")->getValue())>0) {
    if(!request->authenticate(core.cfg.get("user")->getValue(), core.cfg.get("password")->getValue())) {
      return request->requestAuthentication();
    }
  }*/
  if ( request->hasParam("cmd") ) {
    AsyncResponseStream *response = request->beginResponseStream(F("text/html"));
    core.command+=request->getParam("cmd")->value();
    core.command+=";";
    response->println(F("ok"));
    request->send(response);
  } else if ( request->hasParam("luaerror") ) {
    AsyncResponseStream *response = request->beginResponseStream(F("text/html"));
    response->print(luaresult);
    request->send(response);
  }
}

void handleUDPPacket(AsyncUDPPacket packet) {
  for(uint i=0;i<packet.length();i++) {
    core.command+=((char*)packet.data())[i];
  }
  core.command+=";";
  #ifdef DEBUGMODE
    Serial.println(F("INFO: udp packet received"));
    Serial.print("DEBUG:");
    Serial.println(core.command);
  #endif
}
