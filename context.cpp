#include <OpenColorIO/OpenColorIO.h>

#include "ocio.h"
#include "ocio_abi.h"
#include "storage.h"

namespace OCIO = OCIO_NAMESPACE;

namespace ocigo {

IndexMap<OCIO::ContextRcPtr> g_Context_map;

}

extern "C" {
    void deleteContext(ContextId p) {
        if (p != NULL) {
            if (p->handle) {
                ocigo::g_Context_map.remove(p->handle);
                p->handle = 0;
            }
            freeHandleContext(p);
        }
    }

    ContextId Context_Create() {
        OCIO::ContextRcPtr ptr;
        ContextId ctx = NEW_HANDLE_CONTEXT();
        BEGIN_CATCH_CTX_ERR(ctx)
        ctx->handle = ocigo::g_Context_map.add(OCIO::Context::Create());
        END_CATCH_CTX_ERR(ctx)
        return ctx;
    }

    ContextId Context_createEditableCopy(ContextId p) {
        OCIO::ContextRcPtr ptr;
        BEGIN_CATCH_CTX_ERR(p)
        ptr = ocigo::g_Context_map.get(p->handle).get()->createEditableCopy();
        END_CATCH_CTX_ERR(p)
        if ( ptr == NULL) { return 0; }
        return NEW_HANDLE_CONTEXT(ocigo::g_Context_map.add(ptr));
    }

    const char* Context_getCacheID(ContextId p) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Context_map.get(p->handle).get()->getCacheID();
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    void Context_setSearchPath(ContextId p, const char* path) {
        BEGIN_CATCH_CTX_ERR(p)
        ocigo::g_Context_map.get(p->handle).get()->setSearchPath(path);
        END_CATCH_CTX_ERR(p)
    }

    const char* Context_getSearchPath(ContextId p) {
        BEGIN_CATCH_CTX_ERR(p)
        return ocigo::g_Context_map.get(p->handle).get()->getSearchPath();
        END_CATCH_CTX_ERR(p)
    }
    
    void Context_setWorkingDir(ContextId p, const char* dirname) {
        BEGIN_CATCH_CTX_ERR(p)
        ocigo::g_Context_map.get(p->handle).get()->setWorkingDir(dirname);
        END_CATCH_CTX_ERR(p)
    }
    
    const char* Context_getWorkingDir(ContextId p) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Context_map.get(p->handle).get()->getWorkingDir();
        END_CATCH_CTX_ERR(p)
        return ret;
    }
    
    void Context_setStringVar(ContextId p, const char* name, const char* value) {
        BEGIN_CATCH_CTX_ERR(p)
        ocigo::g_Context_map.get(p->handle).get()->setStringVar(name, value);
        END_CATCH_CTX_ERR(p)
    }
    
    const char* Context_getStringVar(ContextId p, const char* name) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Context_map.get(p->handle).get()->getStringVar(name);
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    EnvironmentMode Context_getEnvironmentMode(ContextId p) {
        EnvironmentMode ret;
        BEGIN_CATCH_CTX_ERR(p)
        ret = (EnvironmentMode)(ocigo::g_Context_map.get(p->handle).get()->getEnvironmentMode());
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    void Context_setEnvironmentMode(ContextId p, EnvironmentMode mode) {
        BEGIN_CATCH_CTX_ERR(p)
        ocigo::g_Context_map.get(p->handle).get()->setEnvironmentMode((OCIO::EnvironmentMode)mode);
        END_CATCH_CTX_ERR(p)
    }

    void Context_loadEnvironment(ContextId p) {
        BEGIN_CATCH_CTX_ERR(p)
        ocigo::g_Context_map.get(p->handle).get()->loadEnvironment();
        END_CATCH_CTX_ERR(p)
    }
    
    const char* Context_resolveStringVar(ContextId p, const char* val) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret =  ocigo::g_Context_map.get(p->handle).get()->resolveStringVar(val);
        END_CATCH_CTX_ERR(p)
        return ret;
    }
    
    const char* Context_resolveFileLocation(ContextId p, const char* filename) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Context_map.get(p->handle).get()->resolveFileLocation(filename);
        END_CATCH_CTX_ERR(p)
        return ret;
    }

}