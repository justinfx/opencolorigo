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
        return (Config*) new OCIO::ConstConfigRcPtr(OCIO::GetCurrentConfig());
    }

    void SetCurrentConfig(Config *p) {
        OCIO::SetCurrentConfig(* static_cast<OCIO::ConstConfigRcPtr*>(p));
    }

    const Config* Config_CreateFromEnv() {
        return (Config*) new OCIO::ConstConfigRcPtr(OCIO::Config::CreateFromEnv());
    }

    const Config* Config_CreateFromFile(const char* filename) {
        return (Config*) new OCIO::ConstConfigRcPtr(OCIO::Config::CreateFromFile(filename));
    }

    const Config* Config_CreateFromData(const char* data) {
        std::stringstream s;
        s << data;
        s.seekp(0);
        return (Config*) new OCIO::ConstConfigRcPtr(OCIO::Config::CreateFromStream(s));
    }

    Config* Config_createEditableCopy(Config *p) {
        OCIO::ConfigRcPtr ptr = static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->createEditableCopy();
        return (Config*) new OCIO::ConfigRcPtr(ptr);
    }

    void Config_sanityCheck(Config *p) {
        static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->sanityCheck();
    }

    const char* Config_serialize(Config *p) {
        std::stringstream s;
        static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->serialize(s);
        return s.str().c_str();
    }

    const char* Config_getCacheID(Config *p) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getCacheID();
    }

    const char* Config_getCacheIDWithContext(Config *p, Context *c) {
        OCIO::ConstContextRcPtr context = *(static_cast<OCIO::ConstContextRcPtr*>(c));
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getCacheID(context);
    }

    const char* Config_getDescription(Config *p) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getDescription();
    }

    // Config Resources
    Context* Config_getCurrentContext(Config *p) {
        OCIO::ConstContextRcPtr ptr = static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getCurrentContext();
        return (Context*) new OCIO::ConstContextRcPtr(ptr);
    }

    const char* Config_getSearchPath(Config *p) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getSearchPath();
    }

    const char* Config_getWorkingDir(Config *p) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getWorkingDir();
    }

    // Config Processors 
    Processor* Config_getProcessor_CT_CS_CS(Config *p, Context* ct, ColorSpace* srcCS, ColorSpace* dstCS) {
        OCIO::ConstContextRcPtr ct_ptr       = * static_cast<OCIO::ConstContextRcPtr*>(ct);
        OCIO::ConstColorSpaceRcPtr srcCS_ptr = * static_cast<OCIO::ConstColorSpaceRcPtr*>(srcCS);
        OCIO::ConstColorSpaceRcPtr dstCS_ptr = * static_cast<OCIO::ConstColorSpaceRcPtr*>(dstCS);

        return (Processor*) new OCIO::ConstProcessorRcPtr(
            static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getProcessor(ct_ptr, srcCS_ptr, dstCS_ptr));
    }

    Processor* Config_getProcessor_CS_CS(Config *p, ColorSpace* srcCS, ColorSpace* dstCS) {
        OCIO::ConstColorSpaceRcPtr srcCS_ptr = * static_cast<OCIO::ConstColorSpaceRcPtr*>(srcCS);
        OCIO::ConstColorSpaceRcPtr dstCS_ptr = * static_cast<OCIO::ConstColorSpaceRcPtr*>(dstCS);
        
        return (Processor*) new OCIO::ConstProcessorRcPtr(
            static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getProcessor(srcCS_ptr, dstCS_ptr));
    }

    Processor* Config_getProcessor_S_S(Config *p, const char* srcName, const char* dstName) {
        return (Processor*) new OCIO::ConstProcessorRcPtr(
            static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getProcessor(srcName, dstName));
    }

    Processor* Config_getProcessor_CT_S_S(Config *p, Context* ct, const char* srcName, const char* dstName) {
        OCIO::ConstContextRcPtr ct_ptr = * static_cast<OCIO::ConstContextRcPtr*>(ct);
        return (Processor*) new OCIO::ConstProcessorRcPtr(
            static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getProcessor(ct_ptr, srcName, dstName));
    }

    // Config ColorSpaces
    int Config_getNumColorSpaces(Config *p) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getNumColorSpaces();
    }

    const char* Config_getColorSpaceNameByIndex(Config *p, int index) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getColorSpaceNameByIndex(index);
    }

    const ColorSpace* Config_getColorSpace(Config *p, const char* name) {
        OCIO::ConstColorSpaceRcPtr ptr = static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getColorSpace(name);
        return (ColorSpace*) new OCIO::ConstColorSpaceRcPtr(ptr);
    }

    int Config_getIndexForColorSpace(Config *p, const char* name) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getIndexForColorSpace(name);
    }

    void Config_addColorSpace(Config *p, ColorSpace *cs) {
        OCIO::ConstColorSpaceRcPtr colorspace = *(static_cast<OCIO::ConstColorSpaceRcPtr*>(cs));
        static_cast<OCIO::ConfigRcPtr*>(p)->get()->addColorSpace(colorspace);
    }

    void Config_clearColorSpaces(Config *p) {
        static_cast<OCIO::ConfigRcPtr*>(p)->get()->clearColorSpaces();
    }

    const char* Config_parseColorSpaceFromString(Config *p, const char* str) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->parseColorSpaceFromString(str);
    }

    bool Config_isStrictParsingEnabled(Config *p) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->isStrictParsingEnabled();
    }

    void Config_setStrictParsingEnabled(Config *p, bool enabled) {
        static_cast<OCIO::ConfigRcPtr*>(p)->get()->setStrictParsingEnabled(enabled);
    }

    // Config Roles
    void Config_setRole(Config *p, const char* role, const char* colorSpaceName) {
        static_cast<OCIO::ConfigRcPtr*>(p)->get()->setRole(role, colorSpaceName);
    }

    int Config_getNumRoles(Config *p) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getNumRoles();
    }

    bool Config_hasRole(Config *p, const char* role) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->hasRole(role);
    }

    const char* Config_getRoleName(Config *p, int index) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getRoleName(index);
    }

}