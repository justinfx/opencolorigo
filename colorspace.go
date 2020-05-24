package ocio

// #include "stdlib.h"
//
// #include "ocio.h"
//
import "C"

import (
	"fmt"
	"runtime"
	"unsafe"
)

type BitDepth int

const (
	BIT_DEPTH_UNKNOWN BitDepth = C.BIT_DEPTH_UNKNOWN
	BIT_DEPTH_UINT8   BitDepth = C.BIT_DEPTH_UINT8
	BIT_DEPTH_UINT10  BitDepth = C.BIT_DEPTH_UINT10
	BIT_DEPTH_UINT12  BitDepth = C.BIT_DEPTH_UINT12
	BIT_DEPTH_UINT14  BitDepth = C.BIT_DEPTH_UINT14
	BIT_DEPTH_UINT16  BitDepth = C.BIT_DEPTH_UINT16
	BIT_DEPTH_UINT32  BitDepth = C.BIT_DEPTH_UINT32
	BIT_DEPTH_F16     BitDepth = C.BIT_DEPTH_F16
	BIT_DEPTH_F32     BitDepth = C.BIT_DEPTH_F32
)

/*
The ColorSpace is the state of an image with respect to colorimetry and color encoding.
Transforming images between different ColorSpaces is the primary motivation for this library.

While a complete discussion of colorspaces is beyond the scope of documentation,
traditional uses would be to have ColorSpaces corresponding to: physical capture devices
(known cameras, scanners), and internal ‘convenience’ spaces (such as scene linear, logarithmic).

ColorSpaces are specific to a particular image precision (float32, uint8, etc.), and the set
of ColorSpaces that provide equivalent mappings (at different precisions) are referred to as a ‘family’.
*/
type ColorSpace struct {
	ptr C.ColorSpaceId
}

func newColorSpace(p C.ColorSpaceId) *ColorSpace {
	cfg := &ColorSpace{p}
	runtime.SetFinalizer(cfg, deleteColorspace)
	return cfg
}

func deleteColorspace(c *ColorSpace) {
	if c == nil {
		return
	}
	if c.ptr != 0 {
		runtime.SetFinalizer(c, nil)
		C.deleteColorSpace(c.ptr)
		c.ptr = 0
	}
	runtime.KeepAlive(c)
}

// Create a new empty ColorSpace
func NewColorSpace() *ColorSpace {
	return newColorSpace(C.ColorSpace_Create())
}

// Destroy immediately frees resources for this
// instance instead of waiting for garbage collection
// finalizer to run at some point later
func (c *ColorSpace) Destroy() {
	deleteColorspace(c)
}

func (c *ColorSpace) String() string {
	name := ""
	if c.ptr != 0 {
		name = c.Name()
	}
	return fmt.Sprintf("ColorSpace: %q", name)
}

// Create a new editable copy of this ColorSpace
func (c *ColorSpace) EditableCopy() *ColorSpace {
	ret := newColorSpace(C.ColorSpace_createEditableCopy(c.ptr))
	runtime.KeepAlive(c)
	return ret
}

func (c *ColorSpace) Name() string {
	ret := C.GoString(C.ColorSpace_getName(c.ptr))
	runtime.KeepAlive(c)
	return ret
}

func (c *ColorSpace) SetName(name string) {
	c_str := C.CString(name)
	defer C.free(unsafe.Pointer(c_str))
	C.ColorSpace_setName(c.ptr, c_str)
	runtime.KeepAlive(c)
}

func (c *ColorSpace) Family() string {
	ret := C.GoString(C.ColorSpace_getFamily(c.ptr))
	runtime.KeepAlive(c)
	return ret
}

func (c *ColorSpace) SetFamily(family string) {
	c_str := C.CString(family)
	defer C.free(unsafe.Pointer(c_str))
	C.ColorSpace_setFamily(c.ptr, c_str)
	runtime.KeepAlive(c)
}

// Get the ColorSpace group name (used for equality comparisons)
// This allows no-op transforms between different colorspaces.
// If an equalityGroup is not defined (an empty string), it will be
// considered unique (i.e., it will not compare as equal to other
// ColorSpaces with an empty equality group).
// This is often, though not always, set to the same value as ‘family’.
func (c *ColorSpace) EqualityGroup() string {
	ret := C.GoString(C.ColorSpace_getEqualityGroup(c.ptr))
	runtime.KeepAlive(c)
	return ret
}

func (c *ColorSpace) SetEqualityGroup(group string) {
	c_str := C.CString(group)
	defer C.free(unsafe.Pointer(c_str))
	C.ColorSpace_setEqualityGroup(c.ptr, c_str)
}

func (c *ColorSpace) Description() string {
	return C.GoString(C.ColorSpace_getDescription(c.ptr))
}

func (c *ColorSpace) SetDescription(description string) {
	c_str := C.CString(description)
	defer C.free(unsafe.Pointer(c_str))
	C.ColorSpace_setDescription(c.ptr, c_str)
	runtime.KeepAlive(c)
}

func (c *ColorSpace) BitDepth() BitDepth {
	ret := BitDepth(C.ColorSpace_getBitDepth(c.ptr))
	runtime.KeepAlive(c)
	return ret
}

func (c *ColorSpace) SetBitDepth(bitDepth BitDepth) {
	C.ColorSpace_setBitDepth(c.ptr, C.BitDepth(bitDepth))
	runtime.KeepAlive(c)
}
