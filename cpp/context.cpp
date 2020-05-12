#include <OpenColorIO/OpenColorIO.h>

#include "ocio.h"


extern "C" {

    namespace OCIO = OCIO_NAMESPACE;

    Context* Context_Create() {
        OCIO::ContextRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = OCIO::Context::Create();
        END_CATCH_ERR
        return (Context*) new OCIO::ContextRcPtr(ptr);
    }

    Context* Context_createEditableCopy(Context *p) {
        OCIO::ContextRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = static_cast<OCIO::ConstContextRcPtr*>(p)->get()->createEditableCopy();
        END_CATCH_ERR
        if ( ptr == NULL) { return NULL; }
        return (Context*) new OCIO::ContextRcPtr(ptr);
    }

    const char* Context_getCacheID(Context *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstContextRcPtr*>(p)->get()->getCacheID();
        END_CATCH_ERR
    }

    void Context_setSearchPath(Context *p, const char* path) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ContextRcPtr*>(p)->get()->setSearchPath(path);
        END_CATCH_ERR
    }

    const char* Context_getSearchPath(Context *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstContextRcPtr*>(p)->get()->getSearchPath();
        END_CATCH_ERR
    }
    
    void Context_setWorkingDir(Context *p, const char* dirname) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ContextRcPtr*>(p)->get()->setWorkingDir(dirname);
        END_CATCH_ERR
    }
    
    const char* Context_getWorkingDir(Context *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstContextRcPtr*>(p)->get()->getWorkingDir();
        END_CATCH_ERR
    }
    
    void Context_setStringVar(Context *p, const char* name, const char* value) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ContextRcPtr*>(p)->get()->setStringVar(name, value);
        END_CATCH_ERR
    }
    
    const char* Context_getStringVar(Context *p, const char* name) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstContextRcPtr*>(p)->get()->getStringVar(name);
        END_CATCH_ERR
    }

    EnvironmentMode Context_getEnvironmentMode(Context *p) {
        BEGIN_CATCH_ERR
        return (EnvironmentMode)(static_cast<OCIO::ContextRcPtr*>(p)->get()->getEnvironmentMode());
        END_CATCH_ERR
    }

    void Context_setEnvironmentMode(Context *p, EnvironmentMode mode) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ContextRcPtr*>(p)->get()->setEnvironmentMode((OCIO::EnvironmentMode)mode);
        END_CATCH_ERR
    }

    void Context_loadEnvironment(Context *p) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ContextRcPtr*>(p)->get()->loadEnvironment();
        END_CATCH_ERR
    }
    
    const char* Context_resolveStringVar(Context *p, const char* val) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstContextRcPtr*>(p)->get()->resolveStringVar(val);
        END_CATCH_ERR
    }
    
    const char* Context_resolveFileLocation(Context *p, const char* filename) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstContextRcPtr*>(p)->get()->resolveFileLocation(filename);
        END_CATCH_ERR
    }

}