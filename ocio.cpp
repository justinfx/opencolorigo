#include <OpenColorIO/OpenColorIO.h>

#include <iostream>
#include <sstream>

#include "ocio.h"


extern "C" {

    namespace OCIO = OCIO_NAMESPACE;

    // Global
    void ClearAllCaches() {
        OCIO::ClearAllCaches();
    }

    const char* GetVersion() {
        return OCIO::GetVersion();
    }

    int GetVersionHex() {
        return OCIO::GetVersionHex();
    }

    // Config Init
    const Config* GetCurrentConfig() {
        return (void*)(OCIO::GetCurrentConfig().get());
    }

    const Config* Config_CreateFromEnv() {
        return (void*)(OCIO::Config::CreateFromEnv().get());
    }

    const Config* Config_CreateFromFile(const char* filename) {
        return (void*)(OCIO::Config::CreateFromFile(filename).get());
    }

    const Config* Config_CreateFromData(const char* data) {
        std::stringstream s;
        s << data;
        return (void*)(OCIO::Config::CreateFromStream(s).get());
    }
    
    const char* Config_getCacheID(Config *p) {
        return reinterpret_cast<OCIO::Config*>(p)->getCacheID();
    }

    const char* Config_getDescription(Config *p) {
        return reinterpret_cast<OCIO::Config*>(p)->getDescription();
    }

    // Config Resources
    const char* Config_getSearchPath(Config *p) {
        return reinterpret_cast<OCIO::Config*>(p)->getSearchPath();
    }

    const char* Config_getWorkingDir(Config *p) {
        return reinterpret_cast<OCIO::Config*>(p)->getWorkingDir();
    }

    // Config ColorSpaces
    int Config_getNumColorSpaces(Config *p) {
        return reinterpret_cast<OCIO::Config*>(p)->getNumColorSpaces();
    }

    const char* Config_getColorSpaceNameByIndex(Config *p, int index) {
        return reinterpret_cast<OCIO::Config*>(p)->getColorSpaceNameByIndex(index);
    }

    int Config_getIndexForColorSpace(Config *p, const char* name) {
        return reinterpret_cast<OCIO::Config*>(p)->getIndexForColorSpace(name);
    }

    bool Config_isStrictParsingEnabled(Config *p) {
        return reinterpret_cast<OCIO::Config*>(p)->isStrictParsingEnabled();
    }

    void Config_setStrictParsingEnabled(Config *p, bool enabled) {
        reinterpret_cast<OCIO::Config*>(p)->setStrictParsingEnabled(enabled);
    }

    // Config Roles
    void Config_setRole(Config *p, const char* role, const char* colorSpaceName) {
        reinterpret_cast<OCIO::Config*>(p)->setRole(role, colorSpaceName);
    }

    int Config_getNumRoles(Config *p) {
        return reinterpret_cast<OCIO::Config*>(p)->getNumRoles();
    }

    bool Config_hasRole(Config *p, const char* role) {
        return reinterpret_cast<OCIO::Config*>(p)->hasRole(role);

    }

    const char* Config_getRoleName(Config *p, int index) {
        return reinterpret_cast<OCIO::Config*>(p)->getRoleName(index);
    }

}