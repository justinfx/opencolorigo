#include <OpenColorIO/OpenColorIO.h>

#include <iostream>
#include <string>

#include "ocio.h"
#include "ocio_abi.h"

const char* NO_ERROR = (const char*)"";

_HandleContext* NEW_HANDLE_CONTEXT() {
    return NEW_HANDLE_CONTEXT(0);
}

_HandleContext* NEW_HANDLE_CONTEXT(HandleId handle) {
    _HandleContext *ctx = new _HandleContext;
    ctx->handle = handle;
    ctx->last_error = NULL;
    ctx->has_error = false;
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

    void freeHandleContext(_HandleContext* ctx) {
        if (ctx != NULL) {
            // Specific handle type may have already
            // deleted this for us
            if (ctx->handle) {
                std::cerr << "Warning: OpenColorigo HandleContext handle "
                             "id not deleted when cleaning up HandleContext!"
                             << std::endl;
            }
            free_last_ctx_err(ctx);
            delete ctx;
        }
    }

    bool hasLastError(_HandleContext* ctx) {
        return ctx->has_error;
    }

    const char* getLastError(_HandleContext* ctx) {
        return ctx->last_error;
    }

    void ClearAllCaches() { 
        BEGIN_CATCH_ERR
        OCIO::ClearAllCaches(); 
        END_CATCH_ERR
    }

    const char* GetVersion() {
        const char* ret = NULL;
        BEGIN_CATCH_ERR
        ret = OCIO::GetVersion();
        END_CATCH_ERR
        return ret;
    }

    int GetVersionHex() {
        int ret = 0;
        BEGIN_CATCH_ERR
        ret = OCIO::GetVersionHex();
        END_CATCH_ERR
        return ret;
    }

    LoggingLevel GetLoggingLevel() {
        LoggingLevel lvl;
        BEGIN_CATCH_ERR
        lvl = (LoggingLevel)OCIO::GetLoggingLevel();
        END_CATCH_ERR
        return lvl;
    }

    void SetLoggingLevel(LoggingLevel level) { 
        BEGIN_CATCH_ERR
        OCIO::SetLoggingLevel((OCIO::LoggingLevel)level); 
        END_CATCH_ERR
    };

}