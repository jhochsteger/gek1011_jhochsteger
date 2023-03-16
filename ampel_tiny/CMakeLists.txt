add_executable(ampel_tiny
        ampel_tiny.c
        )

# pull in common dependencies
target_link_libraries(ampel_tiny pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(ampel_tiny)

# add url via pico_set_program_url
example_auto_set_url(ampel_tiny)
