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
#include <tinyxml2.h>

#include "MMO7binder.h"

apps::MMO7_binder* binder = NULL;

void signalExit(int signum)
{
   std::cout << "Exit mmo7_binder" << std::endl;

   if(binder)
      delete binder;

   exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
   //register signalFct
   std::signal(SIGINT, signalExit);

    binder = new apps::MMO7_binder;

   if(argc == 1)
   {//no args given .... juse std config file ~/.mmo7_binder_cfg.xml
      if(binder->parseConfigFile("/etc/mmo7_binder/mmo7_binder_cfg.xml") < 0)
      {
         std::cerr << "error at parsing default config file: /etc/mmo7_binder/mmo7_binder_cfg.xml ... will exit" << std::endl;
         delete binder;
         exit(EXIT_FAILURE);
      }
   }
   else if(argc == 2)
   {
      std::cout << "Using none default config file: " << argv[1] << std::endl;
      if(binder->parseConfigFile(argv[1]) < 0)
      {
         std::cerr << "error at parsing config file... will exit" << std::endl;
         delete binder;
         exit(EXIT_FAILURE);
      }
   }
   else
   {
      std::cerr << "Syntax error. Usage: " << argv[0] << " [config file]" << std::endl;
      delete binder;
      exit(EXIT_FAILURE);
   }

   if(binder->openDevice() < 0)
   {
      std::cerr << "error at opening device ... will exit" << std::endl;
      delete binder;
      exit(EXIT_FAILURE);
   }

   while(1)
   {
      binder->run();
   }

   delete binder;

   exit(EXIT_SUCCESS);
}
