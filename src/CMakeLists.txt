file(GLOB SOURCES
     "*.cpp")

file(GLOB INCLUDES
     "*.h")

foreach(SOURCE ${SOURCES})
    get_filename_component(tgt ${SOURCE} NAME_WE)
    add_executable(${tgt} ${SOURCE} ${INCLUDES})
    target_link_libraries(${tgt} ${cppzmq_LIBRARY})
endforeach()
