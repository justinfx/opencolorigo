#ifndef _OPENCOLORIGO_OCIO_ABI_H
#define _OPENCOLORIGO_OCIO_ABI_H

#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <string.h>
#include <stdlib.h>

// Extending OpenColorABI.h for const_pointer_cast
#if OCIO_USE_BOOST_PTR
#include <boost/shared_ptr.hpp>
#define OCIO_CONST_POINTER_CAST boost::const_pointer_cast
#elif defined(_LIBCPP_VERSION)
#include <memory>
#define OCIO_CONST_POINTER_CAST std::const_pointer_cast
#elif __GNUC__ >= 4
#include <tr1/memory>
#define OCIO_CONST_POINTER_CAST std::tr1::const_pointer_cast
#elif (_MSC_VER > 1600)
#include <memory>
#define OCIO_CONST_POINTER_CAST std::const_pointer_cast
#endif

extern const char* NO_ERROR;


inline void free_last_ctx_err(_HandleContext* ctx) {
    if (ctx == NULL) { return; }
    if (ctx->last_error != NULL && ctx->last_error != NO_ERROR) {
        free((char*)(ctx->last_error));
        ctx->last_error = NULL;
        ctx->has_error = false;
    }
}

/*
BEGIN_CATCH_ERR / END_CATCH_ERR

Macros are used to wrap arbitrary OCIO calls to catch an exception
and signal the failure by way of setting errno to a known value.
It does not capture the error message; only that there was a failure.

Prefer defining types as a _HandleContext and using BEGIN_CATCH_CTX_ERR(ctx)
*/
#define BEGIN_CATCH_ERR                      \
    errno = 0;                               \
    try {


#define END_CATCH_ERR                        \
        errno = 0;                           \
    }                                        \
    catch (const OCIO::Exception& ex) {      \
        errno = ERR_GENERAL;                 \
    }


/*
BEGIN_CATCH_CTX_ERR / END_CATCH_CTX_ERR

Macros are used to wrap OCIO calls in functions that have a _HandleContext
type that can store exception information.

Only some of the types that need to convey error details are defined as
_HandleContext and stored in maps (Config, Context, Processor).
*/
#define BEGIN_CATCH_CTX_ERR(CTX)             \
    errno = 0;                               \
    free_last_ctx_err(CTX);                  \
    try {


#define END_CATCH_CTX_ERR(CTX)               \
        errno = 0;                           \
    }                                        \
    catch (const OCIO::Exception& ex) {      \
        free_last_ctx_err(CTX);              \
        CTX->last_error = strdup(ex.what()); \
        CTX->has_error = true;               \
        errno = ERR_GENERAL;                 \
    }

#endif //_OPENCOLORIGO_OCIO_ABI_H
