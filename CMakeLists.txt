# CMakeList.txt : CMake project for test123, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.11)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

FetchContent_Declare(
	freeglut
	GIT_REPOSITORY	https://github.com/freeglut/freeglut.git
	GIT_TAG 	566cd459201ae29003ef5686c06506d8646e55c6 #refs/tags/v3.6.0
)

FetchContent_MakeAvailable(freeglut)


project ("Engine-3D")

set(SOURCE_FILES
    main
    Engine
    PrimitiveRenderer
    Cube
    Observer
)


set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")


set(FULL_SOURCE_FILES)
foreach(FILE ${SOURCE_FILES})
    list(APPEND FULL_SOURCE_FILES "${SRC_DIR}/${FILE}.cpp")
endforeach()


add_executable(${PROJECT_NAME} ${FULL_SOURCE_FILES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Engine-3D PROPERTY CXX_STANDARD 20)
endif()

INCLUDE_DIRECTORIES(
    ${PROJECT_NAME} "${freeglut_SOURCE_DIR}/include"
)


TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME} PRIVATE
    freeglut
)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME} PRIVATE
    glm::glm
)

add_custom_target(copy-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy
    ${freeglut_BINARY_DIR}/bin/freeglutd.dll
    ${CMAKE_CURRENT_BINARY_DIR}
)

