#ifndef _OPENCOLORIGO_OCIO_ABI_H
#define _OPENCOLORIGO_OCIO_ABI_H

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

#endif //_OPENCOLORIGO_OCIO_ABI_H
