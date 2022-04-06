cmake_minimum_required(VERSION 3.5)

if (WIN32)
project(QtGui2 VERSION 0.1 LANGUAGES CXX)
elseif (UNIX)
     project(QtGui2 VERSION 0.1)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network REQUIRED)

#add_subdirectory(g2clib-1.6.0)
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        DataDefines.h
        DataQString.cpp
        DataQString.h
        DataMeteoAbstract.h
        DataMeteoAbstract.cpp
        Util.cpp
        Util.h
        zuFile.cpp
        zuFile.h
        Therm.cpp
        Therm.h
        GriddedRecord.cpp
        GriddedRecord.h
        GriddedReader.cpp
        GriddedReader.h
        GridType.h
        RegularGridded.h
        GribRecord.cpp
        GribRecord.h
        GribReader.cpp
        GribReader.h
        Settings.cpp
        Settings.h
        Version.h
        Grib2Record.cpp
        Grib2Record.h
        DataPointInfo.cpp
        DataPointInfo.h
        LongTaskMessage.cpp
        LongTaskMessage.h
        LongTaskProgress.cpp
        LongTaskProgress.h
        GribReaderTop.cpp
        GribReaderTop.h
        drstemplates.h
        grib2.h
        gridtemplates.h
        pdstemplates.h
        #cmplxpack.c
        #compack.c
        comunpack.c
        dec_jpeg2000.c
        dec_png.c
        drstemplates.c
        #enc_jpeg2000.c
        #enc_png.c
        #g2_addfield.c
        #g2_addgrid.c
        #g2_addlocal.c
        #g2_create.c
        g2_free.c
        g2_getfld.c
        g2_gribend.c
        g2_info.c
        g2_miss.c
        g2_unpack1.c
        g2_unpack2.c
        g2_unpack3.c
        g2_unpack4.c
        g2_unpack5.c
        g2_unpack6.c
        g2_unpack7.c
        gbits.c
        getdim.c
        getpoly.c
        gridtemplates.c
        int_power.c
        #jpcpack.c
        jpcunpack.c
        jpeg2000_openjpeg.c
        #main.c
        #mainhome.c
        #misspack.c
        #mkieee.c
        #pack_gp.c
        pdstemplates.c
        #pngpack.c
        pngunpack.c
        rdieee.c
        #reduce.c
        seekgb.c
        #simpack.c
        simunpack.c
        #specpack.c
        specunpack.c
)

add_library(g2clib STATIC )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QtGui2
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QtGui2 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QtGui2 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QtGui2
            ${PROJECT_SOURCES}
        )
    endif()
endif()

find_library(LIBNOVA_LIBRARY
     NAMES "libnova.a" "libnova.so"
     PATHS
     $ENV{EXTERNLIBS}/lib
     ~/Library/Frameworks
     /Library/Frameworks
     /usr/local
     /usr
     /sw
     /opt/local
     /opt/csw
     /opt
     PATH_SUFFIXES lib lib64 lib/x86_64-linux-gnu lib/i386-linux-gnu lib/armhf-linux-gnu lib/arm64-linux-gnu
     REQUIRED
 )
find_path(LIBNOVA_INCLUDE_DIR
     NAMES "libnova/libnova.h" "libnova.h"
     PATHS
     $ENV{EXTERNLIBS}/include
     ~/Library/Frameworks/include
     /Library/Frameworks/include
     /usr/local/include
     /usr/include
     /sw/include # Fink
     /opt/local/include # DarwinPorts
     /opt/csw/include # Blastwave
     /opt/include
     REQUIRED
 )
include_directories(${LIBNOVA_INCLUDE_DIR})

#find_package(OpenJPEG REQUIRED)
find_library(OPENJPEG_LIBRARIES
#     NAMES "libopenjp2.a" "libopenjp2.so" "libopenjpeg.so.2" "libopenjp2.dll.a"
     NAMES "libopenjp2.so.7" "libopenjp2.so" "libopenjp2.a" "libopenjpeg.so.2" "libopenjp2.dll.a"
     PATHS
     $ENV{EXTERNLIBS}/lib
     ~/Library/Frameworks
     /Library/Frameworks
     /usr/local
     /usr
     /sw
     /opt/local
     /opt/csw
     /opt
     PATH_SUFFIXES lib lib64
     REQUIRED
 )
find_path(OPENJPEG_INCLUDE_DIR
     NAMES "openjpeg.h"
     PATHS
     $ENV{EXTERNLIBS}/include
     ~/Library/Frameworks/include
     /Library/Frameworks/include
     /usr/local/include
     /usr/include
     /usr/include/openjpeg-2.3
     /sw/include # Fink
     /opt/local/include # DarwinPorts
     /opt/csw/include # Blastwave
     /opt/include
     PATH_SUFFIXES openjpeg-2.3 openjpeg-2.2 openjpeg-2.1
     REQUIRED
 )
include_directories(${OPENJPEG_INCLUDE_DIR})

find_package(PROJ CONFIG REQUIRED)

target_link_libraries(QtGui2 PRIVATE PROJ::proj)


if(WIN32)
    message(STATUS "Search WIN32 BZIP2")
    find_library(BZIP2_LIBRARIES
        NAME "libbz2.a"
        PATHS
        $ENV{EXTERNLIBS}/lib
         /opt/lib
        REQUIRED
    )
    find_path(BZIP2_INCLUDE_DIR
         NAMES "bzlib.h"
         PATHS
         $ENV{EXTERNLIBS}/include
         /opt/include
         REQUIRED
     )
else()
    find_package(BZip2 REQUIRED)
endif()
include_directories(${BZIP2_INCLUDE_DIR})


    find_library(ZLIB_LIBRARIES
        NAME "libz.a"
        PATHS
        $ENV{EXTERNLIBS}/lib
        /opt/lib
        /usr/local
        /home/pr6khar/Downloads/zlib-1.2.11
        REQUIRED
    )
    find_path(ZLIB_INCLUDE_DIR
         NAMES "zlib.h"
         PATHS
         $ENV{EXTERNLIBS}/include
         /opt/include
         /usr/local
         /home/pr6khar/Downloads/zlib-1.2.11
         REQUIRED
     )

    find_package(ZLIB REQUIRED)

    if(WIN32)
        find_library(PNG_LIBRARIES
            NAME "libpng.a"
            PATHS
            $ENV{EXTERNLIBS}/lib
            /opt/lib
            REQUIRED
        )
        find_path(PNG_INCLUDE_DIR
             NAMES "png.h"
             PATHS
             $ENV{EXTERNLIBS}/include
             /opt/include
             REQUIRED
         )
    else()
        find_library(PNG_LIBRARIES
            NAME "libpng.a"
            PATHS
            $ENV{EXTERNLIBS}/lib
            /opt/lib
            REQUIRED
        )
        find_path(PNG_INCLUDE_DIR
             NAMES "png.h"
             PATHS
             $ENV{EXTERNLIBS}/include
             /opt/include
             REQUIRED
         )
    endif()

target_include_directories (g2clib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(QtGui2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES} ${PNG_LIBRARIES} ${OPENJPEG_LIBRARIES})

ADD_DEFINITIONS(-DUSE_PNG -DUSE_OPENJPEG -DUSE_JPEG2000 )

set_target_properties(QtGui2 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QtGui2)
endif()
