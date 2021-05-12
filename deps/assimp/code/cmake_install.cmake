# Install script for directory: D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code

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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/lib/Debug/assimp-vc142-mtd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/lib/Release/assimp-vc142-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/lib/MinSizeRel/assimp-vc142-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/lib/RelWithDebInfo/assimp-vc142-mt.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/bin/Debug/assimp-vc142-mtd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/bin/Release/assimp-vc142-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/bin/MinSizeRel/assimp-vc142-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/bin/RelWithDebInfo/assimp-vc142-mt.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/anim.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/aabb.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/ai_assert.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/camera.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/color4.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/color4.inl"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/config.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/defs.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/Defines.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/cfileio.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/light.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/material.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/material.inl"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/matrix3x3.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/matrix3x3.inl"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/matrix4x4.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/matrix4x4.inl"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/mesh.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/pbrmaterial.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/postprocess.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/quaternion.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/quaternion.inl"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/scene.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/metadata.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/texture.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/types.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/vector2.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/vector2.inl"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/vector3.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/vector3.inl"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/version.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/cimport.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/importerdesc.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/Importer.hpp"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/DefaultLogger.hpp"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/ProgressHandler.hpp"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/IOStream.hpp"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/IOSystem.hpp"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/Logger.hpp"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/LogStream.hpp"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/NullLogger.hpp"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/cexport.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/Exporter.hpp"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/DefaultIOStream.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/DefaultIOSystem.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/ZipArchiveIOSystem.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/SceneCombiner.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/fast_atof.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/qnan.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/BaseImporter.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/Hash.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/MemoryIOWrapper.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/ParsingUtils.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/StreamReader.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/StreamWriter.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/StringComparison.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/StringUtils.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/SGSpatialSort.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/GenericProperty.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/SpatialSort.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/SkeletonMeshBuilder.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/SmoothingGroups.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/SmoothingGroups.inl"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/StandardShapes.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/RemoveComments.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/Subdivision.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/Vertex.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/LineSplitter.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/TinyFormatter.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/Profiler.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/LogAux.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/Bitmap.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/XMLTools.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/IOStreamBuffer.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/CreateAnimMesh.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/irrXMLWrapper.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/BlobIOSystem.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/MathFunctions.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/Macros.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/Exceptional.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/ByteSwapper.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/Compiler/pushpack1.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/Compiler/poppack1.h"
    "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/Debug/assimp-vc142-mtd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/GameDev/GitPushing/ct6025-graphics-programming-assessment-Jimbologo/deps/assimp/code/RelWithDebInfo/assimp-vc142-mt.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

