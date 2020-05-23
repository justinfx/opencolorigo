#ifndef _OPENCOLORIGO_PROCESSOR_H
#define _OPENCOLORIGO_PROCESSOR_H

#include "storage.h"
#include <OpenColorIO/OpenColorIO.h>

namespace ocigo {

extern IndexMap<OCIO_NAMESPACE::ProcessorRcPtr> g_Processor_map;
extern IndexMap<OCIO_NAMESPACE::ProcessorMetadataRcPtr> g_ProcessorMetadata_map;

} // ocigo

#endif //_OPENCOLORIGO_PROCESSOR_H
