//! \file Logging.h Contains functions and macros for Boost logging.

#pragma once
#include "easylogging++.h"

//! Initialize everything required for logging.
/*!
* Sets up easylogging++ and redirects stdout to a separate log file.
*/
extern void init_logging(int argv, char* argc[]);