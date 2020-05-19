#include <OpenColorIO/OpenColorIO.h>

#include "ocio.h"


extern "C" {

    namespace OCIO = OCIO_NAMESPACE;

    void deleteDisplayTransform(DisplayTransform* d) {
        if (d != NULL) {
            delete (OCIO::DisplayTransformRcPtr*)d;
        }
    }

    DisplayTransform* DisplayTransform_Create() {
        OCIO::DisplayTransformRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = OCIO::DisplayTransform::Create();
        END_CATCH_ERR
        return (DisplayTransform*) new OCIO::DisplayTransformRcPtr(ptr);
    }

    DisplayTransform* DisplayTransform_createEditableCopy(DisplayTransform *p) {
        OCIO::TransformRcPtr tptr;
        BEGIN_CATCH_ERR
        tptr = static_cast<OCIO::DisplayTransformRcPtr*>(p)->get()->createEditableCopy();
        END_CATCH_ERR
        if ( tptr == NULL) { return NULL; }

        OCIO::DisplayTransformRcPtr dptr = OCIO_DYNAMIC_POINTER_CAST<OCIO::DisplayTransform>(tptr);
        if ( dptr == NULL) { return NULL; }

        // swap
        OCIO::DisplayTransformRcPtr* ptr = new OCIO::DisplayTransformRcPtr(dptr);
        tptr.reset();

        return (DisplayTransform*) ptr;
    }

    TransformDirection DisplayTransform_getDirection(DisplayTransform *p) {
        BEGIN_CATCH_ERR
        return (TransformDirection)(static_cast<OCIO::DisplayTransformRcPtr*>(p)->get()->getDirection());
        END_CATCH_ERR
    }

    void DisplayTransform_setDirection(DisplayTransform *p, TransformDirection dir) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::DisplayTransformRcPtr*>(p)->get()->setDirection((OCIO::TransformDirection)dir);
        END_CATCH_ERR
    }

    const char* DisplayTransform_getInputColorSpaceName(DisplayTransform *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::DisplayTransformRcPtr*>(p)->get()->getInputColorSpaceName();
        END_CATCH_ERR
    }

    void DisplayTransform_setInputColorSpaceName(DisplayTransform *p, const char* name) {
       BEGIN_CATCH_ERR
       static_cast<OCIO::DisplayTransformRcPtr*>(p)->get()->setInputColorSpaceName(name);
       END_CATCH_ERR
    }

    const char* DisplayTransform_getDisplay(DisplayTransform *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::DisplayTransformRcPtr*>(p)->get()->getDisplay();
        END_CATCH_ERR
    }

    void DisplayTransform_setDisplay(DisplayTransform *p, const char* name) {
       BEGIN_CATCH_ERR
       static_cast<OCIO::DisplayTransformRcPtr*>(p)->get()->setDisplay(name);
       END_CATCH_ERR
    }

    const char* DisplayTransform_getView(DisplayTransform *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::DisplayTransformRcPtr*>(p)->get()->getView();
        END_CATCH_ERR
    }

    void DisplayTransform_setView(DisplayTransform *p, const char* name) {
       BEGIN_CATCH_ERR
       static_cast<OCIO::DisplayTransformRcPtr*>(p)->get()->setView(name);
       END_CATCH_ERR
    }

    const char* DisplayTransform_getLooksOverride(DisplayTransform *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::DisplayTransformRcPtr*>(p)->get()->getLooksOverride();
        END_CATCH_ERR
    }

    void DisplayTransform_setLooksOverride(DisplayTransform *p, const char* looks) {
       BEGIN_CATCH_ERR
       static_cast<OCIO::DisplayTransformRcPtr*>(p)->get()->setLooksOverride(looks);
       END_CATCH_ERR
    }

    bool DisplayTransform_getLooksOverrideEnabled(DisplayTransform *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::DisplayTransformRcPtr*>(p)->get()->getLooksOverrideEnabled();
        END_CATCH_ERR
    }

    void DisplayTransform_setLooksOverrideEnabled(DisplayTransform *p, bool enabled) {
       BEGIN_CATCH_ERR
       static_cast<OCIO::DisplayTransformRcPtr*>(p)->get()->setLooksOverrideEnabled(enabled);
       END_CATCH_ERR
    }

}