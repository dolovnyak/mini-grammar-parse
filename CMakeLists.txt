cmake_minimum_required(VERSION 3.15)
project(mini-grammar-parse)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra")
set(CMAKE_C_STANDARD 99)

################################################################################
# mini-grammar-parse library target
################################################################################
set(MGP_LIB libmgp)
add_library(${MGP_LIB}
        src/mgp_grammar_rule.c
        src/mgp_lexeme.c
        src/mgp_error.c
        src/mgp_utilities.c
        src/mgp_parse.c
        src/mgp_init.c
        )
set(MGP_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${MGP_LIB} PUBLIC ${MGP_INCLUDE_DIRECTORIES})

################################################################################
# example target
################################################################################
set(MGP_EXAMPLE ${PROJECT_NAME}-example)
add_executable(${MGP_EXAMPLE}
        mgp_example.c)
target_include_directories(${MGP_EXAMPLE} PUBLIC ${MGP_INCLUDE_DIRECTORIES})
target_link_libraries(${MGP_EXAMPLE} ${MGP_LIB})
set_target_properties(${MGP_EXAMPLE}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
        )
