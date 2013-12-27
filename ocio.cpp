#include <OpenColorIO/OpenColorIO.h>

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

    LoggingLevel GetLoggingLevel() { 
        return (LoggingLevel)OCIO::GetLoggingLevel(); 
    }

    void SetLoggingLevel(LoggingLevel level) { 
        OCIO::SetLoggingLevel((OCIO::LoggingLevel)level); 
    };

}