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
    add_subdirectory(src)
    add_subdirectory(3rdparty)
endif()

if (${PARENT}_VSTABILISER_TEST)
    add_subdirectory(test)
endif()

if (${PARENT}_VSTABILISER_EXAMPLE)
    add_subdirectory(example)
endif()