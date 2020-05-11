package ocio

/*
#include "stdlib.h"

#include "cpp/ocio.h"
*/
import "C"

import (
	"runtime"
	"unsafe"
)

type TransformDirection int

const (
	TRANSFORM_DIR_UNKNOWN TransformDirection = C.TRANSFORM_DIR_UNKNOWN
	TRANSFORM_DIR_FORWARD TransformDirection = C.TRANSFORM_DIR_FORWARD
	TRANSFORM_DIR_INVERSE TransformDirection = C.TRANSFORM_DIR_INVERSE
)

// Transform interface represents the different types of
// concrete transform types that can be passed into API calls
type Transform interface {
	transformHandle() unsafe.Pointer
}

type DisplayTransform struct {
	ptr unsafe.Pointer
}

func newDisplayTransform(p unsafe.Pointer) *DisplayTransform {
	tx := &DisplayTransform{p}
	runtime.SetFinalizer(tx, deleteDisplayTransform)
	return tx
}

func deleteDisplayTransform(tx *DisplayTransform) { C.free(tx.ptr) }

// Create a new empty DisplayTransform
func NewDisplayTransform() *DisplayTransform {
	return newDisplayTransform(C.DisplayTransform_Create())
}

func (tx *DisplayTransform) transformHandle() unsafe.Pointer {
	return tx.ptr
}

// Create a new editable copy of this DisplayTransform
func (tx *DisplayTransform) EditableCopy() *DisplayTransform {
	cpy := newDisplayTransform(C.DisplayTransform_createEditableCopy(tx.ptr))
	runtime.KeepAlive(tx)
	return cpy
}

func (tx *DisplayTransform) Direction() TransformDirection {
	dir := TransformDirection(C.DisplayTransform_getDirection(tx.ptr))
	runtime.KeepAlive(tx)
	return dir
}

func (tx *DisplayTransform) SetDirection(dir TransformDirection) {
	C.DisplayTransform_setDirection(tx.ptr, C.TransformDirection(dir))
	runtime.KeepAlive(tx)
}

// InputColorSpace returns the incoming color space
func (tx *DisplayTransform) InputColorSpace() string {
	cs := C.GoString(C.DisplayTransform_getInputColorSpaceName(tx.ptr))
	runtime.KeepAlive(tx)
	return cs
}

// SetInputColorSpace sets the incoming color space
func (tx *DisplayTransform) SetInputColorSpace(cs string) {
	c_str := C.CString(cs)
	defer C.free(unsafe.Pointer(c_str))
	C.DisplayTransform_setInputColorSpaceName(tx.ptr, c_str)
	runtime.KeepAlive(tx)
	runtime.KeepAlive(c_str)
}

// Display returns the output display transform.
func (tx *DisplayTransform) Display() string {
	name := C.GoString(C.DisplayTransform_getDisplay(tx.ptr))
	runtime.KeepAlive(tx)
	return name
}

// SetDisplay sets the output display transform.
// This is controlled by the specification of (display, view).
func (tx *DisplayTransform) SetDisplay(name string) {
	c_str := C.CString(name)
	defer C.free(unsafe.Pointer(c_str))
	C.DisplayTransform_setDisplay(tx.ptr, c_str)
	runtime.KeepAlive(tx)
	runtime.KeepAlive(c_str)
}

// View returns the view transform to use
func (tx *DisplayTransform) View() string {
	name := C.GoString(C.DisplayTransform_getView(tx.ptr))
	runtime.KeepAlive(tx)
	return name
}

// SetView: Specify which view transform to use
func (tx *DisplayTransform) SetView(name string) {
	c_str := C.CString(name)
	defer C.free(unsafe.Pointer(c_str))
	C.DisplayTransform_setView(tx.ptr, c_str)
	runtime.KeepAlive(tx)
	runtime.KeepAlive(c_str)
}

func (tx *DisplayTransform) LooksOverride() string {
	looks := C.GoString(C.DisplayTransform_getLooksOverride(tx.ptr))
	runtime.KeepAlive(tx)
	return looks
}

/*
A user can optionally override the looks that are, by default,
used with the expected display / view combination. A common
use case for this functionality is in an image viewing app,
where per-shot looks are supported. If for some reason a per-shot
look is not defined for the current Context, the Config.Processor()
function will not succeed by default. Thus, with this mechanism the
viewing app could override to looks = "", and this will allow image
display to continue (though hopefully) the interface would reflect
this fallback option).

Looks is a potentially comma (or colon) delimited list of lookNames,
where +/- prefixes are optionally allowed to denote forward/inverse
look specification (And forward is assumed in the absence of either).
*/
func (tx *DisplayTransform) SetLooksOverride(looks string) {
	c_str := C.CString(looks)

	defer C.free(unsafe.Pointer(c_str))
	C.DisplayTransform_setLooksOverride(tx.ptr, c_str)
	runtime.KeepAlive(tx)
	runtime.KeepAlive(c_str)
}

func (tx *DisplayTransform) LooksOverrideEnabled() bool {
	enabled := C.DisplayTransform_getLooksOverrideEnabled(tx.ptr)
	runtime.KeepAlive(tx)
	return bool(enabled)
}

// SetLooksOverrideEnabled specifies whether the lookOverride should
// be used or not. This is a separate flag, as it's often useful to
// override "looks" to an empty string
func (tx *DisplayTransform) SetLooksOverrideEnabled(enabled bool) {
	C.DisplayTransform_setLooksOverrideEnabled(tx.ptr, C.bool(enabled))
	runtime.KeepAlive(tx)
}
