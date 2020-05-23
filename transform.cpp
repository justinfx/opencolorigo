#include <OpenColorIO/OpenColorIO.h>

#include "ocio.h"
#include "ocio_abi.h"
#include "storage.h"

namespace OCIO = OCIO_NAMESPACE;

namespace ocigo {

IndexMap<OCIO::TransformRcPtr> g_Transform_map;

}

extern "C" {

    void deleteDisplayTransform(DisplayTransformId p) {
        ocigo::g_Transform_map.remove(p);
    }

    DisplayTransformId DisplayTransform_Create() {
        OCIO::DisplayTransformRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = OCIO::DisplayTransform::Create();
        END_CATCH_ERR
        return ocigo::g_Transform_map.add(OCIO_DYNAMIC_POINTER_CAST<OCIO::Transform>(ptr));
    }

    DisplayTransformId DisplayTransform_createEditableCopy(DisplayTransformId p) {
        OCIO::TransformRcPtr tptr;
        BEGIN_CATCH_ERR
        tptr = ocigo::g_Transform_map.get(p).get()->createEditableCopy();
        END_CATCH_ERR
        if ( tptr == NULL) { return 0; }

        return ocigo::g_Transform_map.add(tptr);
    }

    TransformDirection DisplayTransform_getDirection(DisplayTransformId p) {
        BEGIN_CATCH_ERR
        return (TransformDirection)(ocigo::g_Transform_map.get(p).get()->getDirection());
        END_CATCH_ERR
    }

    void DisplayTransform_setDirection(DisplayTransformId p, TransformDirection dir) {
        BEGIN_CATCH_ERR
        ocigo::g_Transform_map.get(p).get()->setDirection((OCIO::TransformDirection)dir);
        END_CATCH_ERR
    }

    const char* DisplayTransform_getInputColorSpaceName(DisplayTransformId p) {
        BEGIN_CATCH_ERR
        return OCIO_DYNAMIC_POINTER_CAST<OCIO::DisplayTransform>(ocigo::g_Transform_map.get(p))
                .get()->getInputColorSpaceName();
        END_CATCH_ERR
    }

    void DisplayTransform_setInputColorSpaceName(DisplayTransformId p, const char* name) {
       BEGIN_CATCH_ERR
       OCIO_DYNAMIC_POINTER_CAST<OCIO::DisplayTransform>(ocigo::g_Transform_map.get(p))
               .get()->setInputColorSpaceName(name);
       END_CATCH_ERR
    }

    const char* DisplayTransform_getDisplay(DisplayTransformId p) {
        BEGIN_CATCH_ERR
        return OCIO_DYNAMIC_POINTER_CAST<OCIO::DisplayTransform>(ocigo::g_Transform_map.get(p))
                .get()->getDisplay();
        END_CATCH_ERR
    }

    void DisplayTransform_setDisplay(DisplayTransformId p, const char* name) {
       BEGIN_CATCH_ERR
       OCIO_DYNAMIC_POINTER_CAST<OCIO::DisplayTransform>(ocigo::g_Transform_map.get(p))
               .get()->setDisplay(name);
       END_CATCH_ERR
    }

    const char* DisplayTransform_getView(DisplayTransformId p) {
        BEGIN_CATCH_ERR
        return OCIO_DYNAMIC_POINTER_CAST<OCIO::DisplayTransform>(ocigo::g_Transform_map.get(p))
                .get()->getView();
        END_CATCH_ERR
    }

    void DisplayTransform_setView(DisplayTransformId p, const char* name) {
       BEGIN_CATCH_ERR
       OCIO_DYNAMIC_POINTER_CAST<OCIO::DisplayTransform>(ocigo::g_Transform_map.get(p))
               .get()->setView(name);
       END_CATCH_ERR
    }

    const char* DisplayTransform_getLooksOverride(DisplayTransformId p) {
        BEGIN_CATCH_ERR
        return OCIO_DYNAMIC_POINTER_CAST<OCIO::DisplayTransform>(ocigo::g_Transform_map.get(p))
                .get()->getLooksOverride();
        END_CATCH_ERR
    }

    void DisplayTransform_setLooksOverride(DisplayTransformId p, const char* looks) {
       BEGIN_CATCH_ERR
       OCIO_DYNAMIC_POINTER_CAST<OCIO::DisplayTransform>(ocigo::g_Transform_map.get(p))
               .get()->setLooksOverride(looks);
       END_CATCH_ERR
    }

    bool DisplayTransform_getLooksOverrideEnabled(DisplayTransformId p) {
        BEGIN_CATCH_ERR
        return OCIO_DYNAMIC_POINTER_CAST<OCIO::DisplayTransform>(ocigo::g_Transform_map.get(p))
                .get()->getLooksOverrideEnabled();
        END_CATCH_ERR
    }

    void DisplayTransform_setLooksOverrideEnabled(DisplayTransformId p, bool enabled) {
       BEGIN_CATCH_ERR
       OCIO_DYNAMIC_POINTER_CAST<OCIO::DisplayTransform>(ocigo::g_Transform_map.get(p))
               .get()->setLooksOverrideEnabled(enabled);
       END_CATCH_ERR
    }

}