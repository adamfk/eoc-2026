file(GLOB SIM_SOURCES CONFIGURE_DEPENDS *.c)

add_executable(simulation_app ${SIM_SOURCES})

# Create a "run" command for this specific target
add_custom_target(run_sim
    COMMAND simulation_app
    DEPENDS simulation_app
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running the simulation..."
    USES_TERMINAL
)

target_link_libraries(simulation_app PRIVATE common_logic)
target_compile_options(simulation_app PRIVATE -Werror -Wall -Wextra)
