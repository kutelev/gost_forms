foreach(LIBRARY_NAME kxcomapplib kxgostformlib kxparsetablelib kxsvglib)
    file(GLOB SOURCES "${LIBRARY_NAME}/*.h" "${LIBRARY_NAME}/*.cxx")
    add_library(${LIBRARY_NAME} STATIC ${SOURCES})
    target_include_directories(${LIBRARY_NAME} INTERFACE ${LIBRARY_NAME})
endforeach()

target_include_directories(kxcomapplib PRIVATE kxsvglib kxparsetablelib)
target_include_directories(kxgostformlib PRIVATE kxsvglib)
target_include_directories(kxsvglib PRIVATE kxparsetablelib)

target_link_libraries(kxcomapplib INTERFACE kxsvglib)
target_link_libraries(kxgostformlib INTERFACE kxsvglib)
target_link_libraries(kxsvglib INTERFACE kxparsetablelib)
