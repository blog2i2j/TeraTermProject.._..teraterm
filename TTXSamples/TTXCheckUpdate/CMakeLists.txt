﻿project("TTXCheckUpdate")

add_library(
  ${PROJECT_NAME} SHARED
  ttxcheckupdate.c
  parse.h
  parse.cpp
  resource.rc
  resource.h
  #
  ../../libs/cJSON/cJSON.h
  ../../libs/cJSON/cJSON.c
  #
  ../../teraterm/common/codeconv.h
  ../../teraterm/common/dlglib.h
  ../../teraterm/common/getcontent.h
  ${CMAKE_CURRENT_BINARY_DIR}/../../teraterm/common/svnversion.h
  )

if(SUPPORT_OLD_WINDOWS)
  if(MSVC)
    target_sources(
      ${PROJECT_NAME}
      PRIVATE
      ../../teraterm/common/compat_w95_vs2005.c
      )
  endif()
  if(MINGW)
    target_sources(
      ${PROJECT_NAME}
      PRIVATE
      ../../teraterm/libmingw/tlssup.c
      )
    target_link_libraries(
      ${PROJECT_NAME}
      PRIVATE
      -Wl,--whole-archive
      mingw_msvcrt
      -Wl,--no-whole-archive
      )
  endif()
endif(SUPPORT_OLD_WINDOWS)

source_group(
  "common"
  REGULAR_EXPRESSION
  "teraterm\/common\/"
  )

source_group(
  "cJSON"
  REGULAR_EXPRESSION
  "..\/..\/libs\/cJSON\/"
  )

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
  PREFIX ""
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
  )

target_compile_definitions(
  TTXCheckUpdate
  PRIVATE
  -D CJSON_HIDE_SYMBOLS
  )

if(MSVC_VERSION STRLESS 1600)
  # VS2005(1400),2008(1500) (<2010(1600)) special define
  set_source_files_properties(
    ../../libs/cJSON/cJSON.c
    PROPERTIES
    COMPILE_FLAGS "-D NAN=0"
    )
endif()

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
  ../../libs/cJSON
  ../../teraterm/teraterm
)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  common_static
  ttpcmn
  )

if(SUPPORT_OLD_WINDOWS)
  target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    layer_for_unicode
    )
endif(SUPPORT_OLD_WINDOWS)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  wininet.lib
  )

#install(
#  TARGETS ${PROJECT_NAME}
#  RUNTIME
#  DESTINATION .
#)
