#include <OpenColorIO/OpenColorIO.h>

#include "ocio.h"
#include "shared_ptr_map.h"

namespace OCIO = OCIO_NAMESPACE;

namespace ocigo {

SharedPtrMap<OCIO::ContextRcPtr> g_Context_map;

}

extern "C" {
    void deleteContext(ContextId p) {
        ocigo::g_Context_map.remove(p);
    }

    ContextId Context_Create() {
        OCIO::ContextRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = OCIO::Context::Create();
        END_CATCH_ERR
        return ocigo::g_Context_map.add(ptr);
    }

    ContextId Context_createEditableCopy(ContextId p) {
        OCIO::ContextRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = ocigo::g_Context_map.get(p).get()->createEditableCopy();
        END_CATCH_ERR
        if ( ptr == NULL) { return 0; }
        return ocigo::g_Context_map.add(ptr);
    }

    const char* Context_getCacheID(ContextId p) {
        BEGIN_CATCH_ERR
        return ocigo::g_Context_map.get(p).get()->getCacheID();
        END_CATCH_ERR
    }

    void Context_setSearchPath(ContextId p, const char* path) {
        BEGIN_CATCH_ERR
        ocigo::g_Context_map.get(p).get()->setSearchPath(path);
        END_CATCH_ERR
    }

    const char* Context_getSearchPath(ContextId p) {
        BEGIN_CATCH_ERR
        return ocigo::g_Context_map.get(p).get()->getSearchPath();
        END_CATCH_ERR
    }
    
    void Context_setWorkingDir(ContextId p, const char* dirname) {
        BEGIN_CATCH_ERR
        ocigo::g_Context_map.get(p).get()->setWorkingDir(dirname);
        END_CATCH_ERR
    }
    
    const char* Context_getWorkingDir(ContextId p) {
        BEGIN_CATCH_ERR
        return ocigo::g_Context_map.get(p).get()->getWorkingDir();
        END_CATCH_ERR
    }
    
    void Context_setStringVar(ContextId p, const char* name, const char* value) {
        BEGIN_CATCH_ERR
        ocigo::g_Context_map.get(p).get()->setStringVar(name, value);
        END_CATCH_ERR
    }
    
    const char* Context_getStringVar(ContextId p, const char* name) {
        BEGIN_CATCH_ERR
        return ocigo::g_Context_map.get(p).get()->getStringVar(name);
        END_CATCH_ERR
    }

    EnvironmentMode Context_getEnvironmentMode(ContextId p) {
        BEGIN_CATCH_ERR
        return (EnvironmentMode)(ocigo::g_Context_map.get(p).get()->getEnvironmentMode());
        END_CATCH_ERR
    }

    void Context_setEnvironmentMode(ContextId p, EnvironmentMode mode) {
        BEGIN_CATCH_ERR
        ocigo::g_Context_map.get(p).get()->setEnvironmentMode((OCIO::EnvironmentMode)mode);
        END_CATCH_ERR
    }

    void Context_loadEnvironment(ContextId p) {
        BEGIN_CATCH_ERR
        ocigo::g_Context_map.get(p).get()->loadEnvironment();
        END_CATCH_ERR
    }
    
    const char* Context_resolveStringVar(ContextId p, const char* val) {
        BEGIN_CATCH_ERR
        return ocigo::g_Context_map.get(p).get()->resolveStringVar(val);
        END_CATCH_ERR
    }
    
    const char* Context_resolveFileLocation(ContextId p, const char* filename) {
        BEGIN_CATCH_ERR
        return ocigo::g_Context_map.get(p).get()->resolveFileLocation(filename);
        END_CATCH_ERR
    }

}