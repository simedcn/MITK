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
#ifndef __mitkKinectDeviceFactory_h
#define __mitkKinectDeviceFactory_h

#include "mitkKinectModuleExports.h"
#include "mitkKinectDevice.h"
#include "mitkAbstractToFDeviceFactory.h"
#include <mitkCameraIntrinsics.h>
#include <mitkCameraIntrinsicsProperty.h>

namespace mitk
{
  /**
  * \brief KinectDeviceFactory is an implementation of the factory pattern to generate Microsoft Kinect devices.
  * KinectDeviceFactory inherits from AbstractToFDeviceFactory which is a MicroService interface.
  * This offers users the oppertunity to generate new KinectDevices via a global instance of this factory.
  * @ingroup ToFHardware
  */
  class MITK_KINECTMODULE_EXPORT KinectDeviceFactory : public itk::LightObject, public AbstractToFDeviceFactory {

  public:

    KinectDeviceFactory()
    {
      this->m_DeviceNumber = 1;
    }
    /*!
    \brief Defining the Factorie�s Name, here for the Kinect.
    */
    std::string GetFactoryName()
    {
      return std::string("Kinect Factory");
    }
    //Interating the Device name on calling the Factory
    std::string GetCurrentDeviceName()
    {
      std::stringstream name;
      if (m_DeviceNumber>1)
      {
        name << "Kinect " << m_DeviceNumber;
      }
      else
      {
        name << "Kinect ";
      }
      m_DeviceNumber++;
      return name.str();
    }

  private:

    /*!
    \brief Create an instance of a KinectDevice.
    */
    ToFCameraDevice::Pointer createToFCameraDevice()
    {
      KinectDevice::Pointer device = KinectDevice::New();

      //Set default camera intrinsics for the kinect RGB camera.
      //(OpenNI warps the distance data into the RGB space).
      mitk::CameraIntrinsics::Pointer cameraIntrinsics = mitk::CameraIntrinsics::New();
      std::string pathToDefaulCalibrationFile(MITK_TOF_DATA_DIR);
      pathToDefaulCalibrationFile.append("/CalibrationFiles/Kinect_RGB_camera.xml");
      cameraIntrinsics->FromXMLFile(pathToDefaulCalibrationFile);
      device->SetProperty("CameraIntrinsics", mitk::CameraIntrinsicsProperty::New(cameraIntrinsics));

      device->SetBoolProperty("HasRGBImage", true);
      device->SetBoolProperty("HasAmplitudeImage", false);
      device->SetBoolProperty("HasIntensityImage", false);
      device->SetBoolProperty("KinectReconstructionMode", true);

      return device.GetPointer();
    }
    //Member variable as variable for our DeviceNumber
    int m_DeviceNumber;
  };
}
#endif
