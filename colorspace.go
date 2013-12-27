package opencolorigo

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

const (
    BIT_DEPTH_UNKNOWN = C.BIT_DEPTH_UNKNOWN
    BIT_DEPTH_UINT8   = C.BIT_DEPTH_UINT8
    BIT_DEPTH_UINT10  = C.BIT_DEPTH_UINT10
    BIT_DEPTH_UINT12  = C.BIT_DEPTH_UINT12
    BIT_DEPTH_UINT14  = C.BIT_DEPTH_UINT14
    BIT_DEPTH_UINT16  = C.BIT_DEPTH_UINT16
    BIT_DEPTH_UINT32  = C.BIT_DEPTH_UINT32
    BIT_DEPTH_F16     = C.BIT_DEPTH_F16
    BIT_DEPTH_F32     = C.BIT_DEPTH_F32
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
    ptr unsafe.Pointer
}

func newColorSpace(p unsafe.Pointer) *ColorSpace {
    cfg := &ColorSpace{p}
    runtime.SetFinalizer(cfg, deleteColorspace)
    return cfg
}

func deleteColorspace(c *ColorSpace) { C.free(c.ptr) }

// Create a new empty ColorSpace
func NewColorSpace() *ColorSpace {
    return newColorSpace(C.ColorSpace_Create())
}

func (c *ColorSpace) String() string {
    name := ""
    if c.ptr != nil {
        name = c.Name()
    }
    return fmt.Sprintf("ColorSpace: %q", name)
}

// Create a new editable copy of this ColorSpace
func (c *ColorSpace) EditableCopy() *ColorSpace {
    return newColorSpace(C.ColorSpace_createEditableCopy(c.ptr))
}

func (c *ColorSpace) Name() string {
    return C.GoString(C.ColorSpace_getName(c.ptr))
}

func (c *ColorSpace) SetName(name string) {
    c_str := C.CString(name)
    defer C.free(unsafe.Pointer(c_str))
    C.ColorSpace_setName(c.ptr, c_str)
}

func (c *ColorSpace) Family() string {
    return C.GoString(C.ColorSpace_getFamily(c.ptr))
}

func (c *ColorSpace) SetFamily(family string) {
    c_str := C.CString(family)
    defer C.free(unsafe.Pointer(c_str))
    C.ColorSpace_setFamily(c.ptr, c_str)
}

// Get the ColorSpace group name (used for equality comparisons)
// This allows no-op transforms between different colorspaces.
// If an equalityGroup is not defined (an empty string), it will be
// considered unique (i.e., it will not compare as equal to other
// ColorSpaces with an empty equality group).
// This is often, though not always, set to the same value as ‘family’.
func (c *ColorSpace) EqualityGroup() string {
    return C.GoString(C.ColorSpace_getEqualityGroup(c.ptr))
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
}

func (c *ColorSpace) BitDepth() int {
    return int(C.ColorSpace_getBitDepth(c.ptr))
}

func (c *ColorSpace) SetBitDepth(bitDepth int) {
    C.ColorSpace_setBitDepth(c.ptr, C.BitDepth(bitDepth))
}
