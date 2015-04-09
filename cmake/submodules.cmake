if(EXISTS "${PROJECT_SOURCE_DIR}/.gitmodules")

  find_package(Git)

  if(NOT ${GIT_FOUND})
    message(WARNING
      "Git executable could not be found, please manually specify it in order to have submodules automatically updated")

    option(GIT_EXECUTABLE "Git executable path" ${GIT_EXECUTABLE})
    mark_as_advanced(CLEAR GIT_EXECUTABLE)
  else()

    message(STATUS "Updating submodules to their latest/fixed versions")
    message(STATUS "(this can take a while, please be patient)")

    ### First, get all submodules in
    if(${GIT_SUBMODULES_CHECKOUT_QUIET})
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY   ${PROJECT_SOURCE_DIR}
            OUTPUT_QUIET
            ERROR_QUIET
        )

        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule foreach git checkout master
            WORKING_DIRECTORY   ${PROJECT_SOURCE_DIR}
            OUTPUT_QUIET
            ERROR_QUIET
        )

        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule foreach git pull origin master
            WORKING_DIRECTORY   ${PROJECT_SOURCE_DIR}
            OUTPUT_QUIET
            ERROR_QUIET
        )
    else()
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY   ${PROJECT_SOURCE_DIR}
        )

        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule foreach git checkout master
            WORKING_DIRECTORY   ${PROJECT_SOURCE_DIR}
        )

        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule foreach git pull origin master
            WORKING_DIRECTORY   ${PROJECT_SOURCE_DIR}
        )
    endif()

  endif()

endif()
