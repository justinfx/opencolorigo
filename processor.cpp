#include <OpenColorIO/OpenColorIO.h>

#include "ocio.h"
#include "ocio_abi.h"
#include "storage.h"

namespace OCIO = OCIO_NAMESPACE;

namespace ocigo {

IndexMap<OCIO::ProcessorRcPtr> g_Processor_map;
IndexMap<OCIO::ProcessorMetadataRcPtr> g_ProcessorMetadata_map;

}

extern "C" {

    void deleteProcessor(ProcessorId p) {
        ocigo::g_Processor_map.remove(p);
    }

    ProcessorId Processor_Create() {
        OCIO::ProcessorRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = OCIO::Processor::Create();
        END_CATCH_ERR
        return ocigo::g_Processor_map.add(ptr);
    }

    bool Processor_isNoOp(ProcessorId p) {
        BEGIN_CATCH_ERR
        return ocigo::g_Processor_map.get(p).get()->isNoOp();
        END_CATCH_ERR
    }

    bool Processor_hasChannelCrosstalk(ProcessorId p) {
        BEGIN_CATCH_ERR
        return ocigo::g_Processor_map.get(p).get()->hasChannelCrosstalk();
        END_CATCH_ERR
    }

    // Processor CPU
    void Processor_apply(ProcessorId p, ImageDesc *i) {
        OCIO::ImageDesc *img = static_cast<OCIO::ImageDesc*>(i);
        BEGIN_CATCH_ERR
        ocigo::g_Processor_map.get(p).get()->apply(*img);
        END_CATCH_ERR
    }

    const char* Processor_getCpuCacheID(ProcessorId p) {
        BEGIN_CATCH_ERR
        return ocigo::g_Processor_map.get(p).get()->getCpuCacheID();
        END_CATCH_ERR
    }

    // ProcessorMetadata
    void deleteProcessorMetadata(ProcessorMetadataId p) {
        ocigo::g_ProcessorMetadata_map.remove(p);
    }

    ProcessorMetadataId Processor_getMetadata(ProcessorId p) {
        OCIO::ConstProcessorMetadataRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = ocigo::g_Processor_map.get(p).get()->getMetadata();
        END_CATCH_ERR
        return ocigo::g_ProcessorMetadata_map.add(
                OCIO_CONST_POINTER_CAST<OCIO::ProcessorMetadata>(ptr));
    }

    ProcessorMetadataId ProcessorMetadata_Create() {
        OCIO::ProcessorMetadataRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = OCIO::ProcessorMetadata::Create();
        END_CATCH_ERR
        return ocigo::g_ProcessorMetadata_map.add(ptr);
    }

    int ProcessorMetadata_getNumFiles(ProcessorMetadataId p) {
        BEGIN_CATCH_ERR
        return ocigo::g_ProcessorMetadata_map.get(p).get()->getNumFiles();
        END_CATCH_ERR
    }

    const char* ProcessorMetadata_getFile(ProcessorMetadataId p, int index) {
        BEGIN_CATCH_ERR
        return ocigo::g_ProcessorMetadata_map.get(p).get()->getFile(index);
        END_CATCH_ERR
    }

    int ProcessorMetadata_getNumLooks(ProcessorMetadataId p) {
        BEGIN_CATCH_ERR
        return ocigo::g_ProcessorMetadata_map.get(p).get()->getNumLooks();
        END_CATCH_ERR
    }

    const char* ProcessorMetadata_getLook(ProcessorMetadataId p, int index) {
        BEGIN_CATCH_ERR
        return ocigo::g_ProcessorMetadata_map.get(p).get()->getLook(index);
        END_CATCH_ERR
    }

    void ProcessorMetadata_addFile(ProcessorMetadataId p, const char* fname) {
        BEGIN_CATCH_ERR
        ocigo::g_ProcessorMetadata_map.get(p).get()->addFile(fname);
        END_CATCH_ERR
    }

    void ProcessorMetadata_addLook(ProcessorMetadataId p, const char* look) {
        BEGIN_CATCH_ERR
        ocigo::g_ProcessorMetadata_map.get(p).get()->addLook(look);
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