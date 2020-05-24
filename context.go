package ocio

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
	ptr C.ContextId
}

func newContext(p C.ContextId) *Context {
	cfg := &Context{p}
	runtime.SetFinalizer(cfg, deleteContext)
	return cfg
}

func deleteContext(c *Context) {
	if c == nil {
		return
	}
	if c.ptr != nil {
		runtime.SetFinalizer(c, nil)
		C.deleteContext(c.ptr)
		c.ptr = nil
	}
	runtime.KeepAlive(c)
}

// Create a new empty Context
func NewContext() *Context {
	return newContext(C.Context_Create())
}

func (c *Context) lastError() error {
	if c == nil {
		return nil
	}
	err := getLastError(c.ptr)
	runtime.KeepAlive(c)
	return err
}

// Destroy immediately frees resources for this
// instance instead of waiting for garbage collection
// finalizer to run at some point later
func (c *Context) Destroy() {
	deleteContext(c)
}

// Create a new editable copy of this Context
func (c *Context) EditableCopy() *Context {
	ret := newContext(C.Context_createEditableCopy(c.ptr))
	runtime.KeepAlive(c)
	return ret
}

func (c *Context) CacheID() (string, error) {
	id := C.Context_getCacheID(c.ptr)
	if err := c.lastError(); err != nil {
		return "", err
	}
	runtime.KeepAlive(c)
	return C.GoString(id), nil
}

func (c *Context) SetSearchPath(path string) {
	c_str := C.CString(path)
	defer C.free(unsafe.Pointer(c_str))
	C.Context_setSearchPath(c.ptr, c_str)
	runtime.KeepAlive(c)
}

func (c *Context) SearchPath() string {
	ret := C.GoString(C.Context_getSearchPath(c.ptr))
	runtime.KeepAlive(c)
	return ret
}

func (c *Context) SetWorkingDir(dirname string) {
	c_str := C.CString(dirname)
	defer C.free(unsafe.Pointer(c_str))
	C.Context_setWorkingDir(c.ptr, c_str)
	runtime.KeepAlive(c)
}

func (c *Context) WorkingDir() string {
	ret := C.GoString(C.Context_getWorkingDir(c.ptr))
	runtime.KeepAlive(c)
	return ret
}

func (c *Context) SetStringVar(name, value string) {
	c_name := C.CString(name)
	c_val := C.CString(value)
	defer C.free(unsafe.Pointer(c_name))
	defer C.free(unsafe.Pointer(c_val))

	C.Context_setStringVar(c.ptr, c_name, c_val)
	runtime.KeepAlive(c)
}

func (c *Context) StringVar(name string) string {
	c_name := C.CString(name)
	defer C.free(unsafe.Pointer(c_name))
	ret := C.GoString(C.Context_getStringVar(c.ptr, c_name))
	runtime.KeepAlive(c)
	return ret
}

func (c *Context) EnvironmentMode() EnvironmentMode {
	ret := C.Context_getEnvironmentMode(c.ptr)
	runtime.KeepAlive(c)
	return EnvironmentMode(ret)
}

func (c *Context) SetEnvironmentMode(mode EnvironmentMode) {
	C.Context_setEnvironmentMode(c.ptr, C.EnvironmentMode(mode))
	runtime.KeepAlive(c)
}

// Seed all string vars with the current environment.
func (c *Context) LoadEnvironment() {
	C.Context_loadEnvironment(c.ptr)
	runtime.KeepAlive(c)
}

// Do a file lookup.
// Evaluate the specified variable (as needed).
func (c *Context) ResolveStringVar(val string) string {
	c_name := C.CString(val)
	defer C.free(unsafe.Pointer(c_name))
	ret := C.GoString(C.Context_resolveStringVar(c.ptr, c_name))
	runtime.KeepAlive(c)
	return ret
}

// Do a file lookup.
// Evaluate all variables (as needed). Also, walk the full search
// path until the file is found.
// If the filename cannot be found, an error will be returned.
func (c *Context) ResolveFileLocation(filename string) (string, error) {
	c_name := C.CString(filename)
	defer C.free(unsafe.Pointer(c_name))
	val := C.Context_resolveFileLocation(c.ptr, c_name)
	if err := c.lastError(); err != nil {
		return "", err
	}
	runtime.KeepAlive(c)
	return C.GoString(val), nil
}
