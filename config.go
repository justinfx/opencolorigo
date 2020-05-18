package ocio

// #include "stdlib.h"
//
// #include "cpp/ocio.h"
//
import "C"

import (
	"errors"
	"fmt"
	"reflect"
	"runtime"
	"unsafe"
)

// A Config defines all the colorspaces available at runtime.
type Config struct {
	ptr *C.Config
}

/*
Config Initialization
*/

func newConfig(p *C.Config) *Config {
	cfg := &Config{p}
	runtime.SetFinalizer(cfg, deleteConfig)
	return cfg
}

func deleteConfig(c *Config) {
	if c == nil {
		return
	}
	if c.ptr != nil {
		runtime.SetFinalizer(c, nil)
		C.freeContext((*C._Context)(c.ptr))
		c.ptr = nil
	}
	runtime.KeepAlive(c)
}

// Create a new empty Config
func NewConfig() *Config {
	return newConfig(C.Config_Create())
}

// Get the current configuration.
// If a current config had not yet been set, it will be automatically
// initialized from the environment.
func CurrentConfig() (*Config, error) {
	c, err := C.GetCurrentConfig()
	if err != nil {
		return nil, getLastError((*C._Context)(c))
	}
	return newConfig(c), err
}

// Set the current configuration. This will then store a copy of the specified config.
func SetCurrentConfig(config *Config) error {
	_, err := C.SetCurrentConfig(config.ptr)
	runtime.KeepAlive(config)
	return err
}

// Create a Config by checking the OCIO environment variable
func ConfigCreateFromEnv() (*Config, error) {
	c, err := C.Config_CreateFromEnv()
	if err != nil {
		return nil, getLastError((*C._Context)(c))
	}
	return newConfig(c), err
}

// Create a Config from an existing yaml Config file
func ConfigCreateFromFile(filename string) (*Config, error) {
	c_str := C.CString(filename)
	defer C.free(unsafe.Pointer(c_str))

	c, err := C.Config_CreateFromFile(c_str)
	if err != nil {
		return nil, getLastError((*C._Context)(c))
	}
	return newConfig(c), err
}

// Create a Config from a valid yaml string
func ConfigCreateFromData(data string) (*Config, error) {
	c_str := C.CString(data)
	defer C.free(unsafe.Pointer(c_str))

	c, err := C.Config_CreateFromData(c_str)
	if err != nil {
		return nil, getLastError((*C._Context)(c))
	}
	return newConfig(c), err
}

// Destroy immediately frees resources for this
// instance instead of waiting for garbage collection
// finalizer to run at some point later
func (c *Config) Destroy() {
	deleteConfig(c)
}

func (c *Config) lastError() error {
	e := C.GoString(c.ptr.last_error)
	if e == "" {
		return nil
	}
	runtime.KeepAlive(c)
	return errors.New(e)
}

// Create a new editable copy of this Config
func (c *Config) EditableCopy() *Config {
	ret := newConfig(C.Config_createEditableCopy(c.ptr))
	runtime.KeepAlive(c)
	return ret
}

// This will return a non-nil error if the config is malformed.
// The most common error occurs when references are made to colorspaces that do not exist.
func (c *Config) SanityCheck() error {
	_, err := C.Config_sanityCheck(c.ptr)
	if err != nil {
		return c.lastError()
	}
	runtime.KeepAlive(c)
	return nil
}

func (c *Config) Serialize() (string, error) {
	c_str, err := C.Config_serialize(c.ptr)
	if err != nil {
		return "", c.lastError()
	}
	defer C.free(unsafe.Pointer(c_str))
	runtime.KeepAlive(c)
	return C.GoString(c_str), err
}

/*
This will produce a hash of the all colorspace definitions, etc.
All external references, such as files used in FileTransforms, etc.,
will be incorporated into the cacheID. While the contents of the files
are not read, the file system is queried for relavent information (mtime, inode)
so that the Config’s cacheID will change when the underlying luts are updated.

The current Context will be used.
*/
func (c *Config) CacheID() (string, error) {
	id, err := C.Config_getCacheID(c.ptr)
	if err != nil {
		return "", c.lastError()
	}
	runtime.KeepAlive(c)
	return C.GoString(id), err
}

/*
This will produce a hash of the all colorspace definitions, etc.
All external references, such as files used in FileTransforms, etc.,
will be incorporated into the cacheID. While the contents of the files
are not read, the file system is queried for relavent information (mtime, inode)
so that the Config’s cacheID will change when the underlying luts are updated.

If a nil context is provided, file references will not be taken into account
(this is essentially a hash of Config.Serialize).
*/
func (c *Config) CacheIDWithContext(context *Context) (string, error) {
	if context == nil {
		context = NewContext()
	}

	id, err := C.Config_getCacheIDWithContext(c.ptr, context.ptr)
	if err != nil {
		return "", c.lastError()
	}
	runtime.KeepAlive(c)
	runtime.KeepAlive(context)
	return C.GoString(id), err
}

func (c *Config) Description() (string, error) {
	d, err := C.Config_getDescription(c.ptr)
	if err != nil {
		return "", c.lastError()
	}
	runtime.KeepAlive(c)
	return C.GoString(d), err
}

func (c *Config) IsStrictParsingEnabled() bool {
	enabled, err := C.Config_isStrictParsingEnabled(c.ptr)
	if err != nil {
		return false
	}
	runtime.KeepAlive(c)
	return bool(enabled)
}

func (c *Config) SetStrictParsingEnabled(enabled bool) error {
	_, err := C.Config_setStrictParsingEnabled(c.ptr, C.bool(enabled))
	if err != nil {
		return c.lastError()
	}
	runtime.KeepAlive(c)
	return err
}

/*
Config Resources
*/

func (c *Config) CurrentContext() (*Context, error) {
	ptr, err := C.Config_getCurrentContext(c.ptr)
	if err != nil {
		return nil, c.lastError()
	}
	runtime.KeepAlive(c)
	return newContext(ptr), err
}

// Given a lut src name, where should we find it?
func (c *Config) SearchPath() (string, error) {
	path, err := C.Config_getSearchPath(c.ptr)
	if err != nil {
		return "", c.lastError()
	}
	runtime.KeepAlive(c)
	return C.GoString(path), err
}

// Given a lut src name, where should we find it?
func (c *Config) WorkingDir() (string, error) {
	dir, err := C.Config_getWorkingDir(c.ptr)
	if err != nil {
		return "", c.lastError()
	}
	runtime.KeepAlive(c)
	return C.GoString(dir), err
}

/*
Config Processors
*/

/*
Convert from inputColorSpace to outputColorSpace

There are 4 ways this method may be called:

    config.Processor(ctx *Context, src *ColorSpace, dst *ColorSpace)
    config.Processor(ctx *Context, src string, dst string)
    config.Processor(src *ColorSpace, dst *ColorSpace)
    config.Processor(src string, dst string)

String names can be colorspace name, role name, or a combination of both.

This may provide higher fidelity than anticipated due to internal optimizations.
For example, if the inputcolorspace and the outputColorSpace are members of the
same family, no conversion will be applied, even though strictly speaking
quantization should be added

*/
func (c *Config) Processor(args ...interface{}) (*Processor, error) {
	count := len(args)
	if count != 2 && count != 3 {
		return nil, fmt.Errorf("Requires either 2 or 3 parameters; Got %d", count)
	}

	var (
		err  error
		proc unsafe.Pointer
	)

	bad_str := "Error creating processor with src colorspace %v / dst colorspace %v"

	if count == 3 {

		ct, ok := args[0].(*Context)
		if !ok {
			return nil, errors.New("1st argument is not a Context*")
		}

		a1 := reflect.ValueOf(args[1])
		a2 := reflect.ValueOf(args[2])

		if a1.Kind() == reflect.String && a2.Kind() == reflect.String {
			c_a1 := C.CString(a1.String())
			c_a2 := C.CString(a2.String())
			defer C.free(unsafe.Pointer(c_a1))
			defer C.free(unsafe.Pointer(c_a2))

			proc = C.Config_getProcessor_CT_S_S(c.ptr, ct.ptr, c_a1, c_a2)
			if err = c.lastError(); err != nil {
				err = fmt.Errorf("%s: %s", fmt.Sprintf(bad_str, a1, a2), err)
			}
			return newProcessor(proc), err
		}

		if a1.Kind() == reflect.Ptr && a2.Kind() == reflect.Ptr {
			if aPtr1, ok := args[1].(*ColorSpace); ok {
				if aPtr2, ok := args[2].(*ColorSpace); ok {
					proc = C.Config_getProcessor_CT_CS_CS(c.ptr, ct.ptr, aPtr1.ptr, aPtr2.ptr)
					if err = c.lastError(); err != nil {
						err = fmt.Errorf("%s: %s", fmt.Sprintf(bad_str, a1, a2), err)
					}
					return newProcessor(proc), err
				}
			}
		}

	} else if count == 2 {

		a1 := reflect.ValueOf(args[0])
		a2 := reflect.ValueOf(args[1])

		if a1.Kind() == reflect.String && a2.Kind() == reflect.String {
			c_a1 := C.CString(a1.String())
			c_a2 := C.CString(a2.String())
			defer C.free(unsafe.Pointer(c_a1))
			defer C.free(unsafe.Pointer(c_a2))

			proc = C.Config_getProcessor_S_S(c.ptr, c_a1, c_a2)
			if err = c.lastError(); err != nil {
				err = fmt.Errorf("%s: %s", fmt.Sprintf(bad_str, a1, a2), err)
			}
			return newProcessor(proc), err
		}

		if a1.Kind() == reflect.Ptr && a2.Kind() == reflect.Ptr {
			if aPtr1, ok := args[0].(*ColorSpace); ok {
				if aPtr2, ok := args[1].(*ColorSpace); ok {
					proc = C.Config_getProcessor_CS_CS(c.ptr, aPtr1.ptr, aPtr2.ptr)
					if err = c.lastError(); err != nil {
						err = fmt.Errorf("%s: %s", fmt.Sprintf(bad_str, a1, a2), err)
					}
					return newProcessor(proc), err
				}
			}
		}
	}

	runtime.KeepAlive(c)
	return nil, fmt.Errorf("Wrong argument types: %v", args)
}

/*
Get the processor for the specified transform, using the current Config context.

Not often needed, but will allow for the re-use of atomic OCIO functionality
(such as to apply an individual LUT file).
*/
func (c *Config) ProcessorTransform(tx Transform) (*Processor, error) {
	ptr := C.Config_getProcessor_TX(c.ptr, tx.transformHandle())
	if err := c.lastError(); err != nil {
		return nil, c.lastError()
	}
	proc := newProcessor(ptr)
	runtime.KeepAlive(c)
	runtime.KeepAlive(tx)
	return proc, nil
}

/*
Get the processor for the specified transform, and a transform direction,
using the current Config context.

Not often needed, but will allow for the re-use of atomic OCIO functionality
(such as to apply an individual LUT file).
*/
func (c *Config) ProcessorTransformDir(tx Transform, dir TransformDirection) (*Processor, error) {
	ptr := C.Config_getProcessor_TX_D(c.ptr, tx.transformHandle(), C.TransformDirection(dir))
	if err := c.lastError(); err != nil {
		return nil, c.lastError()
	}
	proc := newProcessor(ptr)
	runtime.KeepAlive(c)
	runtime.KeepAlive(tx)
	return proc, nil
}

/*
Get the processor for the specified transform, using a specific Context instead of
the current Config context, and a transform direction.

Not often needed, but will allow for the re-use of atomic OCIO functionality
(such as to apply an individual LUT file).
*/
func (c *Config) ProcessorCtxTransformDir(
	ctx *Context, tx Transform, dir TransformDirection) (*Processor, error) {

	ptr := C.Config_getProcessor_CT_TX_D(
		c.ptr, ctx.ptr, tx.transformHandle(), C.TransformDirection(dir))

	if err := c.lastError(); err != nil {
		return nil, c.lastError()
	}

	proc := newProcessor(ptr)
	runtime.KeepAlive(c)
	runtime.KeepAlive(ctx)
	runtime.KeepAlive(tx)
	return proc, nil
}

/*
Config ColorSpaces
*/

// This will return null if the specified name is not found.
// Accepts either a color space OR role name. (Colorspace names take precedence over roles.)
func (c *Config) ColorSpace(name string) (*ColorSpace, error) {
	c_str := C.CString(name)
	defer C.free(unsafe.Pointer(c_str))

	cs, err := C.Config_getColorSpace(c.ptr, c_str)
	if err != nil {
		if err2 := c.lastError(); err2 != nil {
			err = fmt.Errorf("%v: %v", err2, err)
		}
		err = fmt.Errorf("%q is not a valid ColorSpace: %v", name, err)
		return nil, err
	}
	if cs == nil {
		err = fmt.Errorf("%q is not a valid ColorSpace", name)
		return nil, err
	}
	runtime.KeepAlive(c)
	return newColorSpace(cs), err
}

func (c *Config) NumColorSpaces() int {
	num, err := C.Config_getNumColorSpaces(c.ptr)
	if err != nil {
		return 0
	}
	runtime.KeepAlive(c)
	return int(num)
}

// This will null if an invalid index is specified
func (c *Config) ColorSpaceNameByIndex(index int) (string, error) {
	name, err := C.Config_getColorSpaceNameByIndex(c.ptr, C.int(index))
	if err != nil {
		return "", c.lastError()
	}
	runtime.KeepAlive(c)
	return C.GoString(name), err
}

// Accepts either a color space OR role name. (Colorspace names take precedence over roles.)
func (c *Config) IndexForColorSpace(name string) (int, error) {
	c_str := C.CString(name)
	defer C.free(unsafe.Pointer(c_str))

	idx, err := C.Config_getIndexForColorSpace(c.ptr, c_str)
	if err != nil {
		return -1, c.lastError()
	}
	runtime.KeepAlive(c)
	return int(idx), err
}

// If another color space is already registered with the same name, this will overwrite it.
// This stores a copy of the specified color space.
func (c *Config) AddColorSpace(cs *ColorSpace) error {
	_, err := C.Config_addColorSpace(c.ptr, cs.ptr)
	runtime.KeepAlive(c)
	return err
}

func (c *Config) ClearColorSpaces() error {
	_, err := C.Config_clearColorSpaces(c.ptr)
	runtime.KeepAlive(c)
	return err
}

/*
Given the specified string, get the longest, right-most, colorspace substring that appears.

* If strict parsing is enabled, and no color space is found, return an empty string.
* If strict parsing is disabled, return ROLE_DEFAULT (if defined).
* If the default role is not defined, return an empty string.

*/
func (c *Config) ParseColorSpaceFromString(str string) (string, error) {
	c_str := C.CString(str)
	defer C.free(unsafe.Pointer(c_str))

	name, err := C.Config_parseColorSpaceFromString(c.ptr, c_str)
	if err != nil {
		return "", err
	}
	runtime.KeepAlive(c)
	return C.GoString(name), err
}

/*
Config Roles

A role is like an alias for a colorspace. You can query the colorspace
corresponding to a role using the normal getColorSpace fcn.
*/

// Setting the colorSpaceName name to a null string unsets it.
func (c *Config) SetRole(role, colorSpaceName string) error {
	c_role := C.CString(role)
	defer C.free(unsafe.Pointer(c_role))

	var c_space *C.char
	if colorSpaceName != "" {
		c_space = C.CString(colorSpaceName)
		defer C.free(unsafe.Pointer(c_space))
	}

	_, err := C.Config_setRole(c.ptr, c_role, c_space)
	runtime.KeepAlive(c)
	return err
}

func (c *Config) NumRoles() int {
	num, err := C.Config_getNumRoles(c.ptr)
	if err != nil {
		return 0
	}
	runtime.KeepAlive(c)
	return int(num)
}

// Return true if the role has been defined.
func (c *Config) HasRole(role string) bool {
	c_str := C.CString(role)
	defer C.free(unsafe.Pointer(c_str))

	has, err := C.Config_hasRole(c.ptr, c_str)
	if err != nil {
		return false
	}
	runtime.KeepAlive(c)
	return bool(has)
}

// Get the role name at index, this will return values like ‘scene_linear’,
// ‘compositing_log’. Return empty string if index is out of range.
func (c *Config) RoleName(index int) (string, error) {
	name, err := C.Config_getRoleName(c.ptr, C.int(index))
	if err != nil {
		return "", err
	}
	runtime.KeepAlive(c)
	return C.GoString(name), err
}

/*

Config Display/View Registration

*/

// Looks is a potentially comma (or colon) delimited list of lookNames,
// Where +/- prefixes are optionally allowed to denote forward/inverse
// look specification. (And forward is assumed in the absense of either)
func (c *Config) DisplayLooks(display, view string) string {
	c_disp := C.CString(display)
	c_view := C.CString(view)
	defer C.free(unsafe.Pointer(c_disp))
	defer C.free(unsafe.Pointer(c_view))

	ret := C.GoString(C.Config_getDisplayLooks(c.ptr, c_disp, c_view))
	runtime.KeepAlive(c)
	return ret
}

func (c *Config) DefaultDisplay() string {
	ret := C.GoString(C.Config_getDefaultDisplay(c.ptr))
	runtime.KeepAlive(c)
	return ret
}

func (c *Config) NumDisplays() int {
	ret := int(C.Config_getNumDisplays(c.ptr))
	runtime.KeepAlive(c)
	return ret
}

func (c *Config) Display(index int) string {
	ret := C.GoString(C.Config_getDisplay(c.ptr, C.int(index)))
	runtime.KeepAlive(c)
	return ret
}

func (c *Config) DefaultView(display string) string {
	c_disp := C.CString(display)
	defer C.free(unsafe.Pointer(c_disp))
	ret := C.GoString(C.Config_getDefaultView(c.ptr, c_disp))
	runtime.KeepAlive(c)
	return ret
}

func (c *Config) NumViews(display string) int {
	c_disp := C.CString(display)
	defer C.free(unsafe.Pointer(c_disp))
	ret := int(C.Config_getNumViews(c.ptr, c_disp))
	runtime.KeepAlive(c)
	return ret
}

func (c *Config) View(display string, index int) string {
	c_disp := C.CString(display)
	defer C.free(unsafe.Pointer(c_disp))
	ret := C.GoString(C.Config_getView(c.ptr, c_disp, C.int(index)))
	runtime.KeepAlive(c)
	return ret
}

func (c *Config) DisplayColorSpaceName(display, view string) string {
	c_disp := C.CString(display)
	c_view := C.CString(view)
	defer C.free(unsafe.Pointer(c_disp))
	defer C.free(unsafe.Pointer(c_view))

	ret := C.GoString(C.Config_getDisplayColorSpaceName(c.ptr, c_disp, c_view))
	runtime.KeepAlive(c)
	return ret
}

// For the (display,view) combination, specify which colorSpace and look to use.
// If a look is not desired, then just pass an empty string
func (c *Config) AddDisplay(display, view, colorSpace, looks string) error {
	c_disp := C.CString(display)
	c_view := C.CString(view)
	c_cs := C.CString(colorSpace)
	c_looks := C.CString(looks)
	defer C.free(unsafe.Pointer(c_disp))
	defer C.free(unsafe.Pointer(c_view))
	defer C.free(unsafe.Pointer(c_cs))
	defer C.free(unsafe.Pointer(c_looks))

	_, err := C.Config_addDisplay(c.ptr, c_disp, c_view, c_cs, c_looks)
	runtime.KeepAlive(c)
	return err
}

func (c *Config) ClearDisplays() {
	C.Config_clearDisplays(c.ptr)
	runtime.KeepAlive(c)
}

// Comma-delimited list of display names.
func (c *Config) SetActiveDisplays(displays string) error {
	c_disp := C.CString(displays)
	defer C.free(unsafe.Pointer(c_disp))
	_, err := C.Config_setActiveDisplays(c.ptr, c_disp)
	runtime.KeepAlive(c)
	return err
}

func (c *Config) ActiveDisplays() string {
	ret := C.GoString(C.Config_getActiveDisplays(c.ptr))
	runtime.KeepAlive(c)
	return ret
}

// Comma-delimited list of view names.
func (c *Config) SetActiveViews(views string) error {
	c_view := C.CString(views)
	defer C.free(unsafe.Pointer(c_view))
	_, err := C.Config_setActiveViews(c.ptr, c_view)
	runtime.KeepAlive(c)
	return err
}

func (c *Config) ActiveViews() string {
	ret := C.GoString(C.Config_getActiveViews(c.ptr))
	runtime.KeepAlive(c)
	return ret
}
