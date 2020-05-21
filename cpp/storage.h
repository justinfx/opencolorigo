#ifndef _OPENCOLORIGO_STORAGE_H
#define _OPENCOLORIGO_STORAGE_H

#include <OpenColorIO/OpenColorIO.h>

#include "shared_ptr_map.h"

namespace OCIO = OCIO_NAMESPACE;

namespace ocigo {

extern SharedPtrMap<OCIO::ConfigRcPtr> g_Config_map;
extern SharedPtrMap<OCIO::ProcessorRcPtr> g_Processor_map;
extern SharedPtrMap<OCIO::ContextRcPtr> g_Context_map;
extern SharedPtrMap<OCIO::ColorSpaceRcPtr> g_ColorSpace_map;
extern SharedPtrMap<OCIO::ProcessorMetadataRcPtr> g_ProcessorMetadata_map;
extern SharedPtrMap<OCIO::TransformRcPtr> g_Transform_map;

}

#endif //_OPENCOLORIGO_STORAGE_H
