#-----------------------------------------------------------------------------
# US source files
#-----------------------------------------------------------------------------

set(_srcs
  util/usAny.cpp
  util/usUncompressResourceData.c
  util/usUncompressResourceData.cpp
  util/usThreads.cpp
  util/usUtils.cpp

  service/usLDAPExpr.cpp
  service/usLDAPFilter.cpp
  service/usServiceException.cpp
  service/usServiceEvent.cpp
  service/usServiceListenerEntry.cpp
  service/usServiceListenerEntry_p.h
  service/usServiceListeners.cpp
  service/usServiceListeners_p.h
  service/usServiceProperties.cpp
  service/usServiceReference.cpp
  service/usServiceReferencePrivate.cpp
  service/usServiceRegistration.cpp
  service/usServiceRegistrationPrivate.cpp
  service/usServiceRegistry.cpp
  service/usServiceRegistry_p.h

  module/usCoreModuleContext_p.h
  module/usCoreModuleContext.cpp
  module/usModuleContext.cpp
  module/usModule.cpp
  module/usModuleEvent.cpp
  module/usModuleInfo.cpp
  module/usModulePrivate.cpp
  module/usModuleRegistry.cpp
  module/usModuleResource.cpp
  module/usModuleResourceBuffer.cpp
  module/usModuleResourceStream.cpp
  module/usModuleResourceTree.cpp
  module/usModuleSettings.cpp
  module/usModuleUtils.cpp
  module/usModuleVersion.cpp
)

set(_private_headers
  util/usAtomicInt_p.h
  util/usFunctor_p.h
  util/usStaticInit_p.h
  util/usThreads_p.h
  util/usUtils_p.h

  util/dirent_win32_p.h
  util/stdint_p.h
  util/stdint_vc_p.h

  service/usServiceTracker.tpp
  service/usServiceTrackerPrivate.h
  service/usServiceTrackerPrivate.tpp
  service/usTrackedService_p.h
  service/usTrackedServiceListener_p.h
  service/usTrackedService.tpp

  module/usModuleAbstractTracked_p.h
  module/usModuleAbstractTracked.tpp
  module/usModuleResourceBuffer_p.h
  module/usModuleResourceTree_p.h
  module/usModuleUtils_p.h
)

set(_public_headers
  util/usAny.h
  util/usSharedData.h
  util/usUncompressResourceData.h

  service/usLDAPFilter.h
  service/usServiceEvent.h
  service/usServiceException.h
  service/usServiceInterface.h
  service/usServiceProperties.h
  service/usServiceReference.h
  service/usServiceRegistration.h
  service/usServiceTracker.h
  service/usServiceTrackerCustomizer.h

  module/usGetModuleContext.h
  module/usModule.h
  module/usModuleActivator.h
  module/usModuleContext.h
  module/usModuleEvent.h
  module/usModuleImport.h
  module/usModuleInfo.h
  module/usModuleInitialization.h
  module/usModuleRegistry.h
  module/usModuleResource.h
  module/usModuleResourceStream.h
  module/usModuleSettings.h
  module/usModuleVersion.h
)

if(_us_baseclass_default)
  list(APPEND _public_headers util/usBase.h)
endif()

if(US_ENABLE_SERVICE_FACTORY_SUPPORT)
  list(APPEND _public_headers service/usServiceFactory.h)
endif()

if(US_IS_EMBEDDED)
  set(US_SOURCES )
  get_filename_component(_path_prefix "${PROJECT_SOURCE_DIR}" NAME)
  set(_path_prefix "${_path_prefix}/src")
  foreach(_src ${_srcs} ${_public_headers} ${_private_headers})
    list(APPEND US_SOURCES ${_path_prefix}/${_src})
  endforeach()
  set(US_SOURCES ${US_SOURCES} PARENT_SCOPE)
endif()

#-----------------------------------------------------------------------------
# Create library (only if not in embedded mode)
#-----------------------------------------------------------------------------

if(NOT US_IS_EMBEDDED)
  include_directories(${US_INTERNAL_INCLUDE_DIRS})
  if(US_LINK_DIRS)
    link_directories(${US_LINK_DIRS})
  endif()

  usFunctionGenerateModuleInit(_srcs NAME ${PROJECT_NAME} VERSION "0.9.0")

  add_library(${PROJECT_NAME} ${_srcs}
              ${_public_headers} ${_private_headers} ${us_config_h_file})
  if(NOT US_IS_EMBEDDED AND US_LINK_FLAGS)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${US_LINK_FLAGS}")
  endif()
  set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY COMPILE_DEFINITIONS US_FORCE_MODULE_INIT)
  set_property(TARGET ${PROJECT_NAME} PROPERTY FRAMEWORK 1)
  if(US_LINK_LIBRARIES)
    target_link_libraries(${PROJECT_NAME} ${_link_libraries})
  endif()
endif()

#-----------------------------------------------------------------------------
# Configure public header wrappers
#-----------------------------------------------------------------------------

set(US_PUBLIC_HEADERS ${_public_headers})
if(US_HEADER_PREFIX)
  set(US_PUBLIC_HEADERS )
  foreach(_public_header ${_public_headers})
    get_filename_component(_public_header_basename ${_public_header} NAME_WE)
    set(_us_public_header ${_public_header_basename}.h)
    string(SUBSTRING "${_public_header_basename}" 2 -1 _public_header_basename)
    set(_header_wrapper "${PROJECT_BINARY_DIR}/include/${US_HEADER_PREFIX}${_public_header_basename}.h")
    configure_file(${PROJECT_SOURCE_DIR}/CMake/usPublicHeaderWrapper.h.in ${_header_wrapper} @ONLY)
    list(APPEND US_PUBLIC_HEADERS ${_header_wrapper})
  endforeach()
endif()

foreach(_header ${_public_headers} ${_private_headers})
  get_filename_component(_header_name "${_header}" NAME)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${_header} "${PROJECT_BINARY_DIR}/include/${_header_name}")
endforeach()

if(NOT US_IS_EMBEDDED)
  set_property(TARGET ${PROJECT_NAME} PROPERTY PUBLIC_HEADER ${US_PUBLIC_HEADERS})
  set_property(TARGET ${PROJECT_NAME} PROPERTY PRIVATE_HEADER ${_private_headers} ${us_config_h_file})
else()
  set(US_PUBLIC_HEADERS ${US_PUBLIC_HEADERS} PARENT_SCOPE)
  set(US_PRIVATE_HEADERS ${US_PRIVATE_HEADERS} PARENT_SCOPE)
endif()

#-----------------------------------------------------------------------------
# Install support (only if not in embedded mode)
#-----------------------------------------------------------------------------

if(NOT US_IS_EMBEDDED)
  install(TARGETS ${PROJECT_NAME} FRAMEWORK DESTINATION .
                                  RUNTIME DESTINATION bin
                                  LIBRARY DESTINATION lib
                                  ARCHIVE DESTINATION lib
                                  PUBLIC_HEADER DESTINATION include
                                  PRIVATE_HEADER DESTINATION include)
endif()

