#include <OpenColorIO/OpenColorIO.h>

#include <iostream>
#include <sstream>

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