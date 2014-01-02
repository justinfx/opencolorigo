#include <OpenColorIO/OpenColorIO.h>

#include <iostream>
#include <sstream>
#include <string>

#include "ocio.h"


extern "C" {

    namespace OCIO = OCIO_NAMESPACE;

    // Config Init

    Config* Config_Create() {
        return (Config*) new OCIO::ConfigRcPtr(OCIO::Config::Create());
    }

    const Config* GetCurrentConfig() {
        OCIO::ConstConfigRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = OCIO::GetCurrentConfig();
        END_CATCH_ERR
        if ( ptr == NULL) { return NULL; }
        return (Config*) new OCIO::ConstConfigRcPtr(ptr);
    }

    void SetCurrentConfig(Config *p) {
        BEGIN_CATCH_ERR
        OCIO::SetCurrentConfig(* static_cast<OCIO::ConstConfigRcPtr*>(p));
        END_CATCH_ERR
    } 

    const Config* Config_CreateFromEnv() {
        OCIO::ConstConfigRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = OCIO::Config::CreateFromEnv();
        END_CATCH_ERR
        if ( ptr == NULL) { return NULL; }
        return (Config*) new OCIO::ConstConfigRcPtr(ptr);
    }

    const Config* Config_CreateFromFile(const char* filename) {
        OCIO::ConstConfigRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = OCIO::Config::CreateFromFile(filename);
        END_CATCH_ERR
        if ( ptr == NULL) { return NULL; }
        return (Config*) new OCIO::ConstConfigRcPtr(ptr);
    }

    const Config* Config_CreateFromData(const char* data) {
        std::stringstream s;
        s << data;
        s.seekp(0);
        OCIO::ConstConfigRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = OCIO::Config::CreateFromStream(s);
        END_CATCH_ERR
        if ( ptr == NULL) { return NULL; }
        return (Config*) new OCIO::ConstConfigRcPtr(ptr);
    }

    Config* Config_createEditableCopy(Config *p) {
        OCIO::ConfigRcPtr ptr;
        BEGIN_CATCH_ERR
        ptr = static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->createEditableCopy();
        END_CATCH_ERR
        if ( ptr == NULL) { return NULL; }
        return (Config*) new OCIO::ConfigRcPtr(ptr);
    }

    void Config_sanityCheck(Config *p) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->sanityCheck();
        END_CATCH_ERR
    }

    const char* Config_serialize(Config *p) {
        std::stringstream s;
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->serialize(s);
        END_CATCH_ERR
        return s.str().c_str();
    }

    const char* Config_getCacheID(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getCacheID();
        END_CATCH_ERR
    }

    const char* Config_getCacheIDWithContext(Config *p, Context *c) {
        BEGIN_CATCH_ERR
        OCIO::ConstContextRcPtr context = *(static_cast<OCIO::ConstContextRcPtr*>(c));
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getCacheID(context);
        END_CATCH_ERR
    }

    const char* Config_getDescription(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getDescription();
        END_CATCH_ERR
    }

    // Config Resources
    Context* Config_getCurrentContext(Config *p) {
        OCIO::ConstContextRcPtr ptr;

        BEGIN_CATCH_ERR
        ptr = static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getCurrentContext();
        END_CATCH_ERR
        
        if ( ptr == NULL) { return NULL; }
        return (Context*) new OCIO::ConstContextRcPtr(ptr);
    }

    const char* Config_getSearchPath(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getSearchPath();
        END_CATCH_ERR
    }

    const char* Config_getWorkingDir(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getWorkingDir();
        END_CATCH_ERR
    }

    // Config Processors 
    Processor* Config_getProcessor_CT_CS_CS(Config *p, Context* ct, ColorSpace* srcCS, ColorSpace* dstCS) {
        OCIO::ConstProcessorRcPtr   ptr;
        OCIO::ConstContextRcPtr     ct_ptr    = * static_cast<OCIO::ConstContextRcPtr*>(ct);
        OCIO::ConstColorSpaceRcPtr  srcCS_ptr = * static_cast<OCIO::ConstColorSpaceRcPtr*>(srcCS);
        OCIO::ConstColorSpaceRcPtr  dstCS_ptr = * static_cast<OCIO::ConstColorSpaceRcPtr*>(dstCS);

        BEGIN_CATCH_ERR
        ptr = static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getProcessor(ct_ptr, srcCS_ptr, dstCS_ptr);
        END_CATCH_ERR

        if ( ptr == NULL) { return NULL; }
        return (Processor*) new OCIO::ConstProcessorRcPtr(ptr);

    }

    Processor* Config_getProcessor_CS_CS(Config *p, ColorSpace* srcCS, ColorSpace* dstCS) {
        OCIO::ConstProcessorRcPtr   ptr;
        OCIO::ConstColorSpaceRcPtr  srcCS_ptr = * static_cast<OCIO::ConstColorSpaceRcPtr*>(srcCS);
        OCIO::ConstColorSpaceRcPtr  dstCS_ptr = * static_cast<OCIO::ConstColorSpaceRcPtr*>(dstCS);
        
        BEGIN_CATCH_ERR
        ptr = static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getProcessor(srcCS_ptr, dstCS_ptr);
        END_CATCH_ERR

        if ( ptr == NULL) { return NULL; }
        return (Processor*) new OCIO::ConstProcessorRcPtr(ptr);
    }

    Processor* Config_getProcessor_S_S(Config *p, const char* srcName, const char* dstName) {
        OCIO::ConstProcessorRcPtr ptr;
        
        BEGIN_CATCH_ERR
        ptr = static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getProcessor(srcName, dstName);
        END_CATCH_ERR

        if ( ptr == NULL) { return NULL; }
        return (Processor*) new OCIO::ConstProcessorRcPtr(ptr);
    }

    Processor* Config_getProcessor_CT_S_S(Config *p, Context* ct, const char* srcName, const char* dstName) {
        OCIO::ConstProcessorRcPtr   ptr;
        OCIO::ConstContextRcPtr     ct_ptr = * static_cast<OCIO::ConstContextRcPtr*>(ct);

        BEGIN_CATCH_ERR
        ptr = static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getProcessor(ct_ptr, srcName, dstName);
        END_CATCH_ERR

        if ( ptr == NULL) { return NULL; }
        return (Processor*) new OCIO::ConstProcessorRcPtr(ptr);
    }

    // Config ColorSpaces
    int Config_getNumColorSpaces(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getNumColorSpaces();
        END_CATCH_ERR
    }

    const char* Config_getColorSpaceNameByIndex(Config *p, int index) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getColorSpaceNameByIndex(index);
        END_CATCH_ERR
    }

    const ColorSpace* Config_getColorSpace(Config *p, const char* name) {
        OCIO::ConstColorSpaceRcPtr ptr;

        BEGIN_CATCH_ERR
        ptr = static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getColorSpace(name);
        END_CATCH_ERR

        if ( ptr == NULL) { return NULL; }
        return (ColorSpace*) new OCIO::ConstColorSpaceRcPtr(ptr);
    }

    int Config_getIndexForColorSpace(Config *p, const char* name) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getIndexForColorSpace(name);
        END_CATCH_ERR
    }

    void Config_addColorSpace(Config *p, ColorSpace *cs) {
        OCIO::ConstColorSpaceRcPtr colorspace = *(static_cast<OCIO::ConstColorSpaceRcPtr*>(cs));
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConfigRcPtr*>(p)->get()->addColorSpace(colorspace);
        END_CATCH_ERR
    }

    void Config_clearColorSpaces(Config *p) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConfigRcPtr*>(p)->get()->clearColorSpaces();
        END_CATCH_ERR
    }

    const char* Config_parseColorSpaceFromString(Config *p, const char* str) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->parseColorSpaceFromString(str);
        END_CATCH_ERR
    }

    bool Config_isStrictParsingEnabled(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->isStrictParsingEnabled();
        END_CATCH_ERR
    }

    void Config_setStrictParsingEnabled(Config *p, bool enabled) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConfigRcPtr*>(p)->get()->setStrictParsingEnabled(enabled);
        END_CATCH_ERR
    }

    // Config Roles
    void Config_setRole(Config *p, const char* role, const char* colorSpaceName) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConfigRcPtr*>(p)->get()->setRole(role, colorSpaceName);
        END_CATCH_ERR
    }

    int Config_getNumRoles(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getNumRoles();
        END_CATCH_ERR
    }

    bool Config_hasRole(Config *p, const char* role) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->hasRole(role);
        END_CATCH_ERR
    }

    const char* Config_getRoleName(Config *p, int index) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getRoleName(index);
        END_CATCH_ERR
    }

    // Config Display/View Registration 
    const char* Config_getDefaultDisplay(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getDefaultDisplay();
        END_CATCH_ERR
    }
    
    int Config_getNumDisplays(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getNumDisplays();
        END_CATCH_ERR
    }
    
    const char* Config_getDisplay(Config *p, int index) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getDisplay(index);
        END_CATCH_ERR
    }
    
    const char* Config_getDefaultView(Config *p, const char* display) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getDefaultView(display);
        END_CATCH_ERR
    }
    
    int Config_getNumViews(Config *p, const char* display) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getNumViews(display);
        END_CATCH_ERR
    }
    
    const char* Config_getView(Config *p, const char* display, int index) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getView(display, index);
        END_CATCH_ERR
    }
    
    const char* Config_getDisplayColorSpaceName(Config *p, const char* display, const char* view) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getDisplayColorSpaceName(display, view);
        END_CATCH_ERR
    }
    
    const char* Config_getDisplayLooks(Config *p, const char* display, const char* view) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getDisplayLooks(display, view);
        END_CATCH_ERR
    }
    
    void Config_addDisplay(Config *p, const char* display, const char* view, const char* colorSpaceName, const char* looks) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConfigRcPtr*>(p)->get()->addDisplay(display, view, colorSpaceName, looks);
        END_CATCH_ERR
    }
    
    void Config_clearDisplays(Config *p) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConfigRcPtr*>(p)->get()->clearDisplays();
        END_CATCH_ERR
    }
    
    void Config_setActiveDisplays(Config *p, const char* displays) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConfigRcPtr*>(p)->get()->setActiveDisplays(displays);
        END_CATCH_ERR
    }
    
    const char* Config_getActiveDisplays(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getActiveDisplays();
        END_CATCH_ERR
    }
    
    void Config_setActiveViews(Config *p, const char* views) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConfigRcPtr*>(p)->get()->setActiveViews(views);
        END_CATCH_ERR
    }

    const char* Config_getActiveViews(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getActiveViews();
        END_CATCH_ERR
    }

}