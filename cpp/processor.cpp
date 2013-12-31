#include <OpenColorIO/OpenColorIO.h>

#include <iostream>

#include "ocio.h"


extern "C" {

    namespace OCIO = OCIO_NAMESPACE;

    Processor* Processor_Create() {
        return (Processor*) new OCIO::ProcessorRcPtr(OCIO::Processor::Create());
    }

    bool Processor_isNoOp(Processor *p) {
        return static_cast<OCIO::ConstProcessorRcPtr*>(p)->get()->isNoOp();
    }

    bool Processor_hasChannelCrosstalk(Processor *p) {
        return static_cast<OCIO::ConstProcessorRcPtr*>(p)->get()->hasChannelCrosstalk();
    }

    // Processor CPU
    void Processor_apply(Processor *p, ImageDesc *i) {
        OCIO::ImageDesc *img = static_cast<OCIO::ImageDesc*>(i);
        static_cast<OCIO::ConstProcessorRcPtr*>(p)->get()->apply(*img);
    }

    const char* Processor_getCpuCacheID(Processor *p) {
        return static_cast<OCIO::ConstProcessorRcPtr*>(p)->get()->getCpuCacheID();
    }

    // ProcessorMetadata
    ProcessorMetadata* Processor_getMetadata(Processor *p) {
        OCIO::ConstProcessorMetadataRcPtr ptr = static_cast<OCIO::ConstProcessorRcPtr*>(p)->get()->getMetadata();
        return (ProcessorMetadata*) new OCIO::ConstProcessorMetadataRcPtr(ptr);
    }

    ProcessorMetadata* ProcessorMetadata_Create() {
        return (ProcessorMetadata*) new OCIO::ProcessorMetadataRcPtr(OCIO::ProcessorMetadata::Create());
    }

    int ProcessorMetadata_getNumFiles(ProcessorMetadata *p) {
        return static_cast<OCIO::ConstProcessorMetadataRcPtr*>(p)->get()->getNumFiles();
    }

    const char* ProcessorMetadata_getFile(ProcessorMetadata *p, int index) {
        return static_cast<OCIO::ConstProcessorMetadataRcPtr*>(p)->get()->getFile(index);
    }

    int ProcessorMetadata_getNumLooks(ProcessorMetadata *p) {
        return static_cast<OCIO::ConstProcessorMetadataRcPtr*>(p)->get()->getNumLooks();
    }

    const char* ProcessorMetadata_getLook(ProcessorMetadata *p, int index) {
        return static_cast<OCIO::ConstProcessorMetadataRcPtr*>(p)->get()->getLook(index);
    }

    void ProcessorMetadata_addFile(ProcessorMetadata *p, const char* fname) {
        static_cast<OCIO::ProcessorMetadataRcPtr*>(p)->get()->addFile(fname);
    }

    void ProcessorMetadata_addLook(ProcessorMetadata *p, const char* look) {
        static_cast<OCIO::ProcessorMetadataRcPtr*>(p)->get()->addLook(look);
    }

    // ImageDesc 
    PackedImageDesc* PackedImageDesc_Create(float* data, long width, long height, long numChannels) {
        return (PackedImageDesc*) new OCIO::PackedImageDesc(data, width, height, numChannels);
    }

    float* PackedImageDesc_getData(PackedImageDesc *p) {
        return static_cast<OCIO::PackedImageDesc*>(p)->getData();
    }

    long PackedImageDesc_getWidth(PackedImageDesc *p) {
        return static_cast<OCIO::PackedImageDesc*>(p)->getWidth();
    }

    long PackedImageDesc_getHeight(PackedImageDesc *p) {
        return static_cast<OCIO::PackedImageDesc*>(p)->getHeight();
    }

    long PackedImageDesc_getNumChannels(PackedImageDesc *p) {
        return static_cast<OCIO::PackedImageDesc*>(p)->getNumChannels();
    }

}