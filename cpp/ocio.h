#ifndef _OCIO_OCIO_H_
#define _OCIO_OCIO_H_

#include <stdbool.h>
#include <errno.h>

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
enum ErrnNo {
    ERR_GENERAL     = -1,
    ERR_BAD_ARGS    = -2,
};

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
typedef void Processor;
typedef void ProcessorMetadata;
typedef void ImageDesc;
typedef void PackedImageDesc;

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

// Config Processors
Processor* Config_getProcessor_CT_CS_CS(Config *p, Context* ct, ColorSpace* srcCS, ColorSpace* dstCS);
Processor* Config_getProcessor_CT_S_S(Config *p, Context* ct, const char* srcName, const char* dstName);
Processor* Config_getProcessor_CS_CS(Config *p, ColorSpace* srcCS, ColorSpace* dstCS);
Processor* Config_getProcessor_S_S(Config *p, const char* srcName, const char* dstName);

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

// Config Display/View Registration 
const char* Config_getDefaultDisplay(Config *p);
int Config_getNumDisplays(Config *p);
const char* Config_getDisplay(Config *p, int index);
const char* Config_getDefaultView(Config *p, const char* display);
int Config_getNumViews(Config *p, const char* display);
const char* Config_getView(Config *p, const char* display, int index);
const char* Config_getDisplayColorSpaceName(Config *p, const char* display, const char* view);
const char* Config_getDisplayLooks(Config *p, const char* display, const char* view);
void Config_addDisplay(Config *p, const char* display, const char* view, const char* colorSpaceName, const char* looks);
void Config_clearDisplays(Config *p);
void Config_setActiveDisplays(Config *p, const char* displays);
const char* Config_getActiveDisplays(Config *p);
void Config_setActiveViews(Config *p, const char* views);
const char* Config_getActiveViews(Config *p);

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

// Processor
Processor* Processor_Create();
bool Processor_isNoOp(Processor *p);
bool Processor_hasChannelCrosstalk(Processor *p);
ProcessorMetadata* Processor_getMetadata(Processor *p);

// Processor CPU
void Processor_apply(Processor *p, ImageDesc *i);
const char* Processor_getCpuCacheID(Processor *p);

ProcessorMetadata* ProcessorMetadata_Create();
int ProcessorMetadata_getNumFiles(ProcessorMetadata *p);
const char* ProcessorMetadata_getFile(ProcessorMetadata *p, int index);
int ProcessorMetadata_getNumLooks(ProcessorMetadata *p);
const char* ProcessorMetadata_getLook(ProcessorMetadata *p, int index);
void ProcessorMetadata_addFile(ProcessorMetadata *p, const char* fname);
void ProcessorMetadata_addLook(ProcessorMetadata *p, const char* look);

// ImageDesc
PackedImageDesc* PackedImageDesc_Create(float* data, long width, long height, long numChannels);
float* PackedImageDesc_getData(PackedImageDesc *p);
long PackedImageDesc_getWidth(PackedImageDesc *p);
long PackedImageDesc_getHeight(PackedImageDesc *p);
long PackedImageDesc_getNumChannels(PackedImageDesc *p);

#ifdef __cplusplus
}
#endif
#endif