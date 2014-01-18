#include <OpenColorIO/OpenColorIO.h>

#include <iostream>
#include <string>

#include "ocio.h"


#define BEGIN_CATCH_ERR                      \
    errno = 0;                               \
    try {                                   


#define END_CATCH_ERR                        \
    }                                        \
    catch (const OCIO::Exception& ex) {      \
        errno = ERR_GENERAL;                 \
    } 


#define END_CATCH_CTX_ERR(CTX)               \
    }                                        \
    catch (const OCIO::Exception& ex) {      \
        if (CTX->last_error != NULL &&       \
            CTX->last_error != NO_ERROR) {   \
            free(CTX->last_error);           \
        }                                    \
        CTX->last_error = strdup(ex.what()); \
        errno = ERR_GENERAL;                 \
    } 


static char* NO_ERROR = (char*)"";


_Context* NEW_CONTEXT(void* handle=NULL) {
    _Context *ctx = new _Context;
    ctx->handle = handle;
    ctx->last_error = NULL;
    return ctx;
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

    void freeContext(_Context* ctx) {
        if (ctx != NULL) {
            if (ctx->handle != NULL) {
                free(ctx->handle);
            }  
            if (ctx->last_error != NULL && ctx->last_error != NO_ERROR) {
                free(ctx->last_error);
            }  
            free(ctx);      
        }
    }

    char* getLastError(_Context* ctx) {
        return ctx->last_error;
    }

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

    // const char* GetLastError() {
    //     (void) pthread_once(&last_err_once, make_last_err);
    //     char *err= (char*) pthread_getspecific(last_err);
    //     return err;
    // }

}