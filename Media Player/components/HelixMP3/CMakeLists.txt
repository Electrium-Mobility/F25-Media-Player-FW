idf_component_register(
    SRC_DIRS
        "."
        "real"
    INCLUDE_DIRS
        "pub"
    PRIV_INCLUDE_DIRS
        "real")

# Some of warinings, block them.
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-but-set-variable)