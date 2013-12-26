package opencolorigo

/*
#cgo LDFLAGS: -lstdc++
#cgo pkg-config: OpenColorIO

#include "stdlib.h"

#include "ocio.h"

*/
import "C"

/*
Errors
*/

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
func GetVersion() string {
    return C.GoString(C.GetVersion())
}

// Get the version number for the library, as a single 4-byte hex number
// (e.g., 0x01050200 for “1.5.2”), to be used for numeric comparisons.
// This is also available at compile time as OCIO_VERSION_HEX.
func GetVersionHex() int {
    return int(C.GetVersionHex())
}
