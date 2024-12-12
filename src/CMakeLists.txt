file(GLOB SOURCES CONFIGURE_DEPENDS "cpu/*.cc" 
                                    "syscall/*.cc"
                                    "virtual-memory/*.cc"
                                    "cache/*.cc")
add_subdirectory(sim)