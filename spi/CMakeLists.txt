add_executable(spi
        spi.c
        )

# pull in common dependencies
target_link_libraries(spi pico_stdlib)
target_link_libraries(spi hardware_spi)
target_link_libraries(spi pico_binary_info)

# create map/bin/hex file etc.
pico_add_extra_outputs(spi)

# add url via pico_set_program_url
example_auto_set_url(spi)
