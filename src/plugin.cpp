#include "plugin.h"

Plugin *Plugin::headPlugin = NULL;

Plugin::Plugin() {
  nextPlugin = headPlugin;
  headPlugin = this;
}

void Plugin::setupAll() {
  for (Plugin *r = headPlugin; r; r = r->nextPlugin) {
    r->setup();
    yield();
  }
}

void Plugin::loopAll() {
  for (Plugin *r = headPlugin; r; r = r->nextPlugin) {
    r->loop();
    yield();
  }
}

void Plugin::jsonAll(AsyncWebServerRequest *request,AsyncResponseStream *response) {
  for (Plugin *r = headPlugin; r; r = r->nextPlugin) {
    r->json(request,response);
    //yield(); Don't use yield in a callback! This cause reboots
  }
}
