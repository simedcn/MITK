/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include "mitkOpenCLActivator.h"

void OpenCLActivator::Load(mitk::ModuleContext *context)
{
  // generate context
  m_ResourceService.reset(new OclResourceServiceImpl);
  mitk::ServiceProperties props;

  context->RegisterService<OclResourceService>(m_ResourceService.get(), props);
}

void OpenCLActivator::Unload(mitk::ModuleContext *)
{
  m_ResourceService.release();
}

US_EXPORT_MODULE_ACTIVATOR(MitkOcl, OpenCLActivator )
