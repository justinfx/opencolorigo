package opencolorigo

// #include "stdlib.h"
//
// #include "ocio.h"
//
import "C"

import (
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

type ColorSpace struct {
    ptr unsafe.Pointer
}

func newColorSpace(p unsafe.Pointer) *ColorSpace {
    cfg := &ColorSpace{p}
    runtime.SetFinalizer(cfg, deleteColorspace)
    return cfg
}

func deleteColorspace(c *ColorSpace) { C.free(c.ptr) }

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
