/*
 * MMO7binder.h
 *
 *  Created on: 13.02.2015
 *      Author: m1ch1
 */

#ifndef MMO7BINDER_H_
#define MMO7BINDER_H_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <linux/joystick.h>

#include <tinyxml2.h>

#define PRESS 1
#define RELEASE 0

#define JS_EVENT_BUTTON         0x01    /* button pressed/released */
#define JS_EVENT_AXIS           0x02    /* joystick moved */
#define JS_EVENT_INIT           0x80    /* initial state of device */

namespace apps
{

struct Action{
   unsigned int btn;
   unsigned int type;
   std::string cmd;

   Action() :
      btn(0),
      type(PRESS)
   {}
};

class MMO7_binder
{
public:
   MMO7_binder();
   ~MMO7_binder();

   int parseConfigFile(std::string config_file);
   int openDevice(void);

   void run();

private: //data elements
   int _fd;
   std::string _device;
   std::vector<Action*> _actions;
};

} /* namespace apps */

#endif /* MMO7BINDER_H_ */
