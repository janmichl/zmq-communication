file(GLOB SOURCES
     "*.cpp")

file(GLOB INCLUDES
     "*.h")

foreach(SOURCE ${SOURCES})
    get_filename_component(TGT_NAME ${SOURCE} NAME_WE)
    add_executable(${TGT_NAME} ${SOURCE} ${INCLUDES})
    target_link_libraries(${TGT_NAME} ${LIBS_TO_LINK_WITH})
    add_dependencies(${TGT_NAME} copy_headers)
    add_dependencies(${TGT_NAME} build_libraries)
endforeach()

