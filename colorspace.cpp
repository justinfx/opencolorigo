#include <OpenColorIO/OpenColorIO.h>

#include "ocio.h"


extern "C" {

    namespace OCIO = OCIO_NAMESPACE;

    const char* ColorSpace_getName(ColorSpace *p) {
        return static_cast<OCIO::ConstColorSpaceRcPtr*>(p)->get()->getName();
    }

    void ColorSpace_setName(ColorSpace *p, const char* name) {
        return static_cast<OCIO::ColorSpaceRcPtr*>(p)->get()->setName(name);
    }

    const char* ColorSpace_getFamily(ColorSpace *p) {
        return static_cast<OCIO::ConstColorSpaceRcPtr*>(p)->get()->getFamily();
    }

    void ColorSpace_setFamily(ColorSpace *p, const char* family) {
        return static_cast<OCIO::ColorSpaceRcPtr*>(p)->get()->setFamily(family);
    }

    const char* ColorSpace_getEqualityGroup(ColorSpace *p) {
        return static_cast<OCIO::ConstColorSpaceRcPtr*>(p)->get()->getEqualityGroup();
    }

    void ColorSpace_setEqualityGroup(ColorSpace *p, const char* group) {
        return static_cast<OCIO::ColorSpaceRcPtr*>(p)->get()->setEqualityGroup(group);
    }

    const char* ColorSpace_getDescription(ColorSpace *p) {
        return static_cast<OCIO::ConstColorSpaceRcPtr*>(p)->get()->getDescription();
    }

    void ColorSpace_setDescription(ColorSpace *p, const char* description) {
        return static_cast<OCIO::ColorSpaceRcPtr*>(p)->get()->setDescription(description);
    }

    BitDepth ColorSpace_getBitDepth(ColorSpace *p) {
        return (BitDepth)static_cast<OCIO::ConstColorSpaceRcPtr*>(p)->get()->getBitDepth();
    }

    void ColorSpace_setBitDepth(ColorSpace *p, BitDepth bitDepth) {
        return static_cast<OCIO::ColorSpaceRcPtr*>(p)->get()->setBitDepth((OCIO::BitDepth)bitDepth);
    }

}