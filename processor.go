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
    ptr unsafe.Pointer
}

func newProcessor(p unsafe.Pointer) *Processor {
    cfg := &Processor{p}
    runtime.SetFinalizer(cfg, deleteProcessor)
    return cfg
}

func deleteProcessor(c *Processor) { C.free(c.ptr) }

// Create a new empty Processor
func NewProcessor() *Processor {
    return newProcessor(C.Processor_Create())
}

func (p *Processor) IsNoOp() bool {
    return bool(C.Processor_isNoOp(p.ptr))
}

// does the processor represent an image transformation that
// introduces crosstalk between the image channels
func (p *Processor) HasChannelCrosstalk() bool {
    return bool(C.Processor_hasChannelCrosstalk(p.ptr))
}

func (p *Processor) Metadata() *ProcessorMetadata {
    return newProcessorMetadata(C.Processor_getMetadata(p.ptr))
}

/* Processor CPU */

// Apply to an image.
func (p *Processor) Apply(i ImageDescriptor) error {
    _, err := C.Processor_apply(p.ptr, unsafe.Pointer(i.imageDescPtr()))
    return err
}

func (p *Processor) CpuCacheID() (string, error) {
    id, err := C.Processor_getCpuCacheID(p.ptr)
    if err != nil {
        return "", err
    }
    return C.GoString(id), err
}

// This class contains meta information about the process that generated this processor.
// The results of these functions do not impact the pixel processing.
type ProcessorMetadata struct {
    ptr unsafe.Pointer
}

func newProcessorMetadata(p unsafe.Pointer) *ProcessorMetadata {
    cfg := &ProcessorMetadata{p}
    runtime.SetFinalizer(cfg, deleteProcessorMetadata)
    return cfg
}

func deleteProcessorMetadata(c *ProcessorMetadata) { C.free(c.ptr) }

// Create a new empty ProcessorMetadata
func NewProcessorMetadata() *ProcessorMetadata {
    return newProcessorMetadata(C.ProcessorMetadata_Create())
}

func (p *ProcessorMetadata) NumFiles() int {
    return int(C.ProcessorMetadata_getNumFiles(p.ptr))
}

func (p *ProcessorMetadata) File(index int) string {
    return C.GoString(C.ProcessorMetadata_getFile(p.ptr, C.int(index)))
}

func (p *ProcessorMetadata) NumLooks() int {
    return int(C.ProcessorMetadata_getNumLooks(p.ptr))
}

func (p *ProcessorMetadata) Look(index int) string {
    return C.GoString(C.ProcessorMetadata_getLook(p.ptr, C.int(index)))
}

func (p *ProcessorMetadata) AddFile(fileName string) {
    c_str := C.CString(fileName)
    defer C.free(unsafe.Pointer(c_str))

    C.ProcessorMetadata_addFile(p.ptr, c_str)
}

func (p *ProcessorMetadata) AddLook(lookName string) {
    c_str := C.CString(lookName)
    defer C.free(unsafe.Pointer(c_str))

    C.ProcessorMetadata_addLook(p.ptr, c_str)
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
    runtime.SetFinalizer(i, func(c *PackedImageDesc) { C.free(c.ptr) })
    return i
}

// Create a PackedImageDesc wrapper for some raw rgb data
// Pass a []float32 slice of image data: rgbrgbrgb, etc.
// The number of channels must be greater than or equal to 3 If a 4th channel is specified,
// it is assumed to be alpha information. Channels > 4 will be ignored.
func NewPackedImageDesc(rgb ColorData, width, height, numChannels int) *PackedImageDesc {
    ptr := C.PackedImageDesc_Create((*C.float)(&rgb[0]), C.long(width), C.long(height), C.long(numChannels))
    return newPackedImageDesc(ptr, rgb)
}

// Return the current state of the image data,
// potentially modified if color transformations
// have been applied.
func (p *PackedImageDesc) Data() ColorData {
    return p.data
}

// Pixel width of the image
func (p *PackedImageDesc) Width() int {
    return int(C.PackedImageDesc_getWidth(p.ptr))
}

// Pixel height of the image
func (p *PackedImageDesc) Height() int {
    return int(C.PackedImageDesc_getHeight(p.ptr))
}

// Number of color channels in the image
func (p *PackedImageDesc) NumChannels() int {
    return int(C.PackedImageDesc_getNumChannels(p.ptr))
}

func (p *PackedImageDesc) imageDescPtr() unsafe.Pointer {
    return p.ptr
}
