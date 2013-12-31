#include <OpenColorIO/OpenColorIO.h>

#include "ocio.h"


extern "C" {

    namespace OCIO = OCIO_NAMESPACE;

    const char* ROLE_DEFAULT            = OCIO::ROLE_DEFAULT;
    const char* ROLE_REFERENCE          = OCIO::ROLE_REFERENCE;
    const char* ROLE_DATA               = OCIO::ROLE_DATA;
    const char* ROLE_COLOR_PICKING      = OCIO::ROLE_COLOR_PICKING;
    const char* ROLE_SCENE_LINEAR       = OCIO::ROLE_SCENE_LINEAR;
    const char* ROLE_COMPOSITING_LOG    = OCIO::ROLE_COMPOSITING_LOG;
    const char* ROLE_COLOR_TIMING       = OCIO::ROLE_COLOR_TIMING;
    const char* ROLE_TEXTURE_PAINT      = OCIO::ROLE_TEXTURE_PAINT;
    const char* ROLE_MATTE_PAINT        = OCIO::ROLE_MATTE_PAINT;

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

    LoggingLevel GetLoggingLevel() { 
        return (LoggingLevel)OCIO::GetLoggingLevel(); 
    }

    void SetLoggingLevel(LoggingLevel level) { 
        OCIO::SetLoggingLevel((OCIO::LoggingLevel)level); 
    };

}