include(ExternalProject)

# WTL 10.0.10320 – fetched from the GitHub mirror of the official SourceForge release.
# Pinned to a specific commit for reproducibility.
set(WTL_VERSION  "10.0.10320")  # informational; encoded in WTL_ARCHIVE below
set(WTL_GIT_SHA  "a95669345fb0b3c8be5c2607aa844f6adda7b28d")
set(WTL_ARCHIVE  "WTL10_10320_Final")
set(WTL_URL      "https://raw.githubusercontent.com/Win32-WTL/WTL/${WTL_GIT_SHA}/Releases/${WTL_ARCHIVE}.zip")
set(WTL_URL_HASH "SHA256=13de5cd7ed41454b8f287e9bcc4525234c6ac8c85905e4d4fe1a0776709fd78f")

ExternalProject_Add(
    wtl_fetch
    URL                    "${WTL_URL}"
    URL_HASH               "${WTL_URL_HASH}"
    SOURCE_DIR             "${CMAKE_CURRENT_BINARY_DIR}/wtl-src"
    CONFIGURE_COMMAND      ""
    BUILD_COMMAND          ""
    INSTALL_COMMAND        ""
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

ExternalProject_Get_Property(wtl_fetch SOURCE_DIR)

# Create the include directory at configure time so CMake doesn't complain
# about a non-existent path. It will be fully populated at build time when
# wtl_fetch actually downloads and extracts the archive.
file(MAKE_DIRECTORY "${SOURCE_DIR}/Include")

add_library(wtl INTERFACE)
add_dependencies(wtl wtl_fetch)
target_include_directories(wtl INTERFACE "${SOURCE_DIR}/Include")
