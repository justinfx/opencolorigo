#include <OpenColorIO/OpenColorIO.h>

#include <iostream>

#include "ocio.h"


extern "C" {

    namespace OCIO = OCIO_NAMESPACE;

    void deleteColorSpace(ColorSpace *p) {
        if (p != NULL) {
            delete static_cast<OCIO::ColorSpaceRcPtr*>(p);
        }
    }

    ColorSpace* ColorSpace_Create() {
        OCIO::ColorSpaceRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = OCIO::ColorSpace::Create();
        END_CATCH_ERR
        return (ColorSpace*) new OCIO::ColorSpaceRcPtr(ptr);
    }

    ColorSpace* ColorSpace_createEditableCopy(ColorSpace *p) {
        OCIO::ColorSpaceRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = static_cast<OCIO::ConstColorSpaceRcPtr*>(p)->get()->createEditableCopy();
        END_CATCH_ERR
        if ( ptr == NULL) { return NULL; }
        return (ColorSpace*) new OCIO::ColorSpaceRcPtr(ptr);
    }

    const char* ColorSpace_getName(ColorSpace *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstColorSpaceRcPtr*>(p)->get()->getName();
        END_CATCH_ERR
    }

    void ColorSpace_setName(ColorSpace *p, const char* name) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ColorSpaceRcPtr*>(p)->get()->setName(name);
        END_CATCH_ERR
    }

    const char* ColorSpace_getFamily(ColorSpace *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstColorSpaceRcPtr*>(p)->get()->getFamily();
        END_CATCH_ERR
    }

    void ColorSpace_setFamily(ColorSpace *p, const char* family) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ColorSpaceRcPtr*>(p)->get()->setFamily(family);
        END_CATCH_ERR
    }

    const char* ColorSpace_getEqualityGroup(ColorSpace *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstColorSpaceRcPtr*>(p)->get()->getEqualityGroup();
        END_CATCH_ERR
    }

    void ColorSpace_setEqualityGroup(ColorSpace *p, const char* group) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ColorSpaceRcPtr*>(p)->get()->setEqualityGroup(group);
        END_CATCH_ERR
    }

    const char* ColorSpace_getDescription(ColorSpace *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstColorSpaceRcPtr*>(p)->get()->getDescription();
        END_CATCH_ERR
    }

    void ColorSpace_setDescription(ColorSpace *p, const char* description) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ColorSpaceRcPtr*>(p)->get()->setDescription(description);
        END_CATCH_ERR
    }

    BitDepth ColorSpace_getBitDepth(ColorSpace *p) {
        BEGIN_CATCH_ERR
        return (BitDepth)static_cast<OCIO::ConstColorSpaceRcPtr*>(p)->get()->getBitDepth();
        END_CATCH_ERR
    }

    void ColorSpace_setBitDepth(ColorSpace *p, BitDepth bitDepth) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ColorSpaceRcPtr*>(p)->get()->setBitDepth((OCIO::BitDepth)bitDepth);
        END_CATCH_ERR
    }

}