#include <OpenColorIO/OpenColorIO.h>

#include "ocio.h"


extern "C" {

    namespace OCIO = OCIO_NAMESPACE;

    void deleteProcessor(Processor* p) {
        if (p != NULL) {
            delete (OCIO::ProcessorRcPtr*)p;
        }
    }

    Processor* Processor_Create() {
        OCIO::ProcessorRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = OCIO::Processor::Create();
        END_CATCH_ERR
        return (Processor*) new OCIO::ProcessorRcPtr(ptr);
    }

    bool Processor_isNoOp(Processor *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstProcessorRcPtr*>(p)->get()->isNoOp();
        END_CATCH_ERR
    }

    bool Processor_hasChannelCrosstalk(Processor *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstProcessorRcPtr*>(p)->get()->hasChannelCrosstalk();
        END_CATCH_ERR
    }

    // Processor CPU
    void Processor_apply(Processor *p, ImageDesc *i) {
        OCIO::ImageDesc *img = static_cast<OCIO::ImageDesc*>(i);
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConstProcessorRcPtr*>(p)->get()->apply(*img);
        END_CATCH_ERR
    }

    const char* Processor_getCpuCacheID(Processor *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstProcessorRcPtr*>(p)->get()->getCpuCacheID();
        END_CATCH_ERR
    }

    // ProcessorMetadata
    void deleteProcessorMetadata(ProcessorMetadata* p) {
        if (p != NULL) {
            delete (OCIO::ProcessorMetadataRcPtr*)p;
        }
    }

    ProcessorMetadata* Processor_getMetadata(Processor *p) {
        OCIO::ConstProcessorMetadataRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = static_cast<OCIO::ConstProcessorRcPtr*>(p)->get()->getMetadata();
        END_CATCH_ERR
        return (ProcessorMetadata*) new OCIO::ConstProcessorMetadataRcPtr(ptr);
    }

    ProcessorMetadata* ProcessorMetadata_Create() {
        OCIO::ProcessorMetadataRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = OCIO::ProcessorMetadata::Create();
        END_CATCH_ERR
        return (ProcessorMetadata*) new OCIO::ProcessorMetadataRcPtr(ptr);
    }

    int ProcessorMetadata_getNumFiles(ProcessorMetadata *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstProcessorMetadataRcPtr*>(p)->get()->getNumFiles();
        END_CATCH_ERR
    }

    const char* ProcessorMetadata_getFile(ProcessorMetadata *p, int index) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstProcessorMetadataRcPtr*>(p)->get()->getFile(index);
        END_CATCH_ERR
    }

    int ProcessorMetadata_getNumLooks(ProcessorMetadata *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstProcessorMetadataRcPtr*>(p)->get()->getNumLooks();
        END_CATCH_ERR
    }

    const char* ProcessorMetadata_getLook(ProcessorMetadata *p, int index) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstProcessorMetadataRcPtr*>(p)->get()->getLook(index);
        END_CATCH_ERR
    }

    void ProcessorMetadata_addFile(ProcessorMetadata *p, const char* fname) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ProcessorMetadataRcPtr*>(p)->get()->addFile(fname);
        END_CATCH_ERR
    }

    void ProcessorMetadata_addLook(ProcessorMetadata *p, const char* look) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ProcessorMetadataRcPtr*>(p)->get()->addLook(look);
        END_CATCH_ERR
    }

    // ImageDesc
    void deletePackedImageDesc(PackedImageDesc* p) {
        if (p != NULL) {
            delete (OCIO::PackedImageDesc*)p;
        }
    }

    PackedImageDesc* PackedImageDesc_Create(float* data, long width, long height, long numChannels) {
        BEGIN_CATCH_ERR
        return (PackedImageDesc*) new OCIO::PackedImageDesc(data, width, height, numChannels);
        END_CATCH_ERR
    }

    float* PackedImageDesc_getData(PackedImageDesc *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::PackedImageDesc*>(p)->getData();
        END_CATCH_ERR
    }

    long PackedImageDesc_getWidth(PackedImageDesc *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::PackedImageDesc*>(p)->getWidth();
        END_CATCH_ERR
    }

    long PackedImageDesc_getHeight(PackedImageDesc *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::PackedImageDesc*>(p)->getHeight();
        END_CATCH_ERR
    }

    long PackedImageDesc_getNumChannels(PackedImageDesc *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::PackedImageDesc*>(p)->getNumChannels();
        END_CATCH_ERR
    }

}