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

/* Processor */

type Processor struct {
	ptr C.ProcessorId
}

func newProcessor(p C.ProcessorId) *Processor {
	cfg := &Processor{p}
	runtime.SetFinalizer(cfg, deleteProcessor)
	return cfg
}

func deleteProcessor(p *Processor) {
	if p == nil {
		return
	}
	if p.ptr != nil {
		runtime.SetFinalizer(p, nil)
		C.deleteProcessor(p.ptr)
		p.ptr = nil
	}
	runtime.KeepAlive(p)
}

// Create a new empty Processor
func NewProcessor() *Processor {
	return newProcessor(C.Processor_Create())
}

func (p *Processor) lastError(errno ...error) error {
	if p == nil {
		return nil
	}
	err := getLastError(p.ptr, errno...)
	runtime.KeepAlive(p)
	return err
}

// Destroy immediately frees resources for this
// instance instead of waiting for garbage collection
// finalizer to run at some point later
func (p *Processor) Destroy() {
	deleteProcessor(p)
}

func (p *Processor) IsNoOp() bool {
	ret := bool(C.Processor_isNoOp(p.ptr))
	runtime.KeepAlive(p)
	return ret
}

// does the processor represent an image transformation that
// introduces crosstalk between the image channels
func (p *Processor) HasChannelCrosstalk() bool {
	ret := bool(C.Processor_hasChannelCrosstalk(p.ptr))
	runtime.KeepAlive(p)
	return ret
}

func (p *Processor) Metadata() *ProcessorMetadata {
	return newProcessorMetadata(C.Processor_getMetadata(p.ptr))
}

/* Processor CPU */

// Apply to an image.
func (p *Processor) Apply(i ImageDescriptor) error {
	_, err := C.Processor_apply(p.ptr, unsafe.Pointer(i.imageDescPtr()))
	err = p.lastError(err)
	runtime.KeepAlive(p)
	runtime.KeepAlive(i)
	return err
}

func (p *Processor) CpuCacheID() (string, error) {
	id, err := C.Processor_getCpuCacheID(p.ptr)
	if err = p.lastError(err); err != nil {
		return "", err
	}
	runtime.KeepAlive(p)
	return C.GoString(id), nil
}

// This class contains meta information about the process that generated this processor.
// The results of these functions do not impact the pixel processing.
type ProcessorMetadata struct {
	ptr C.ProcessorMetadataId
}

func newProcessorMetadata(p C.ProcessorMetadataId) *ProcessorMetadata {
	cfg := &ProcessorMetadata{p}
	runtime.SetFinalizer(cfg, deleteProcessorMetadata)
	return cfg
}

func deleteProcessorMetadata(c *ProcessorMetadata) {
	if c == nil {
		return
	}
	if c.ptr != 0 {
		runtime.SetFinalizer(c, nil)
		C.deleteProcessorMetadata(c.ptr)
		c.ptr = 0
	}
	runtime.KeepAlive(c)
}

// Create a new empty ProcessorMetadata
func NewProcessorMetadata() *ProcessorMetadata {
	return newProcessorMetadata(C.ProcessorMetadata_Create())
}

// Destroy immediately frees resources for this
// instance instead of waiting for garbage collection
// finalizer to run at some point later
func (p *ProcessorMetadata) Destroy() {
	deleteProcessorMetadata(p)
}

func (p *ProcessorMetadata) NumFiles() int {
	ret := int(C.ProcessorMetadata_getNumFiles(p.ptr))
	runtime.KeepAlive(p)
	return ret
}

func (p *ProcessorMetadata) File(index int) string {
	ret := C.GoString(C.ProcessorMetadata_getFile(p.ptr, C.int(index)))
	runtime.KeepAlive(p)
	return ret
}

// Files is a helper to return a slice of the file paths,
// using NumFiles and File(index)
func (p *ProcessorMetadata) Files() []string {
	num := p.NumFiles()
	files := make([]string, 0, num)
	for i := 0; i < num; i++ {
		files = append(files, p.File(i))
	}
	return files
}

func (p *ProcessorMetadata) NumLooks() int {
	ret := int(C.ProcessorMetadata_getNumLooks(p.ptr))
	runtime.KeepAlive(p)
	return ret
}

func (p *ProcessorMetadata) Look(index int) string {
	ret := C.GoString(C.ProcessorMetadata_getLook(p.ptr, C.int(index)))
	runtime.KeepAlive(p)
	return ret
}

func (p *ProcessorMetadata) AddFile(fileName string) {
	c_str := C.CString(fileName)
	defer C.free(unsafe.Pointer(c_str))

	C.ProcessorMetadata_addFile(p.ptr, c_str)
	runtime.KeepAlive(p)
}

func (p *ProcessorMetadata) AddLook(lookName string) {
	c_str := C.CString(lookName)
	defer C.free(unsafe.Pointer(c_str))

	C.ProcessorMetadata_addLook(p.ptr, c_str)
	runtime.KeepAlive(p)
}

/* ImageDesc */

// A ColorData meant to hold floating point pixel data in forms:
// * RGBRGB...
// * RGBARGBA...
type ColorData []float32

type ImageDescriptor interface {
	imageDescPtr() unsafe.Pointer
}

// This is a light-weight wrapper around an image, that provides a context for pixel access
type PackedImageDesc struct {
	ptr  unsafe.Pointer
	data ColorData
}

func newPackedImageDesc(p unsafe.Pointer, data ColorData) *PackedImageDesc {
	i := &PackedImageDesc{p, data}
	runtime.SetFinalizer(i, deletePackedImageDesc)
	return i
}

func deletePackedImageDesc(p *PackedImageDesc) {
	if p == nil {
		return
	}
	if p.ptr != nil {
		runtime.SetFinalizer(p, nil)
		C.deletePackedImageDesc(p.ptr)
		p.ptr = nil
	}
	runtime.KeepAlive(p)
}

// Create a PackedImageDesc wrapper for some raw rgb data
// Pass a []float32 slice of image data: rgbrgbrgb, etc.
// The number of channels must be greater than or equal to 3 If a 4th channel is specified,
// it is assumed to be alpha information. Channels > 4 will be ignored.
func NewPackedImageDesc(rgb ColorData, width, height, numChannels int) *PackedImageDesc {
	ptr := C.PackedImageDesc_Create((*C.float)(&rgb[0]), C.long(width), C.long(height), C.long(numChannels))
	return newPackedImageDesc(ptr, rgb)
}

// Destroy immediately frees resources for this
// instance instead of waiting for garbage collection
// finalizer to run at some point later
func (p *PackedImageDesc) Destroy() {
	deletePackedImageDesc(p)
}

// Return the current state of the image data,
// potentially modified if color transformations
// have been applied.
func (p *PackedImageDesc) Data() ColorData {
	return p.data
}

// Pixel width of the image
func (p *PackedImageDesc) Width() int {
	ret := int(C.PackedImageDesc_getWidth(p.ptr))
	runtime.KeepAlive(p)
	return ret
}

// Pixel height of the image
func (p *PackedImageDesc) Height() int {
	ret := int(C.PackedImageDesc_getHeight(p.ptr))
	runtime.KeepAlive(p)
	return ret
}

// Number of color channels in the image
func (p *PackedImageDesc) NumChannels() int {
	ret := int(C.PackedImageDesc_getNumChannels(p.ptr))
	runtime.KeepAlive(p)
	return ret
}

func (p *PackedImageDesc) imageDescPtr() unsafe.Pointer {
	return p.ptr
}
