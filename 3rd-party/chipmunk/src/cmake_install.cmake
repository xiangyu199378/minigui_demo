# Install script for directory: /home/yu/work/build-minigui-5.0/3rd-party/chipmunk/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libchipmunk.so.5.1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libchipmunk.so.5.1")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libchipmunk.so.5.1"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/src/libchipmunk.so.5.1")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libchipmunk.so.5.1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libchipmunk.so.5.1")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libchipmunk.so.5.1")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libchipmunk.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libchipmunk.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libchipmunk.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/src/libchipmunk.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libchipmunk.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libchipmunk.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libchipmunk.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/src/libchipmunk.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/chipmunk" TYPE FILE FILES
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/chipmunk.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/chipmunk_ffi.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/chipmunk_private.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/chipmunk_types.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/chipmunk_unsafe.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/cpArbiter.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/cpArray.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/cpBB.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/cpBody.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/cpCollision.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/cpHashSet.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/cpPolyShape.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/cpShape.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/cpSpace.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/cpSpaceHash.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/cpVect.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/chipmunk/constraints" TYPE FILE FILES
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/constraints/cpConstraint.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/constraints/cpDampedRotarySpring.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/constraints/cpDampedSpring.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/constraints/cpGearJoint.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/constraints/cpGrooveJoint.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/constraints/cpPinJoint.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/constraints/cpPivotJoint.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/constraints/cpRatchetJoint.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/constraints/cpRotaryLimitJoint.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/constraints/cpSimpleMotor.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/constraints/cpSlideJoint.h"
    "/home/yu/work/build-minigui-5.0/3rd-party/chipmunk/include/chipmunk/constraints/util.h"
    )
endif()

