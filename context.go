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

type Context struct {
    ptr unsafe.Pointer
}

func newContext(p unsafe.Pointer) *Context {
    cfg := &Context{p}
    runtime.SetFinalizer(cfg, deleteContext)
    return cfg
}

func deleteContext(c *Context) { C.free(c.ptr) }

// Create a new empty Context
func NewContext() *Context {
    return newContext(C.Context_Create())
}

// Create a new editable copy of this Context
func (c *Context) EditableCopy() *Context {
    return newContext(C.Context_createEditableCopy(c.ptr))
}

func (c *Context) CacheID() (string, error) {
    id, err := C.Context_getCacheID(c.ptr)
    if err != nil {
        return "", err
    }
    return C.GoString(id), err
}

func (c *Context) SetSearchPath(path string) {
    c_str := C.CString(path)
    defer C.free(unsafe.Pointer(c_str))
    C.Context_setSearchPath(c.ptr, c_str)
}

func (c *Context) SearchPath() string {
    return C.GoString(C.Context_getSearchPath(c.ptr))
}

func (c *Context) SetWorkingDir(dirname string) {
    c_str := C.CString(dirname)
    defer C.free(unsafe.Pointer(c_str))
    C.Context_setWorkingDir(c.ptr, c_str)
}

func (c *Context) WorkingDir() string {
    return C.GoString(C.Context_getWorkingDir(c.ptr))
}

func (c *Context) SetStringVar(name, value string) {
    c_name := C.CString(name)
    c_val := C.CString(value)
    defer C.free(unsafe.Pointer(c_name))
    defer C.free(unsafe.Pointer(c_val))

    C.Context_setStringVar(c.ptr, c_name, c_val)
}

func (c *Context) StringVar(name string) string {
    c_name := C.CString(name)
    defer C.free(unsafe.Pointer(c_name))
    return C.GoString(C.Context_getStringVar(c.ptr, c_name))
}

// Seed all string vars with the current environment.
func (c *Context) LoadEnvironment() {
    C.Context_loadEnvironment(c.ptr)
}

// Do a file lookup.
// Evaluate the specified variable (as needed).
func (c *Context) ResolveStringVar(val string) string {
    c_name := C.CString(val)
    defer C.free(unsafe.Pointer(c_name))
    return C.GoString(C.Context_resolveStringVar(c.ptr, c_name))
}

// Do a file lookup.
// Evaluate all variables (as needed). Also, walk the full search
// path until the file is found.
// If the filename cannot be found, an error will be returned.
func (c *Context) ResolveFileLocation(filename string) (string, error) {
    c_name := C.CString(filename)
    defer C.free(unsafe.Pointer(c_name))
    val, err := C.Context_resolveFileLocation(c.ptr, c_name)
    if err != nil {
        return "", err
    }
    return C.GoString(val), err
}
