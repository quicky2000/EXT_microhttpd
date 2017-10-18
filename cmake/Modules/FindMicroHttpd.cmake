#
#  This file is part of EXT_microhttpd
#  Copyright (C) 2017  Julien Thevenon ( julien_thevenon at yahoo.fr )
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>
#
#.rst:
# FindMicroHttpd
# -------------
#
# Module to locate MicroHttpd library
#
# This module define the following variables
#    MicroHttpd_FOUND
#    MicroHttpd_VERSION
#    MicroHttpd_LIBRARIES
#    MicroHttpd_INCLUDE_DIRS
#
find_package(PkgConfig)
pkg_check_modules(PC_MICROHTTPD QUIET libmicrohttpd)
if("${PC_MICROHTTPD}" STREQUAL "")
    set(MICROHTTPD_HINTS $ENV{MicroHttpd_INSTALL})

    set(MICROHTTPD_INCLUDE_PATHS /usr
        /usr/local/MicroHttpd-${MicroHttpd_FIND_VERSION}
        /usr/lib/MicroHttpd-${MicroHttpd_FIND_VERSION}
        /usr/local/lib/MicroHttpd-${MicroHttpd_FIND_VERSION}
        /usr/local/share/MicroHttpd-${MicroHttpd_FIND_VERSION}
        /usr/share/MicroHttpd-${MicroHttpd_FIND_VERSION})

    set(MICROHTTPD_LIBRARY_PATHS /usr/lib
        /usr/local/MicroHttpd-${MicroHttpd_FIND_VERSION}
        /usr/lib/MicroHttpd-${MicroHttpd_FIND_VERSION}
        /usr/local/lib/MicroHttpd-${MicroHttpd_FIND_VERSION}
        /usr/local/share/MicroHttpd-${MicroHttpd_FIND_VERSION}
        /usr/share/MicroHttpd-${MicroHttpd_FIND_VERSION})
    find_path(MicroHttpd_INCLUDE_DIR
              NAMES microhttpd.h
              PATHS ${MICROHTTPD_INCLUDE_PATHS}
              HINTS {MICROHTTPD_HINTS}
              PATH_SUFFIXES include
              DOC "Variable storing the location of MicroHttpd header")

    find_library(MicroHttpd_LIBRARY
              NAMES microhttpd
              PATHS ${MicroHttpd_PATHS}
              HINTS {MicroHttpd_HINTS}
              PATH_SUFFIXES x86-64-linux-gnu
              DOC "Variable storing the location of MicroHttpd library")
    set(MicroHttpd_VERSION ${MicroHttpd_FIND_VERSION})
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(MicroHttpd
                                      FOUND_VAR MicroHttpd_FOUND
                                      REQUIRED_VARS
                                      MicroHttpd_LIBRARY
                                      MicroHttpd_INCLUDE_DIR
                                      VERSION_VAR MicroHttpd_VERSION
                                      )
    if(MicroHttpd_FOUND)
        set(MicroHttpd_LIBRARIES ${MicroHttpd_LIBRARY})
        set(MicroHttpd_INCLUDE_DIRS ${MicroHttpd_INCLUDE_DIR})
        #set(MicroHttpd_DEFINITIONS ${PC_MicroHttpd_CFLAGS_OTHER})
    endif()
    if(MicroHttpd_FOUND AND NOT TARGET MicroHttpd::MicroHttpd)
        add_library(MicroHttpd::MicroHttpd UNKNOWN IMPORTED)
        set_target_properties(MicroHttpd::MicroHttpd PROPERTIES
        IMPORTED_LOCATION "${MicroHttpd_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS ""
        INTERFACE_INCLUDE_DIRECTORIES "${MicroHttpd_INCLUDE_DIR}")
    endif()
endif("${PC_MICROHTTPD}" STREQUAL "")
