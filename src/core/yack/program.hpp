//! \file

#ifndef YACK_PROGRAM_INCLUDED
#define YACK_PROGRAM_INCLUDED 1

#include "yack/exception.hpp"
#include <iostream>

#define YACK_PROGRAM() \
/**/  int main(int argc, char **argv) \
/**/    {  const char *program = argv[0]; (void)argc; (void)program;\
/**/      try {

#define YACK_DONE() \
/**/          }\
/**/      catch(const yack::exception &e)\
/**/      { e.show(std::cerr); return -1; }\
/**/      catch(const std::exception &e)\
/**/      { std::cerr << "std::exception: " << e.what() << std::endl; return -1;}\
/**/      catch(...)\
/**/      { std::cerr << "unhandled exception" << std::endl; return -1;}\
/**/    return 0;\
/**/  }
 #endif

