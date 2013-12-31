#include <OpenColorIO/OpenColorIO.h>

#include "ocio.h"


extern "C" {

    namespace OCIO = OCIO_NAMESPACE;

    Context* Context_Create() {
        return (Context*) new OCIO::ContextRcPtr(OCIO::Context::Create());
    }

    Context* Context_createEditableCopy(Context *p) {
        OCIO::ContextRcPtr ptr = static_cast<OCIO::ConstContextRcPtr*>(p)->get()->createEditableCopy();
        return (Context*) new OCIO::ContextRcPtr(ptr);
    }

    const char* Context_getCacheID(Context *p) {
        return static_cast<OCIO::ConstContextRcPtr*>(p)->get()->getCacheID();
    }

    void Context_setSearchPath(Context *p, const char* path) {
        static_cast<OCIO::ContextRcPtr*>(p)->get()->setSearchPath(path);
    }

    const char* Context_getSearchPath(Context *p) {
        return static_cast<OCIO::ConstContextRcPtr*>(p)->get()->getSearchPath();
    }
    
    void Context_setWorkingDir(Context *p, const char* dirname) {
        static_cast<OCIO::ContextRcPtr*>(p)->get()->setWorkingDir(dirname);
    }
    
    const char* Context_getWorkingDir(Context *p) {
        return static_cast<OCIO::ConstContextRcPtr*>(p)->get()->getWorkingDir();
    }
    
    void Context_setStringVar(Context *p, const char* name, const char* value) {
        static_cast<OCIO::ContextRcPtr*>(p)->get()->setStringVar(name, value);
    }
    
    const char* Context_getStringVar(Context *p, const char* name) {
        return static_cast<OCIO::ConstContextRcPtr*>(p)->get()->getStringVar(name);
    }
    
    void Context_loadEnvironment(Context *p) {
        static_cast<OCIO::ContextRcPtr*>(p)->get()->loadEnvironment();
    }
    
    const char* Context_resolveStringVar(Context *p, const char* val) {
        return static_cast<OCIO::ConstContextRcPtr*>(p)->get()->resolveStringVar(val);
    }
    
    const char* Context_resolveFileLocation(Context *p, const char* filename) {
        return static_cast<OCIO::ConstContextRcPtr*>(p)->get()->resolveFileLocation(filename);
    }

}