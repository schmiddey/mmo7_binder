/*
 * mmo7_get_btn_id.cpp
 *
 *  Created on: 13.02.2015
 *      Author: m1ch1
 */

#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <csignal>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <linux/joystick.h>


#define PRESS 1
#define RELEASE 0

#define JS_EVENT_BUTTON         0x01    /* button pressed/released */
#define JS_EVENT_AXIS           0x02    /* joystick moved */
#define JS_EVENT_INIT           0x80    /* initial state of device */

int fd;

void signalExit(int signum)
{
   std::cout << "Exit mmo7_get_btn_id" << std::endl;

   close(fd);

   exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{

   if(argc != 2)
   {
      std::cerr << "Syntax error. Usage: " << argv[0] << "joystickdevice" << std::endl;
      exit(EXIT_FAILURE);
   }


   fd = open(argv[1], O_RDONLY);

   if(fd < 0)
   {
      std::cerr << "Unable to open " << argv[1] <<  "  ... will exit" << std ::endl;
      exit(EXIT_FAILURE);
   }

   //register signalFct
   std::signal(SIGINT, signalExit);

   while(1)
   {

      struct js_event e;
      int ret = read (fd, &e, sizeof(e));
      if(ret != sizeof(e))
      {
         std::cerr << "unable to read enough data from " << argv[1] << std::endl;
         continue;
      }

      if(e.type == JS_EVENT_BUTTON)
      {
         std::cout << "Button: " << (int)e.number << " ,type: " <<( e.value == RELEASE ? "release" : "press") << std::endl;
      }
   }

   return 0;
}
