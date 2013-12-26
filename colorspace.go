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

type ColorSpace struct {
    ptr unsafe.Pointer
}

func newColorSpace(p unsafe.Pointer) *ColorSpace {
    cfg := &ColorSpace{p}
    runtime.SetFinalizer(cfg, deleteColorspace)
    return cfg
}

func deleteColorspace(c *ColorSpace) {
    C.free(c.ptr)
}
