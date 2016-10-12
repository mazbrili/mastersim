#ifndef IBK_configuration_h
#define IBK_configuration_h

#include "IBK_BuildFlags.h"

#ifdef _MSC_VER
	typedef __int64 int64_t;
	typedef unsigned __int64 uint64_t;
	typedef unsigned __int32 uint32_t;
	typedef unsigned __int16 uint16_t;
#elif _GLIBCXX_HAVE_STDINT_H
	#include <stdint.h>
#endif


/// Enables/Disables function argument checking for all math functions.
#define IBK_ENABLE_SAFE_MATH

/// Enables/Disables colored text in the console window.
#define IBK_ENABLE_COLORED_CONSOLE

/// Enables/Disables name string in IBK::Unit (for debugging)
#define IBK_ENABLE_UNIT_NAME

/*! \file IBK_configuration.h
	\brief Central configuration file for IBK library, include in all header/source files that
			make use of the IBK macros and defines.

	Configuration header files for IBK library
	------------------------------------------

	If you don't use the CMake build system, copy this file over to
	IBK_configuration.h and manually define all options you want to have
	enabled.

	This file should be included into every header and source file of the
	IBK lib where compilation options/macros are used.

	\warning This header file is generated from IBK_configuration.h.in, so any changes made in this file will
	be lost in the next cmake configuration.

*/

/*! \file IBK_configuration.h
	\brief Compilation configurations and macros for CMake build system.
*/

#endif // IBK_configuration_h
