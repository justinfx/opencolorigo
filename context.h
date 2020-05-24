#ifndef _OPENCOLORIGO_CONTEXT_H
#define _OPENCOLORIGO_CONTEXT_H

#include "storage.h"
#include <OpenColorIO/OpenColorIO.h>

namespace ocigo {

extern IndexMap<OCIO_NAMESPACE::ContextRcPtr> g_Context_map;

} // ocigo

#endif //_OPENCOLORIGO_CONTEXT_H
