# Create an INTERFACE library for our C module.
add_library(usermod_builtindac INTERFACE)

# Add our source files to the lib
target_sources(usermod_builtindac INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/builtindac.c
    ${CMAKE_CURRENT_LIST_DIR}/../lib/i2sdacbuiltintone.c
)

# Add the current directory as an include directory.
target_include_directories(usermod_builtindac INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_builtindac)
