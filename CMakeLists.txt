cmake_minimum_required(VERSION 3.13)



################################################################################
## PROJECT
## name and version
################################################################################
project(VStabiliser LANGUAGES CXX)



################################################################################
## SETTINGS
## basic project settings before use
################################################################################
# If this project is used as a submodule, the variable should be overridden
# to "OFF" in the top-level application (to disable forced cache rewriting)
option(${PARENT}_SUBMODULE_CACHE_OVERWRITE "Enable forced cache rewriting" ON)
if (${PARENT}_SUBMODULE_CACHE_OVERWRITE)
    SET(REWRITE_FORCE "FORCE")
else()
    SET(REWRITE_FORCE "")
endif()

# Set default Release mode if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type defaulting to Release" FORCE)
endif()


################################################################################
## CONFIGURATION
## project configuration
################################################################################
SET(${PARENT}_VSTABILISER                    ON  CACHE BOOL "" ${REWRITE_FORCE})
if(NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    SET(${PARENT}_VSTABILISER_TEST           OFF CACHE BOOL "" ${REWRITE_FORCE})
    SET(${PARENT}_VSTABILISER_EXAMPLE        OFF CACHE BOOL "" ${REWRITE_FORCE})
    message("${PROJECT_NAME} included as subrepository.")
else()
    SET(${PARENT}_VSTABILISER_TEST           ON  CACHE BOOL "" ${REWRITE_FORCE})
    SET(${PARENT}_VSTABILISER_EXAMPLE        ON  CACHE BOOL "" ${REWRITE_FORCE})
    message("${PROJECT_NAME} is a standalone project.")
endif()


################################################################################
## INCLUDING SUBDIRECTORIES
## Adding subdirectories according to the configuration
################################################################################
if (${PARENT}_VSTABILISER)
    # If this project is a stand alone repository, its dependencies are expected
    # to be next to it (../Frame, ../ConfigReader) and are added first. If this
    # project is included in another one, the top-level CMakeLists.txt must add
    # these libraries by itself.
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Frame ${CMAKE_CURRENT_BINARY_DIR}/Frame)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../ConfigReader ${CMAKE_CURRENT_BINARY_DIR}/ConfigReader)
    endif()
    add_subdirectory(src)
endif()

if (${PARENT}_VSTABILISER_TEST)
    add_subdirectory(test)
endif()

if (${PARENT}_VSTABILISER_EXAMPLE)
    add_subdirectory(example)
endif()