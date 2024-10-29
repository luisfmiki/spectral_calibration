# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\spectral_calibration_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\spectral_calibration_autogen.dir\\ParseCache.txt"
  "spectral_calibration_autogen"
  )
endif()
