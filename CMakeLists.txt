cmake_minimum_required(VERSION 3.4.1)

# Compiler options
add_compile_options(-Wall)

# Submodule library
add_library(loax
            STATIC

            # Source
            loax_server.c
            loax_server_cmd.c
            loax_server_gl2.c
            loax_log.c
            loax_serialize.c)

# Linking
target_link_libraries(loax

                      # Submodule libraries
                      net

                      # NDK libraries
                      log)
