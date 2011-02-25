MACRO(MITK_USE_MODULE)
  SET(DEPENDS "")
  SET(DEPENDS_BEFORE "not initialized")
  # check for each parameter if it is a package (3rd party)
  FOREACH(package ${ARGN})
    IF(EXISTS "${MITK_MODULES_PACKAGE_DEPENDS_DIR}/MITK_${package}_Config.cmake") 
      LIST(APPEND PACKAGE_DEPENDS ${package})
    ELSE() 
      LIST(APPEND DEPENDS ${package}) 
    ENDIF() 
  ENDFOREACH(package)
  WHILE(NOT "${DEPENDS}" STREQUAL "${DEPENDS_BEFORE}")
    SET(DEPENDS_BEFORE ${DEPENDS})
    FOREACH(dependency ${DEPENDS})
      INCLUDE(${MITK_MODULES_CONF_DIR}/${dependency}Config.cmake)
      LIST(APPEND DEPENDS ${${dependency}_DEPENDS})
      LIST(APPEND PACKAGE_DEPENDS ${${dependency}_PACKAGE_DEPENDS})
    ENDFOREACH(dependency) 
    IF(DEPENDS)
      LIST(REMOVE_DUPLICATES DEPENDS)
      LIST(SORT DEPENDS)
    ENDIF(DEPENDS)
    IF(PACKAGE_DEPENDS)
      LIST(REMOVE_DUPLICATES PACKAGE_DEPENDS)
      LIST(SORT PACKAGE_DEPENDS)
    ENDIF(PACKAGE_DEPENDS)
  ENDWHILE()
  # CMake Debug
  FOREACH(dependency ${DEPENDS} ${MODULE_DEPENDS_INTERNAL})
    INCLUDE(${MITK_MODULES_CONF_DIR}/${dependency}Config.cmake)
    SET(ALL_INCLUDE_DIRECTORIES ${ALL_INCLUDE_DIRECTORIES} ${${dependency}_INCLUDE_DIRS})
    SET(ALL_LIBRARIES ${ALL_LIBRARIES} ${${dependency}_PROVIDES})
    SET(ALL_LIBRARY_DIRS ${ALL_LIBRARY_DIRS} ${${dependency}_LIBRARY_DIRS})
  ENDFOREACH(dependency)

  FOREACH(package ${PACKAGE_DEPENDS})  
    INCLUDE(${MITK_MODULES_PACKAGE_DEPENDS_DIR}/MITK_${package}_Config.cmake)
    #SET(ALL_INCLUDE_DIRECTORIES ${ALL_INCLUDE_DIRECTORIES} ${${package}_INCLUDE_DIRS})
    # SET(ALL_LIBRARIES ${ALL_LIBRARIES} ${${package}_LIBRARIES})
  ENDFOREACH(package)
  SET(ALL_LIBRARIES ${ALL_LIBRARIES} ${MODULE_ADDITIONAL_LIBS})
  SET(ALL_INCLUDE_DIRECTORIES ${ALL_INCLUDE_DIRECTORIES} ${MODULE_INCLUDE_DIRS} ${MODULE_INTERNAL_INCLUDE_DIRS} ${MITK_MODULES_CONF_DIR})
  IF(ALL_LIBRARY_DIRS)
    LIST(REMOVE_DUPLICATES ALL_LIBRARY_DIRS)
  ENDIF(ALL_LIBRARY_DIRS)

ENDMACRO(MITK_USE_MODULE)