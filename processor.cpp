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
        if (p != NULL) {
            if (p->handle) {
                ocigo::g_Processor_map.remove(p->handle);
                p->handle = 0;
            }
            freeHandleContext(p);
        }
    }

    ProcessorId Processor_Create() {
        OCIO::ProcessorRcPtr ptr;
        ProcessorId p = NEW_HANDLE_CONTEXT();
        BEGIN_CATCH_CTX_ERR(p)
        p->handle = ocigo::g_Processor_map.add(OCIO::Processor::Create());
        END_CATCH_CTX_ERR(p)
        return p;
    }

    bool Processor_isNoOp(ProcessorId p) {
        bool ret = false;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Processor_map.get(p->handle).get()->isNoOp();
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    bool Processor_hasChannelCrosstalk(ProcessorId p) {
        bool ret = false;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Processor_map.get(p->handle).get()->hasChannelCrosstalk();
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    // Processor CPU
    void Processor_apply(ProcessorId p, ImageDesc *i) {
        OCIO::ImageDesc *img = static_cast<OCIO::ImageDesc*>(i);
        BEGIN_CATCH_CTX_ERR(p)
        ocigo::g_Processor_map.get(p->handle).get()->apply(*img);
        END_CATCH_CTX_ERR(p)
    }

    const char* Processor_getCpuCacheID(ProcessorId p) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Processor_map.get(p->handle).get()->getCpuCacheID();
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    // ProcessorMetadata
    void deleteProcessorMetadata(ProcessorMetadataId p) {
        ocigo::g_ProcessorMetadata_map.remove(p);
    }

    ProcessorMetadataId Processor_getMetadata(ProcessorId p) {
        OCIO::ConstProcessorMetadataRcPtr ptr;
        BEGIN_CATCH_CTX_ERR(p)
        ptr = ocigo::g_Processor_map.get(p->handle).get()->getMetadata();
        END_CATCH_CTX_ERR(p)
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
        int ret = 0;
        BEGIN_CATCH_ERR
        ret = ocigo::g_ProcessorMetadata_map.get(p).get()->getNumFiles();
        END_CATCH_ERR
        return ret;
    }

    const char* ProcessorMetadata_getFile(ProcessorMetadataId p, int index) {
        const char* ret = NULL;
        BEGIN_CATCH_ERR
        ret =  ocigo::g_ProcessorMetadata_map.get(p).get()->getFile(index);
        END_CATCH_ERR
        return ret;
    }

    int ProcessorMetadata_getNumLooks(ProcessorMetadataId p) {
        BEGIN_CATCH_ERR
        return ocigo::g_ProcessorMetadata_map.get(p).get()->getNumLooks();
        END_CATCH_ERR
    }

    const char* ProcessorMetadata_getLook(ProcessorMetadataId p, int index) {
        const char* ret = NULL;
        BEGIN_CATCH_ERR
        ret = ocigo::g_ProcessorMetadata_map.get(p).get()->getLook(index);
        END_CATCH_ERR
        return ret;
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
        PackedImageDesc* ret = NULL;
        BEGIN_CATCH_ERR
        ret = (PackedImageDesc*) new OCIO::PackedImageDesc(data, width, height, numChannels);
        END_CATCH_ERR
        return ret;
    }

    float* PackedImageDesc_getData(PackedImageDesc *p) {
        float* ret = NULL;
        BEGIN_CATCH_ERR
        ret = static_cast<OCIO::PackedImageDesc*>(p)->getData();
        END_CATCH_ERR
        return ret;
    }

    long PackedImageDesc_getWidth(PackedImageDesc *p) {
        long ret = 0;
        BEGIN_CATCH_ERR
        ret = static_cast<OCIO::PackedImageDesc*>(p)->getWidth();
        END_CATCH_ERR
        return ret;
    }

    long PackedImageDesc_getHeight(PackedImageDesc *p) {
        long ret = 0;
        BEGIN_CATCH_ERR
        ret = static_cast<OCIO::PackedImageDesc*>(p)->getHeight();
        END_CATCH_ERR
        return ret;
    }

    long PackedImageDesc_getNumChannels(PackedImageDesc *p) {
        long ret = 0;
        BEGIN_CATCH_ERR
        ret = static_cast<OCIO::PackedImageDesc*>(p)->getNumChannels();
        END_CATCH_ERR
        return ret;
    }

}