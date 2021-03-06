#include <OpenColorIO/OpenColorIO.h>

#include <iostream>

#include "ocio.h"
#include "ocio_abi.h"
#include "storage.h"

namespace OCIO = OCIO_NAMESPACE;

namespace ocigo {

IndexMap<OCIO::ColorSpaceRcPtr> g_ColorSpace_map;

}

extern "C" {

    void deleteColorSpace(ColorSpaceId p) {
        ocigo::g_ColorSpace_map.remove(p);
    }

    ColorSpaceId ColorSpace_Create() {
        OCIO::ColorSpaceRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = OCIO::ColorSpace::Create();
        END_CATCH_ERR
        return ocigo::g_ColorSpace_map.add(ptr);
    }

    ColorSpaceId ColorSpace_createEditableCopy(ColorSpaceId p) {
        OCIO::ColorSpaceRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = ocigo::g_ColorSpace_map.get(p).get()->createEditableCopy();
        END_CATCH_ERR
        if ( ptr == NULL) { return 0; }
        return ocigo::g_ColorSpace_map.add(ptr);
    }

    const char* ColorSpace_getName(ColorSpaceId p) {
        const char* ret = NULL;
        BEGIN_CATCH_ERR
        ret = ocigo::g_ColorSpace_map.get(p).get()->getName();
        END_CATCH_ERR
        return ret;
    }

    void ColorSpace_setName(ColorSpaceId p, const char* name) {
        BEGIN_CATCH_ERR
        ocigo::g_ColorSpace_map.get(p).get()->setName(name);
        END_CATCH_ERR
    }

    const char* ColorSpace_getFamily(ColorSpaceId p) {
        const char* ret = NULL;
        BEGIN_CATCH_ERR
        ret = ocigo::g_ColorSpace_map.get(p).get()->getFamily();
        END_CATCH_ERR
        return ret;
    }

    void ColorSpace_setFamily(ColorSpaceId p, const char* family) {
        BEGIN_CATCH_ERR
        ocigo::g_ColorSpace_map.get(p).get()->setFamily(family);
        END_CATCH_ERR
    }

    const char* ColorSpace_getEqualityGroup(ColorSpaceId p) {
        const char* ret = NULL;
        BEGIN_CATCH_ERR
        ret = ocigo::g_ColorSpace_map.get(p).get()->getEqualityGroup();
        END_CATCH_ERR
        return ret;
    }

    void ColorSpace_setEqualityGroup(ColorSpaceId p, const char* group) {
        BEGIN_CATCH_ERR
        return ocigo::g_ColorSpace_map.get(p).get()->setEqualityGroup(group);
        END_CATCH_ERR
    }

    const char* ColorSpace_getDescription(ColorSpaceId p) {
        const char* ret = NULL;
        BEGIN_CATCH_ERR
        ret = ocigo::g_ColorSpace_map.get(p).get()->getDescription();
        END_CATCH_ERR
        return ret;
    }

    void ColorSpace_setDescription(ColorSpaceId p, const char* description) {
        BEGIN_CATCH_ERR
        ocigo::g_ColorSpace_map.get(p).get()->setDescription(description);
        END_CATCH_ERR
    }

    BitDepth ColorSpace_getBitDepth(ColorSpaceId p) {
        BitDepth ret;
        BEGIN_CATCH_ERR
        ret = (BitDepth)ocigo::g_ColorSpace_map.get(p).get()->getBitDepth();
        END_CATCH_ERR
        return ret;
    }

    void ColorSpace_setBitDepth(ColorSpaceId p, BitDepth bitDepth) {
        BEGIN_CATCH_ERR
        ocigo::g_ColorSpace_map.get(p).get()->setBitDepth((OCIO::BitDepth)bitDepth);
        END_CATCH_ERR
    }

}