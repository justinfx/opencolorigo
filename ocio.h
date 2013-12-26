#ifndef _OCIO_OCIO_H_
#define _OCIO_OCIO_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Global
void ClearAllCaches();
const char* GetVersion();
int GetVersionHex();

// Config
typedef void Config;

// Config Init
const Config* GetCurrentConfig();
const Config* Config_CreateFromEnv();
const Config* Config_CreateFromFile(const char* filename);
const Config* Config_CreateFromData(const char* data);

const char* Config_getCacheID(Config *p);
const char* Config_getDescription(Config *p);

// Config Resources
const char* Config_getSearchPath(Config *p);
const char* Config_getWorkingDir(Config *p);

// Config ColorSpaces
typedef void ColorSpace;

int Config_getNumColorSpaces(Config *p);
const char* Config_getColorSpaceNameByIndex(Config *p, int index);
const ColorSpace* Config_getColorSpace(Config *p, const char* name);
int Config_getIndexForColorSpace(Config *p, const char* name);
bool Config_isStrictParsingEnabled(Config *p);
void Config_setStrictParsingEnabled(Config *p, bool enabled);

// Config Roles
void Config_setRole(Config *p, const char* role, const char* colorSpaceName);
int Config_getNumRoles(Config *p);
bool Config_hasRole(Config *p, const char* role);
const char* Config_getRoleName(Config *p, int index);

#ifdef __cplusplus
}
#endif
#endif