//! \file Logging.h Contains functions and macros for Boost logging.

#pragma once

#define _ELPP_NO_DEFAULT_LOG_FILE

// Not used in this file but included so only this header is needed
// in other files for logging
#include "easylogging++.h"

namespace ECSE
{

//! Initialize everything required for logging.
/*!
* Sets up easylogging++ and redirects stdout to a separate log file.
*/
extern void init_logging(int argv, char* argc[]);

}
