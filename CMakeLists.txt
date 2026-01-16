
cmake_minimum_required (VERSION 3.20)

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

FetchContent_Declare(
	stb
	GIT_REPOSITORY	https://github.com/nothings/stb.git
)

FetchContent_MakeAvailable(stb)



project ("Engine-3D")

set(SOURCE_FILES
    main
    BitMapHandler
    Cube
    Engine
    Observer
    Wall
    Shader
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
    ${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/include"
    ${PROJECT_NAME} "${freeglut_SOURCE_DIR}/include"
    ${PROJECT_NAME} "${stb_SOURCE_DIR}"
)

link_directories("${CMAKE_SOURCE_DIR}/lib")

set(SHADERS

    ${CMAKE_SOURCE_DIR}/shaders/vertex_shader.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragment_shader.glsl
    ${CMAKE_SOURCE_DIR}/shaders/depth_fragment_shader.glsl
    ${CMAKE_SOURCE_DIR}/shaders/depth_vertex_shader.glsl
)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME} PRIVATE
    freeglut
    glm::glm
    "${CMAKE_SOURCE_DIR}/lib/glew32.lib"
)


if(EXISTS "${CMAKE_SOURCE_DIR}/shaders")
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    )
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/textures")
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/textures
    )
endif()



add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:freeglut>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/lib/glew32.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/
)