# Install script for directory: D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Assimp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibassimp5.0.0-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/assimp-5.0" TYPE FILE FILES
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/assimp-config.cmake"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/assimp-config-version.cmake"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/assimpTargets.cmake"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/assimpTargets-debug.cmake"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/assimpTargets-release.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibassimp5.0.0-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/assimp.pc")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/contrib/zlib/cmake_install.cmake")
  include("D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/contrib/cmake_install.cmake")
  include("D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/cmake_install.cmake")
  include("D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/tools/assimp_cmd/cmake_install.cmake")
  include("D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/test/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
