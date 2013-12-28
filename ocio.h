#ifndef _OCIO_OCIO_H_
#define _OCIO_OCIO_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Constants
extern const char* ROLE_DEFAULT;
extern const char* ROLE_REFERENCE;
extern const char* ROLE_DATA;
extern const char* ROLE_COLOR_PICKING;
extern const char* ROLE_SCENE_LINEAR;
extern const char* ROLE_COMPOSITING_LOG;
extern const char* ROLE_COLOR_TIMING;
extern const char* ROLE_TEXTURE_PAINT;
extern const char* ROLE_MATTE_PAINT;

// Enum
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

typedef enum Interpolation {
    INTERP_UNKNOWN = 0,
    INTERP_NEAREST = 1,     //! nearest neighbor in all dimensions
    INTERP_LINEAR = 2,      //! linear interpolation in all dimensions
    INTERP_TETRAHEDRAL = 3, //! tetrahedral interpolation in all directions
    INTERP_BEST = 255       //! the 'best' suitable interpolation type
} Interpolation;

typedef enum EnvironmentMode {
    ENV_ENVIRONMENT_UNKNOWN = 0,
    ENV_ENVIRONMENT_LOAD_PREDEFINED,
    ENV_ENVIRONMENT_LOAD_ALL
} EnvironmentMode;

typedef void Config;
typedef void ColorSpace;
typedef void Context;

// Global
void ClearAllCaches();
const char* GetVersion();
int GetVersionHex();
LoggingLevel GetLoggingLevel();
void SetLoggingLevel(LoggingLevel level);

// Config Init
Config* Config_Create();
const Config* GetCurrentConfig();
void SetCurrentConfig(Config *p);
const Config* Config_CreateFromEnv();
const Config* Config_CreateFromFile(const char* filename);
const Config* Config_CreateFromData(const char* data);
Config* Config_createEditableCopy(Config *p);
void Config_sanityCheck(Config *p);

const char* Config_serialize(Config *p);
const char* Config_getCacheID(Config *p);
const char* Config_getCacheIDWithContext(Config *p, Context *c);
const char* Config_getDescription(Config *p);

// Config Resources
Context* Config_getCurrentContext(Config *p);
const char* Config_getSearchPath(Config *p);
const char* Config_getWorkingDir(Config *p);

// Config ColorSpaces
int Config_getNumColorSpaces(Config *p);
const char* Config_getColorSpaceNameByIndex(Config *p, int index);
const ColorSpace* Config_getColorSpace(Config *p, const char* name);
int Config_getIndexForColorSpace(Config *p, const char* name);
bool Config_isStrictParsingEnabled(Config *p);
void Config_setStrictParsingEnabled(Config *p, bool enabled);
void Config_addColorSpace(Config *p, ColorSpace *cs);
void Config_clearColorSpaces(Config *p);
const char* Config_parseColorSpaceFromString(Config *p, const char* str);

// Config Roles
void Config_setRole(Config *p, const char* role, const char* colorSpaceName);
int Config_getNumRoles(Config *p);
bool Config_hasRole(Config *p, const char* role);
const char* Config_getRoleName(Config *p, int index);

// ColorSpaces
ColorSpace* ColorSpace_Create();
ColorSpace* ColorSpace_createEditableCopy(ColorSpace *p);
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

// Context 
Context* Context_Create();
Context* Context_createEditableCopy(Context *p);
const char* Context_getCacheID(Context *p);
void Context_setSearchPath(Context *p, const char* path);
const char* Context_getSearchPath(Context *p);
void Context_setWorkingDir(Context *p, const char* dirname);
const char* Context_getWorkingDir(Context *p);
void Context_setStringVar(Context *p, const char* name, const char* value);
const char* Context_getStringVar(Context *p, const char* name);
void Context_loadEnvironment(Context *p);
const char* Context_resolveStringVar(Context *p, const char* val);
const char* Context_resolveFileLocation(Context *p, const char* filename);

#ifdef __cplusplus
}
#endif
#endif