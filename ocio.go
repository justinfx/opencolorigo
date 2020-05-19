/*

OpenColorIO bindings - http://opencolorio.org/developers/api/OpenColorIO.html

*/
package ocio

/*
#cgo CPPFLAGS: -I./cpp -Wno-return-type
#cgo LDFLAGS: -lstdc++

#include "stdlib.h"

#include "cpp/ocio.h"
*/
import "C"

import (
	"errors"
)

type (
	LoggingLevelType int
	EnvironmentMode  int
	InterpType       int
)

const (
	LOGGING_LEVEL_NONE    LoggingLevelType = C.LOGGING_LEVEL_NONE
	LOGGING_LEVEL_WARNING LoggingLevelType = C.LOGGING_LEVEL_WARNING
	LOGGING_LEVEL_INFO    LoggingLevelType = C.LOGGING_LEVEL_INFO
	LOGGING_LEVEL_DEBUG   LoggingLevelType = C.LOGGING_LEVEL_DEBUG
	LOGGING_LEVEL_UNKNOWN LoggingLevelType = C.LOGGING_LEVEL_UNKNOWN
)

const (
	ENVIRONMENT_UNKNOWN         EnvironmentMode = C.ENV_ENVIRONMENT_UNKNOWN
	ENVIRONMENT_LOAD_PREDEFINED EnvironmentMode = C.ENV_ENVIRONMENT_LOAD_PREDEFINED
	ENVIRONMENT_LOAD_ALL        EnvironmentMode = C.ENV_ENVIRONMENT_LOAD_ALL
)

const (
	INTERP_UNKNOWN     InterpType = C.INTERP_UNKNOWN
	INTERP_NEAREST     InterpType = C.INTERP_NEAREST
	INTERP_LINEAR      InterpType = C.INTERP_LINEAR
	INTERP_TETRAHEDRAL InterpType = C.INTERP_TETRAHEDRAL
	INTERP_BEST        InterpType = C.INTERP_BEST
)

var (
	ROLE_DEFAULT         = C.GoString(C.ROLE_DEFAULT)
	ROLE_REFERENCE       = C.GoString(C.ROLE_REFERENCE)
	ROLE_DATA            = C.GoString(C.ROLE_DATA)
	ROLE_COLOR_PICKING   = C.GoString(C.ROLE_COLOR_PICKING)
	ROLE_SCENE_LINEAR    = C.GoString(C.ROLE_SCENE_LINEAR)
	ROLE_COMPOSITING_LOG = C.GoString(C.ROLE_COMPOSITING_LOG)
	ROLE_COLOR_TIMING    = C.GoString(C.ROLE_COLOR_TIMING)
	ROLE_TEXTURE_PAINT   = C.GoString(C.ROLE_TEXTURE_PAINT)
	ROLE_MATTE_PAINT     = C.GoString(C.ROLE_MATTE_PAINT)
)

/*
Errors
*/

func getLastError(ptr *C._HandleContext) error {
	return errors.New(C.GoString(ptr.last_error))
}

// An exception class for errors detected at runtime,
// thrown when OCIO cannot find a file that is expected to exist.
// This is provided as a custom type to distinguish cases where
// one wants to continue looking for missing files, but wants to
// properly fail for other error conditions.
type ErrMissingFile struct{ what string }

func (e ErrMissingFile) Error() string { return e.what }

/*
Global
*/

/*
OpenColorIO, during normal usage, tends to cache certain information
(such as the contents of LUTs on disk, intermediate results, etc.).
Calling this function will flush all such information.
Under normal usage, this is not necessary, but it can be helpful in
particular instances, such as designing OCIO profiles, and wanting
to re-read luts without restarting.
*/
func ClearAllCaches() {
	C.ClearAllCaches()
}

// Get the version number for the library, as a dot-delimited string (e.g., “1.0.0”).
// This is also available at compile time as OCIO_VERSION.
func Version() string {
	return C.GoString(C.GetVersion())
}

// Get the version number for the library, as a single 4-byte hex number
// (e.g., 0x01050200 for “1.5.2”), to be used for numeric comparisons.
// This is also available at compile time as OCIO_VERSION_HEX.
func VersionHex() int {
	return int(C.GetVersionHex())
}

// Get the global logging level. You can override this at runtime using the
// OCIO_LOGGING_LEVEL environment variable. The client application that sets
// this should use SetLoggingLevel(), and not the environment variable.
// The default value is INFO.
//
// Returns on of the LOGGING_LEVEL_* const values
func LoggingLevel() LoggingLevelType {
	return LoggingLevelType(C.GetLoggingLevel())
}

// Set the global logging level.
func SetLoggingLevel(level LoggingLevelType) {
	C.SetLoggingLevel(C.LoggingLevel(level))
}
