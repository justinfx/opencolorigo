#ifndef _OCIO_OCIO_H_
#define _OCIO_OCIO_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum LoggingLevel {
    LOGGING_LEVEL_NONE = 0,
    LOGGING_LEVEL_WARNING = 1,
    LOGGING_LEVEL_INFO = 2,
    LOGGING_LEVEL_DEBUG = 3,
    LOGGING_LEVEL_UNKNOWN = 255
} LoggingLevel;

typedef enum BitDepth {
    BIT_DEPTH_UNKNOWN = 0,
    BIT_DEPTH_UINT8,
    BIT_DEPTH_UINT10,
    BIT_DEPTH_UINT12,
    BIT_DEPTH_UINT14,
    BIT_DEPTH_UINT16,
    BIT_DEPTH_UINT32,
    BIT_DEPTH_F16,
    BIT_DEPTH_F32
} BitDepth;

typedef void Config;
typedef void ColorSpace;

// Global
void ClearAllCaches();
const char* GetVersion();
int GetVersionHex();
LoggingLevel GetLoggingLevel();
void SetLoggingLevel(LoggingLevel level);

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

// ColorSpaces
const char* ColorSpace_getName(ColorSpace *p);
void ColorSpace_setName(ColorSpace *p, const char* name);
const char* ColorSpace_getFamily(ColorSpace *p);
void ColorSpace_setFamily(ColorSpace *p, const char* family);
const char* ColorSpace_getEqualityGroup(ColorSpace *p);
void ColorSpace_setEqualityGroup(ColorSpace *p, const char* group);
const char* ColorSpace_getDescription(ColorSpace *p);
void ColorSpace_setDescription(ColorSpace *p, const char* description);
BitDepth ColorSpace_getBitDepth(ColorSpace *p);
void ColorSpace_setBitDepth(ColorSpace *p, BitDepth bitDepth);

#ifdef __cplusplus
}
#endif
#endif