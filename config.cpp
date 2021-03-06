#include <OpenColorIO/OpenColorIO.h>

#include <iostream>
#include <sstream>
#include <cstring>
#include <string>

#include "ocio.h"
#include "ocio_abi.h"
#include "storage.h"
#include "colorspace.h"
#include "context.h"
#include "processor.h"
#include "transform.h"

namespace OCIO = OCIO_NAMESPACE;

namespace ocigo {

IndexMap<OCIO::ConfigRcPtr> g_Config_map;

}

extern "C" {
    void deleteConfig(Config *p) {
        if (p != NULL) {
            if (p->handle) {
                ocigo::g_Config_map.remove(p->handle);
                p->handle = 0;
            }
            freeHandleContext(p);
        }
    }

    // Config Init
    Config* Config_Create() {
        return (Config*) NEW_HANDLE_CONTEXT(ocigo::g_Config_map.add(OCIO::Config::Create()));
    }

    Config* GetCurrentConfig() {
        Config* c = NEW_HANDLE_CONTEXT();
        BEGIN_CATCH_CTX_ERR(c)
        c->handle = ocigo::g_Config_map.add(
                OCIO_CONST_POINTER_CAST<OCIO::Config>(OCIO::GetCurrentConfig()));
        END_CATCH_CTX_ERR(c)
        return c;
    }

    void SetCurrentConfig(Config* c) {
        BEGIN_CATCH_CTX_ERR(c)
        OCIO::SetCurrentConfig(ocigo::g_Config_map.get(c->handle));
        END_CATCH_CTX_ERR(c)
    } 

    Config* Config_CreateFromEnv() {
        Config* c = NEW_HANDLE_CONTEXT();
        BEGIN_CATCH_CTX_ERR(c)
        c->handle = ocigo::g_Config_map.add(
                OCIO_CONST_POINTER_CAST<OCIO::Config>(OCIO::Config::CreateFromEnv()));
        END_CATCH_CTX_ERR(c)
        return c;
    }

    Config* Config_CreateFromFile(const char* filename) {
        Config* c = NEW_HANDLE_CONTEXT();
        BEGIN_CATCH_CTX_ERR(c)
        c->handle = ocigo::g_Config_map.add(
                OCIO_CONST_POINTER_CAST<OCIO::Config>(OCIO::Config::CreateFromFile(filename)));
        END_CATCH_CTX_ERR(c)
        return c;
    }

    Config* Config_CreateFromData(const char* data) {
        std::stringstream s;
        s << data;
        s.seekp(0);

        Config* c = NEW_HANDLE_CONTEXT();

        BEGIN_CATCH_CTX_ERR(c)
        c->handle = ocigo::g_Config_map.add(
                OCIO_CONST_POINTER_CAST<OCIO::Config>(OCIO::Config::CreateFromStream(s)));
        END_CATCH_CTX_ERR(c)

        return c;
    }

    Config* Config_createEditableCopy(Config* p) {
        Config* cpy = NEW_HANDLE_CONTEXT();
        BEGIN_CATCH_CTX_ERR(p)
        cpy->handle = ocigo::g_Config_map.add(ocigo::g_Config_map.get(p->handle).get()->createEditableCopy());
        END_CATCH_CTX_ERR(p)
        return cpy;
    }

    void Config_sanityCheck(Config* p) {
        BEGIN_CATCH_CTX_ERR(p)
        ocigo::g_Config_map.get(p->handle).get()->sanityCheck();
        END_CATCH_CTX_ERR(p)
    }

    char* Config_serialize(Config* p) {
        std::stringstream s;
        BEGIN_CATCH_CTX_ERR(p)
        ocigo::g_Config_map.get(p->handle).get()->serialize(s);
        END_CATCH_CTX_ERR(p)

        char* cstr = new char[s.str().length()+1];
        std::strcpy(cstr, s.str().c_str());

        return cstr;
    }

    const char* Config_getCacheID(Config* p) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->getCacheID();
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    const char* Config_getCacheIDWithContext(Config* p, ContextId c) {
        const char* ret = NULL;
        OCIO::ConstContextRcPtr context = ocigo::g_Context_map.get(c->handle);
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->getCacheID(context);
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    const char* Config_getDescription(Config* p) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->getDescription();
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    // Config Resources
    ContextId Config_getCurrentContext(Config* p) {
        OCIO::ContextRcPtr ptr;

        BEGIN_CATCH_CTX_ERR(p)
        ptr = OCIO_CONST_POINTER_CAST<OCIO::Context>(ocigo::g_Config_map.get(p->handle).get()->getCurrentContext());
        END_CATCH_CTX_ERR(p)
        if (ptr == NULL) { return 0; }

        return NEW_HANDLE_CONTEXT(ocigo::g_Context_map.add(ptr));
    }

    const char* Config_getSearchPath(Config* p) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->getSearchPath();
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    const char* Config_getWorkingDir(Config* p) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->getWorkingDir();
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    // Config Processors 
    ProcessorId Config_getProcessor_CT_CS_CS(Config* p, ContextId ct, ColorSpaceId srcCS, ColorSpaceId dstCS) {
        OCIO::ConstProcessorRcPtr   ptr;
        OCIO::ConstContextRcPtr     ct_ptr    = ocigo::g_Context_map.get(ct->handle);
        OCIO::ConstColorSpaceRcPtr  srcCS_ptr = ocigo::g_ColorSpace_map.get(srcCS);
        OCIO::ConstColorSpaceRcPtr  dstCS_ptr = ocigo::g_ColorSpace_map.get(dstCS);

        BEGIN_CATCH_CTX_ERR(p)
        ptr = ocigo::g_Config_map.get(p->handle).get()->getProcessor(ct_ptr, srcCS_ptr, dstCS_ptr);
        END_CATCH_CTX_ERR(p)

        return NEW_HANDLE_CONTEXT(ocigo::g_Processor_map.add(OCIO_CONST_POINTER_CAST<OCIO::Processor>(ptr)));
    }

    ProcessorId Config_getProcessor_CS_CS(Config* p, ColorSpaceId srcCS, ColorSpaceId dstCS) {
        OCIO::ConstProcessorRcPtr   ptr;
        OCIO::ConstColorSpaceRcPtr  srcCS_ptr = ocigo::g_ColorSpace_map.get(srcCS);
        OCIO::ConstColorSpaceRcPtr  dstCS_ptr = ocigo::g_ColorSpace_map.get(dstCS);
        
        BEGIN_CATCH_CTX_ERR(p)
        ptr = ocigo::g_Config_map.get(p->handle).get()->getProcessor(srcCS_ptr, dstCS_ptr);
        END_CATCH_CTX_ERR(p)

        if ( ptr == NULL) { return 0; }
        return NEW_HANDLE_CONTEXT(ocigo::g_Processor_map.add(OCIO_CONST_POINTER_CAST<OCIO::Processor>(ptr)));
    }

    ProcessorId Config_getProcessor_S_S(Config* p, const char* srcName, const char* dstName) {
        OCIO::ConstProcessorRcPtr ptr;
        
        BEGIN_CATCH_CTX_ERR(p)
        ptr = ocigo::g_Config_map.get(p->handle).get()->getProcessor(srcName, dstName);
        END_CATCH_CTX_ERR(p)

        if ( ptr == NULL) { return 0; }
        return NEW_HANDLE_CONTEXT(ocigo::g_Processor_map.add(OCIO_CONST_POINTER_CAST<OCIO::Processor>(ptr)));
    }

    ProcessorId Config_getProcessor_CT_S_S(Config* p, ContextId ct, const char* srcName, const char* dstName) {
        OCIO::ConstProcessorRcPtr   ptr;
        OCIO::ConstContextRcPtr     ct_ptr = ocigo::g_Context_map.get(ct->handle);

        BEGIN_CATCH_CTX_ERR(p)
        ptr = ocigo::g_Config_map.get(p->handle).get()->getProcessor(ct_ptr, srcName, dstName);
        END_CATCH_CTX_ERR(p)

        if ( ptr == NULL) { return 0; }
        return NEW_HANDLE_CONTEXT(ocigo::g_Processor_map.add(OCIO_CONST_POINTER_CAST<OCIO::Processor>(ptr)));
    }

    ProcessorId Config_getProcessor_TX(Config* p, TransformId tx) {
        OCIO::ConstProcessorRcPtr ptr;
        OCIO::ConstTransformRcPtr tx_ptr = ocigo::g_Transform_map.get(tx);

        BEGIN_CATCH_CTX_ERR(p)
        ptr = ocigo::g_Config_map.get(p->handle).get()->getProcessor(tx_ptr);
        END_CATCH_CTX_ERR(p)

        if ( ptr == NULL) { return 0; }
        return NEW_HANDLE_CONTEXT(ocigo::g_Processor_map.add(OCIO_CONST_POINTER_CAST<OCIO::Processor>(ptr)));
    }

    ProcessorId Config_getProcessor_TX_D(Config* p, TransformId tx, TransformDirection direction) {
        OCIO::ConstProcessorRcPtr ptr;
        OCIO::ConstTransformRcPtr tx_ptr = ocigo::g_Transform_map.get(tx);

        BEGIN_CATCH_CTX_ERR(p)
        ptr = ocigo::g_Config_map.get(p->handle).get()->getProcessor(
            tx_ptr, (OCIO::TransformDirection)direction);
        END_CATCH_CTX_ERR(p)

        if ( ptr == NULL) { return 0; }
        return NEW_HANDLE_CONTEXT(ocigo::g_Processor_map.add(OCIO_CONST_POINTER_CAST<OCIO::Processor>(ptr)));
    }

    ProcessorId Config_getProcessor_CT_TX_D(Config* p, ContextId ct, TransformId tx, TransformDirection direction) {
        OCIO::ConstProcessorRcPtr ptr;
        OCIO::ConstContextRcPtr ct_ptr = ocigo::g_Context_map.get(ct->handle);
        OCIO::ConstTransformRcPtr tx_ptr = ocigo::g_Transform_map.get(tx);

        BEGIN_CATCH_CTX_ERR(p)
        ptr = ocigo::g_Config_map.get(p->handle).get()->getProcessor(
            ct_ptr, tx_ptr, (OCIO::TransformDirection)direction);
        END_CATCH_CTX_ERR(p)

        if ( ptr == NULL) { return 0; }
        return NEW_HANDLE_CONTEXT(ocigo::g_Processor_map.add(OCIO_CONST_POINTER_CAST<OCIO::Processor>(ptr)));
    }

    // Config ColorSpaces
    int Config_getNumColorSpaces(Config* p) {
        int ret = 0;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->getNumColorSpaces();
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    const char* Config_getColorSpaceNameByIndex(Config* p, int index) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->getColorSpaceNameByIndex(index);
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    ColorSpaceId Config_getColorSpace(Config* p, const char* name) {
        OCIO::ConstColorSpaceRcPtr ptr;

        BEGIN_CATCH_CTX_ERR(p)
        ptr = ocigo::g_Config_map.get(p->handle).get()->getColorSpace(name);
        END_CATCH_CTX_ERR(p)

        if ( ptr == NULL) { return 0; }
        return ocigo::g_ColorSpace_map.add(OCIO_CONST_POINTER_CAST<OCIO::ColorSpace>(ptr));
    }

    int Config_getIndexForColorSpace(Config* p, const char* name) {
        int ret = -1;
        BEGIN_CATCH_CTX_ERR(p)
        ret =  ocigo::g_Config_map.get(p->handle).get()->getIndexForColorSpace(name);
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    void Config_addColorSpace(Config* p, ColorSpaceId cs) {
        OCIO::ConstColorSpaceRcPtr colorspace = ocigo::g_ColorSpace_map.get(cs);
        BEGIN_CATCH_CTX_ERR(p)
        ocigo::g_Config_map.get(p->handle).get()->addColorSpace(colorspace);
        END_CATCH_CTX_ERR(p)
    }

    void Config_clearColorSpaces(Config* p) {
        BEGIN_CATCH_CTX_ERR(p)
        ocigo::g_Config_map.get(p->handle).get()->clearColorSpaces();
        END_CATCH_CTX_ERR(p)
    }

    const char* Config_parseColorSpaceFromString(Config* p, const char* str) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->parseColorSpaceFromString(str);
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    bool Config_isStrictParsingEnabled(Config* p) {
        bool ret = false;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->isStrictParsingEnabled();
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    void Config_setStrictParsingEnabled(Config* p, bool enabled) {
        BEGIN_CATCH_CTX_ERR(p)
        ocigo::g_Config_map.get(p->handle).get()->setStrictParsingEnabled(enabled);
        END_CATCH_CTX_ERR(p)
    }

    // Config Roles
    void Config_setRole(Config* p, const char* role, const char* colorSpaceName) {
        BEGIN_CATCH_CTX_ERR(p)
        ocigo::g_Config_map.get(p->handle).get()->setRole(role, colorSpaceName);
        END_CATCH_CTX_ERR(p)
    }

    int Config_getNumRoles(Config* p) {
        int ret = 0;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->getNumRoles();
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    bool Config_hasRole(Config* p, const char* role) {
        bool ret = false;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->hasRole(role);
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    const char* Config_getRoleName(Config* p, int index) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->getRoleName(index);
        END_CATCH_CTX_ERR(p)
        return ret;
    }

    // Config Display/View Registration 
    const char* Config_getDefaultDisplay(Config* p) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->getDefaultDisplay();
        END_CATCH_CTX_ERR(p)
        return ret;
    }
    
    int Config_getNumDisplays(Config* p) {
        int ret = 0;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->getNumDisplays();
        END_CATCH_CTX_ERR(p)
        return ret;
    }
    
    const char* Config_getDisplay(Config* p, int index) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->getDisplay(index);
        END_CATCH_CTX_ERR(p)
        return ret;
    }
    
    const char* Config_getDefaultView(Config* p, const char* display) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->getDefaultView(display);
        END_CATCH_CTX_ERR(p)
        return ret;
    }
    
    int Config_getNumViews(Config* p, const char* display) {
        int ret = 0;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->getNumViews(display);
        END_CATCH_CTX_ERR(p)
        return ret;
    }
    
    const char* Config_getView(Config* p, const char* display, int index) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret =  ocigo::g_Config_map.get(p->handle).get()->getView(display, index);
        END_CATCH_CTX_ERR(p)
        return ret;
    }
    
    const char* Config_getDisplayColorSpaceName(Config* p, const char* display, const char* view) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret =  ocigo::g_Config_map.get(p->handle).get()->getDisplayColorSpaceName(display, view);
        END_CATCH_CTX_ERR(p)
        return ret;
    }
    
    const char* Config_getDisplayLooks(Config* p, const char* display, const char* view) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret =  ocigo::g_Config_map.get(p->handle).get()->getDisplayLooks(display, view);
        END_CATCH_CTX_ERR(p)
        return ret;
    }
    
    void Config_addDisplay(Config* p, const char* display, const char* view, const char* colorSpaceName, const char* looks) {
        BEGIN_CATCH_CTX_ERR(p)
        ocigo::g_Config_map.get(p->handle).get()->addDisplay(display, view, colorSpaceName, looks);
        END_CATCH_CTX_ERR(p)
    }
    
    void Config_clearDisplays(Config* p) {
        BEGIN_CATCH_CTX_ERR(p)
        ocigo::g_Config_map.get(p->handle).get()->clearDisplays();
        END_CATCH_CTX_ERR(p)
    }
    
    void Config_setActiveDisplays(Config* p, const char* displays) {
        BEGIN_CATCH_CTX_ERR(p)
        ocigo::g_Config_map.get(p->handle).get()->setActiveDisplays(displays);
        END_CATCH_CTX_ERR(p)
    }
    
    const char* Config_getActiveDisplays(Config* p) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->getActiveDisplays();
        END_CATCH_CTX_ERR(p)
        return ret;
    }
    
    void Config_setActiveViews(Config* p, const char* views) {
        BEGIN_CATCH_CTX_ERR(p)
        ocigo::g_Config_map.get(p->handle).get()->setActiveViews(views);
        END_CATCH_CTX_ERR(p)
    }

    const char* Config_getActiveViews(Config* p) {
        const char* ret = NULL;
        BEGIN_CATCH_CTX_ERR(p)
        ret = ocigo::g_Config_map.get(p->handle).get()->getActiveViews();
        END_CATCH_CTX_ERR(p)
        return ret;
    }

}