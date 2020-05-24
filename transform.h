#ifndef _OPENCOLORIGO_TRANSFORM_H
#define _OPENCOLORIGO_TRANSFORM_H

#include "storage.h"
#include <OpenColorIO/OpenColorIO.h>

namespace ocigo {

extern IndexMap<OCIO_NAMESPACE::TransformRcPtr> g_Transform_map;

} // ocigo

#endif //_OPENCOLORIGO_TRANSFORM_H
