package ocio

/*
#cgo LDFLAGS: -lstdc++
#cgo pkg-config: OpenColorIO

#include "stdlib.h"

#include "ocio.h"

*/
import "C"

import "unsafe"

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

/*
Config
*/

// A config defines all the color spaces to be available at runtime.
type Config struct {
    ptr unsafe.Pointer
}

/*
Config Initialization
*/

// Get the current configuration.
// If a current config had not yet been set, it will be automatically
// initialized from the environment.
func GetCurrentConfig() Config {
    return Config{C.GetCurrentConfig()}
}

// Create a Config by checking the OCIO environment variable
func ConfigCreateFromEnv() Config {
    return Config{C.Config_CreateFromEnv()}
}

// Create a Config from an existing yaml Config file
func ConfigCreateFromFile(filename string) Config {
    c_str := C.CString(filename)
    defer C.free(unsafe.Pointer(c_str))
    return Config{C.Config_CreateFromFile(c_str)}
}

// Create a Config from a valid yaml string
func ConfigCreateFromData(data string) Config {
    c_str := C.CString(data)
    defer C.free(unsafe.Pointer(c_str))
    return Config{C.Config_CreateFromData(c_str)}
}

/*
This will produce a hash of the all colorspace definitions, etc.
All external references, such as files used in FileTransforms, etc.,
will be incorporated into the cacheID. While the contents of the files
are not read, the file system is queried for relavent information (mtime, inode)
so that the Config’s cacheID will change when the underlying luts are updated.
If a context is not provided, the current Context will be used.
If a null context is provided, file references will not be taken into account
(this is essentially a hash of Config::serialize).
*/
func (c Config) GetCacheID() string {
    return C.GoString(C.Config_getCacheID(c.ptr))
}

func (c Config) GetDescription() string {
    return C.GoString(C.Config_getDescription(c.ptr))
}

func (c Config) IsStrictParsingEnabled() bool {
    return bool(C.Config_isStrictParsingEnabled(c.ptr))
}

func (c Config) SetStrictParsingEnabled(enabled bool) {
    C.Config_setStrictParsingEnabled(c.ptr, C.bool(enabled))
}

/*
Config Resources
*/

// Given a lut src name, where should we find it?
func (c Config) GetSearchPath() string {
    return C.GoString(C.Config_getSearchPath(c.ptr))
}

// Given a lut src name, where should we find it?
func (c Config) GetWorkingDir() string {
    return C.GoString(C.Config_getWorkingDir(c.ptr))
}

/*
Config ColorSpaces
*/

func (c Config) GetNumColorSpaces() int {
    return int(C.Config_getNumColorSpaces(c.ptr))
}

// This will null if an invalid index is specified
func (c Config) GetColorSpaceNameByIndex(index int) string {
    return C.GoString(C.Config_getColorSpaceNameByIndex(c.ptr, C.int(index)))
}

func (c Config) GetIndexForColorSpace(name string) int {
    c_str := C.CString(name)
    defer C.free(unsafe.Pointer(c_str))
    return int(C.Config_getIndexForColorSpace(c.ptr, c_str))
}

/*
Config Roles

A role is like an alias for a colorspace. You can query the colorspace
corresponding to a role using the normal getColorSpace fcn.
*/

// Setting the colorSpaceName name to a null string unsets it.
func (c Config) SetRole(role, colorSpaceName string) {
    c_role := C.CString(role)
    defer C.free(unsafe.Pointer(c_role))

    var c_space *C.char
    if colorSpaceName != "" {
        c_space = C.CString(colorSpaceName)
        defer C.free(unsafe.Pointer(c_space))
    }

    C.Config_setRole(c.ptr, c_role, c_space)
}

func (c Config) GetNumRoles() int {
    return int(C.Config_getNumRoles(c.ptr))
}

// Return true if the role has been defined.
func (c Config) HasRole(role string) bool {
    c_str := C.CString(role)
    defer C.free(unsafe.Pointer(c_str))
    return bool(C.Config_hasRole(c.ptr, c_str))
}

// Get the role name at index, this will return values like ‘scene_linear’,
// ‘compositing_log’. Return empty string if index is out of range.
func (c Config) GetRoleName(index int) string {
    return C.GoString(C.Config_getRoleName(c.ptr, C.int(index)))
}
