#ifndef _OPENCOLORIGO_COLORSPACE_H
#define _OPENCOLORIGO_COLORSPACE_H

#include "storage.h"
#include <OpenColorIO/OpenColorIO.h>

namespace ocigo {

extern IndexMap<OCIO_NAMESPACE::ColorSpaceRcPtr> g_ColorSpace_map;

} // ocigo

#endif //_OPENCOLORIGO_COLORSPACE_H
