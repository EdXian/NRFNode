
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was nrf-ble-driverConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

include("${CMAKE_CURRENT_LIST_DIR}/nrf-ble-driverTargets.cmake")



# SD_API_V2 related properties
get_target_property(nrf-ble-driver_SD_API_V2_INCLUDE_DIR nrf::nrf_ble_driver_sd_api_v2_shared INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(nrf-ble-driver_SD_API_V2_LIBRARY nrf::nrf_ble_driver_sd_api_v2_shared LOCATION)
get_target_property(nrf-ble-driver_SD_API_V2_STATIC_LIBRARY nrf::nrf_ble_driver_sd_api_v2_static LOCATION)

# SD_API_V3 related properties
get_target_property(nrf-ble-driver_SD_API_V3_INCLUDE_DIR nrf::nrf_ble_driver_sd_api_v3_shared INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(nrf-ble-driver_SD_API_V3_LIBRARY nrf::nrf_ble_driver_sd_api_v3_shared LOCATION)
get_target_property(nrf-ble-driver_SD_API_V3_STATIC_LIBRARY nrf::nrf_ble_driver_sd_api_v3_static LOCATION)

# SD_API_V5 related properties
get_target_property(nrf-ble-driver_SD_API_V5_INCLUDE_DIR nrf::nrf_ble_driver_sd_api_v5_shared INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(nrf-ble-driver_SD_API_V5_LIBRARY nrf::nrf_ble_driver_sd_api_v5_shared LOCATION)
get_target_property(nrf-ble-driver_SD_API_V5_STATIC_LIBRARY nrf::nrf_ble_driver_sd_api_v5_static LOCATION)

# SD_API_V6 related properties
get_target_property(nrf-ble-driver_SD_API_V6_INCLUDE_DIR nrf::nrf_ble_driver_sd_api_v6_shared INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(nrf-ble-driver_SD_API_V6_LIBRARY nrf::nrf_ble_driver_sd_api_v6_shared LOCATION)
get_target_property(nrf-ble-driver_SD_API_V6_STATIC_LIBRARY nrf::nrf_ble_driver_sd_api_v6_static LOCATION)
