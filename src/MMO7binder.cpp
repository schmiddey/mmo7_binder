/*
 * MMO7binder.cpp
 *
 *  Created on: 13.02.2015
 *      Author: m1ch1
 */

#include "MMO7binder.h"

namespace{
const int RET_ERROR = -1;
const int RET_SUCCES = 0;
}

namespace apps
{

MMO7_binder::MMO7_binder()
{
   _device = std::string("/dev/input/js0");
   _fd = 0;
}


MMO7_binder::~MMO7_binder()
{
   close(_fd);
   //delete actions
   for(unsigned int i = 0; i < _actions.size(); ++i)
   {
      delete _actions[i];
   }
}

int MMO7_binder::parseConfigFile(std::string config_file)
{
   tinyxml2::XMLDocument config;
   config.LoadFile(config_file.c_str());
   if(config.ErrorID() != 0)
   {
      std::cerr << "unable to open XML config file " << config_file << " ..." << std::endl;
      return RET_ERROR;
   }
   //get device name
   _device = config.FirstChildElement("config")->FirstChildElement("mouse_js_device")->GetText();

   std::vector<apps::Action*> tmp_actions;
   tinyxml2::XMLElement* action_element = NULL;
   while(1)
   {
      action_element = config.FirstChildElement("config")->FirstChildElement("action");
      if(action_element == NULL)
         break;

      apps::Action* tmp_action = new apps::Action;

      std::stringstream sstr;
      sstr << action_element->FirstChildElement("mouse_btn")->GetText();
      unsigned int tmp_btn = 0;
      sstr >> tmp_btn;
      tmp_action->btn = tmp_btn;

      std::string type = action_element->FirstChildElement("type")->GetText();
      if(type == "release")
      {
         tmp_action->type = RELEASE;
      }
      else
      {
         tmp_action->type = PRESS;
      }

      tmp_action->cmd = action_element->FirstChildElement("system_cmd")->GetText();

      tmp_actions.push_back(tmp_action);

      //to get next child element with same name
      config.FirstChildElement("config")->DeleteChild(action_element);
   }

   //get greatest Key element
   unsigned int greatestKey = 0;
   for(unsigned int i = 0; i < tmp_actions.size(); ++i)
   {
      if(tmp_actions[i]->btn > greatestKey)
         greatestKey = tmp_actions[i]->btn;
   }

   if(greatestKey < tmp_actions.size())
      greatestKey = tmp_actions.size();

   _actions.resize(greatestKey + 1);
   for(unsigned int i = 0; i < _actions.size(); ++i)
   {
      _actions[i] = NULL;
   }


   //set actions at index with corresonding buttons
   for(unsigned int i = 0; i < tmp_actions.size(); ++i)
   {
      _actions[tmp_actions[i]->btn] = tmp_actions[i];
   }

   return RET_SUCCES;
}

int MMO7_binder::openDevice(void)
{
   //open in blocking mode
   _fd = open(_device.c_str(), O_RDONLY);
   if(_fd < 0)
   {
      std::cerr << "unable to open device(" << _device << ")" << std::endl;
      return RET_ERROR;
   }

   return RET_SUCCES;
}

void MMO7_binder::run()
{
   struct js_event e;

   int ret = read(_fd, &e, sizeof(e));
   if(ret != sizeof(e))
   {
      std::cerr << "unable to read enough data from " << _device << std::endl;
      return;
   }

   if(e.number >= _actions.size())
      return;

   //get action
   apps::Action* action = _actions[e.number];

   if(action)
   {
      //execute command if type is equal
      if(e.type == JS_EVENT_BUTTON && (unsigned int)e.value == action->type)
      {
         if(system(action->cmd.c_str()) < 0)
         {
            std::cerr << "unable to execute command" << std::endl;
            return;
         }
      }
   }
}

} /* namespace apps */
