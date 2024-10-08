file(GLOB_RECURSE SRC_FILES "*.cpp")
file(GLOB_RECURSE NESTED_SRC_FILES "**/*.cpp")
add_library(Library SHARED ${SRC_FILES} ${NESTED_SRC_FILES})

# Link SFML libraries to this library
target_link_libraries(Library sfml-graphics sfml-window sfml-system)

# Include SFML directories for the library
target_include_directories(Library PRIVATE ${SFML_INCLUDE_DIR})

# Define a function to check if a file contains a specific keyword
function(file_contains_keyword FILE_PATH KEYWORD RESULT)
    file(READ ${FILE_PATH} FILE_CONTENT)
    string(FIND "${FILE_CONTENT}" "${KEYWORD}" KEYWORD_POSITION)
    if (KEYWORD_POSITION GREATER -1)
        set(${RESULT} TRUE PARENT_SCOPE)
    else()
        set(${RESULT} FALSE PARENT_SCOPE)
    endif()
endfunction()

# Add doctest tests
FOREACH(TEST_FILE ${SRC_FILES})
    file_contains_keyword(${TEST_FILE} "#include \"doctest.h\"" HAS_DOCTEST_INCLUDE)
    file_contains_keyword(${TEST_FILE} "TEST_CASE" HAS_DOCTEST_CASE)
 
    if (HAS_DOCTEST_CASE AND HAS_DOCTEST_INCLUDE)
        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
        add_executable(${TEST_NAME}_test ${TEST_FILE})
        target_link_libraries(${TEST_NAME}_test doctest Library)
        set_target_properties(${TEST_NAME}_test PROPERTIES COMPILE_FLAGS "-g")
        add_test(NAME ${TEST_NAME}_test COMMAND ${TEST_NAME}_test)
    endif()
ENDFOREACH()
