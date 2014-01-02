#include <OpenColorIO/OpenColorIO.h>

#include <iostream>

#include "ocio.h"


#define BEGIN_CATCH_ERR                      \
    errno = 0;                               \
    try {                               

#define END_CATCH_ERR                        \
    }                                        \
    catch (const OCIO::Exception& ex) {      \
        std::cerr << ex.what() << std::endl; \
        errno = ERR_GENERAL;                 \
    }   


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

    void ClearAllCaches() { 
        BEGIN_CATCH_ERR
        OCIO::ClearAllCaches(); 
        END_CATCH_ERR
    }

    const char* GetVersion() { 
        BEGIN_CATCH_ERR
        return OCIO::GetVersion(); 
        END_CATCH_ERR
    }

    int GetVersionHex() { 
        BEGIN_CATCH_ERR
        return OCIO::GetVersionHex(); 
        END_CATCH_ERR
    }

    LoggingLevel GetLoggingLevel() { 
        BEGIN_CATCH_ERR
        return (LoggingLevel)OCIO::GetLoggingLevel(); 
        END_CATCH_ERR
    }

    void SetLoggingLevel(LoggingLevel level) { 
        BEGIN_CATCH_ERR
        OCIO::SetLoggingLevel((OCIO::LoggingLevel)level); 
        END_CATCH_ERR
    };

}