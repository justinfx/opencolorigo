#ifndef _OPENCOLORIGO_OCIO_H_
#define _OPENCOLORIGO_OCIO_H_

#include <stdint.h>
#include <stdlib.h>
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

typedef enum TransformDirection {
    TRANSFORM_DIR_UNKNOWN = 0,
    TRANSFORM_DIR_FORWARD,
    TRANSFORM_DIR_INVERSE
} TransformDirection;

typedef uint64_t HandleId;

typedef struct _HandleContext {
    HandleId handle;
    const char* last_error;
    bool has_error;
} _HandleContext;

// typedef void Config;
typedef _HandleContext Config;
typedef HandleId ColorSpaceId;
typedef _HandleContext* ContextId;
typedef _HandleContext* ProcessorId;
typedef HandleId ProcessorMetadataId;
typedef void ImageDesc;
typedef void PackedImageDesc;
typedef HandleId TransformId;
typedef HandleId DisplayTransformId;

void freeHandleContext(_HandleContext* ctx);
bool hasLastError(_HandleContext* ctx);
const char* getLastError(_HandleContext* ctx);

// Global
void ClearAllCaches();
const char* GetVersion();
int GetVersionHex();
LoggingLevel GetLoggingLevel();
void SetLoggingLevel(LoggingLevel level);

// Config Init
void deleteConfig(Config *p);
Config* Config_Create();
Config* GetCurrentConfig();
void SetCurrentConfig(Config *p);
Config* Config_CreateFromEnv();
Config* Config_CreateFromFile(const char* filename);
Config* Config_CreateFromData(const char* data);
Config* Config_createEditableCopy(Config *p);
void Config_sanityCheck(Config *p);

char* Config_serialize(Config *p);
const char* Config_getCacheID(Config *p);
const char* Config_getCacheIDWithContext(Config *p, ContextId c);
const char* Config_getDescription(Config *p);

// Config Resources
ContextId Config_getCurrentContext(Config *p);
const char* Config_getSearchPath(Config *p);
const char* Config_getWorkingDir(Config *p);

// Config Processors
ProcessorId Config_getProcessor_CT_CS_CS(Config* p, ContextId ct, ColorSpaceId srcCS, ColorSpaceId dstCS);
ProcessorId Config_getProcessor_CT_S_S(Config* p, ContextId ct, const char* srcName, const char* dstName);
ProcessorId Config_getProcessor_CS_CS(Config* p, ColorSpaceId srcCS, ColorSpaceId dstCS);
ProcessorId Config_getProcessor_S_S(Config* p, const char* srcName, const char* dstName);
ProcessorId Config_getProcessor_TX(Config* p, TransformId tx);
ProcessorId Config_getProcessor_TX_D(Config* p, TransformId tx, TransformDirection direction);
ProcessorId Config_getProcessor_CT_TX_D(Config* p, ContextId ct, TransformId tx, TransformDirection direction);

// Config ColorSpaces
void deleteColorSpace(ColorSpaceId p);
int Config_getNumColorSpaces(Config *p);
const char* Config_getColorSpaceNameByIndex(Config *p, int index);
ColorSpaceId Config_getColorSpace(Config *p, const char* name);
int Config_getIndexForColorSpace(Config *p, const char* name);
bool Config_isStrictParsingEnabled(Config *p);
void Config_setStrictParsingEnabled(Config *p, bool enabled);
void Config_addColorSpace(Config *p, ColorSpaceId cs);
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
ColorSpaceId ColorSpace_Create();
ColorSpaceId ColorSpace_createEditableCopy(ColorSpaceId p);
const char* ColorSpace_getName(ColorSpaceId p);
void ColorSpace_setName(ColorSpaceId p, const char* name);
const char* ColorSpace_getFamily(ColorSpaceId p);
void ColorSpace_setFamily(ColorSpaceId p, const char* family);
const char* ColorSpace_getEqualityGroup(ColorSpaceId p);
void ColorSpace_setEqualityGroup(ColorSpaceId p, const char* group);
const char* ColorSpace_getDescription(ColorSpaceId p);
void ColorSpace_setDescription(ColorSpaceId p, const char* description);
BitDepth ColorSpace_getBitDepth(ColorSpaceId p);
void ColorSpace_setBitDepth(ColorSpaceId p, BitDepth bitDepth);

// Context
void deleteContext(ContextId p);
ContextId Context_Create();
ContextId Context_createEditableCopy(ContextId p);
const char* Context_getCacheID(ContextId p);
void Context_setSearchPath(ContextId p, const char* path);
const char* Context_getSearchPath(ContextId p);
void Context_setWorkingDir(ContextId p, const char* dirname);
const char* Context_getWorkingDir(ContextId p);
void Context_setStringVar(ContextId p, const char* name, const char* value);
const char* Context_getStringVar(ContextId p, const char* name);
void Context_loadEnvironment(ContextId p);
EnvironmentMode Context_getEnvironmentMode(ContextId p);
void Context_setEnvironmentMode(ContextId p, EnvironmentMode mode);
const char* Context_resolveStringVar(ContextId p, const char* val);
const char* Context_resolveFileLocation(ContextId p, const char* filename);

// Processor
void deleteProcessor(ProcessorId p);
ProcessorId Processor_Create();
bool Processor_isNoOp(ProcessorId p);
bool Processor_hasChannelCrosstalk(ProcessorId p);
ProcessorMetadataId Processor_getMetadata(ProcessorId p);

// Processor CPU
void Processor_apply(ProcessorId p, ImageDesc *i);
const char* Processor_getCpuCacheID(ProcessorId p);

void deleteProcessorMetadata(ProcessorMetadataId p);
ProcessorMetadataId ProcessorMetadata_Create();
int ProcessorMetadata_getNumFiles(ProcessorMetadataId p);
const char* ProcessorMetadata_getFile(ProcessorMetadataId p, int index);
int ProcessorMetadata_getNumLooks(ProcessorMetadataId p);
const char* ProcessorMetadata_getLook(ProcessorMetadataId p, int index);
void ProcessorMetadata_addFile(ProcessorMetadataId p, const char* fname);
void ProcessorMetadata_addLook(ProcessorMetadataId p, const char* look);

// ImageDesc
void deletePackedImageDesc(PackedImageDesc* p);
PackedImageDesc* PackedImageDesc_Create(float* data, long width, long height, long numChannels);
float* PackedImageDesc_getData(PackedImageDesc *p);
long PackedImageDesc_getWidth(PackedImageDesc *p);
long PackedImageDesc_getHeight(PackedImageDesc *p);
long PackedImageDesc_getNumChannels(PackedImageDesc *p);

// DisplayTransform
void deleteDisplayTransform(DisplayTransformId p);
DisplayTransformId DisplayTransform_Create();
DisplayTransformId DisplayTransform_createEditableCopy(DisplayTransformId p);
TransformDirection DisplayTransform_getDirection(DisplayTransformId p);
void DisplayTransform_setDirection(DisplayTransformId p, TransformDirection dir);
const char* DisplayTransform_getInputColorSpaceName(DisplayTransformId p);
void DisplayTransform_setInputColorSpaceName(DisplayTransformId p, const char* name);
const char* DisplayTransform_getDisplay(DisplayTransformId p);
void DisplayTransform_setDisplay(DisplayTransformId p, const char* name);
const char* DisplayTransform_getView(DisplayTransformId p);
void DisplayTransform_setView(DisplayTransformId p, const char* name);
const char* DisplayTransform_getLooksOverride(DisplayTransformId p);
void DisplayTransform_setLooksOverride(DisplayTransformId p, const char* looks);
bool DisplayTransform_getLooksOverrideEnabled(DisplayTransformId p);
void DisplayTransform_setLooksOverrideEnabled(DisplayTransformId p, bool enabled);

#ifdef __cplusplus
}
#endif

_HandleContext* NEW_HANDLE_CONTEXT();
_HandleContext* NEW_HANDLE_CONTEXT(HandleId handle);

#endif // _OPENCOLORIGO_OCIO_H_