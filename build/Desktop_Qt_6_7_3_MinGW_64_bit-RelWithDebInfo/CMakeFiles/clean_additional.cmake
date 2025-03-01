# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "RelWithDebInfo")
  file(REMOVE_RECURSE
  "CMakeFiles\\Integartion_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Integartion_autogen.dir\\ParseCache.txt"
  "Integartion_autogen"
  )
endif()
