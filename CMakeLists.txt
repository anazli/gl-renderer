cmake_minimum_required(VERSION 3.25)

project(animation LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)

add_library(${PROJECT_NAME}
	include/application.h
	include/attribute.h
	include/draw.h
	include/index_buffer.h
	include/shader.h
	include/texture.h
	include/uniform.h
	include/glad/glad.h
	include/KHR/khrplatform.h
	include/stb/stb_image.h
	include/math/constants.h
	include/math/vec2.h
	include/math/vec3.h
	include/math/vec4.h
	include/math/mat4.h
	include/math/quat.h

	src/application.cpp
	src/draw.cpp
	src/glad.c
	src/index_buffer.cpp
	src/shader.cpp
	src/texture.cpp
	src/stb_image.cpp
	src/windows.h
	src/windows.cpp
	src/sample1/sample1.cpp
	src/sample1/sample1.h
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

set_source_files_properties(src/glad.c PROPERTIES LANGUAGE C)
target_compile_definitions(${PROJECT_NAME} PRIVATE
	$<$<COMPILE_LANGUAGE:C>:GLAD_GLAPI_EXPORT_BUILD>
	ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets"
	SHADERS_DIR="${CMAKE_SOURCE_DIR}/shaders"
)

target_compile_options(${PROJECT_NAME} PRIVATE
	$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<PLATFORM_ID:Windows>>:/wd28251>
	$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<PLATFORM_ID:Windows>>:/wd28159>
)

if(MSVC)
	target_link_options(${PROJECT_NAME} PRIVATE
		$<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
		$<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
	)
else()
	target_link_options(${PROJECT_NAME} PRIVATE
		$<$<CONFIG:Debug>:-Wl,-subsystem,console>
		$<$<CONFIG:Release>:-Wl,-subsystem,windows>
	)
endif()

if(TARGET OpenGL::GL)
	target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL)
else()
	if(OPENGL_gl_LIBRARY)
		target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_gl_LIBRARY})
	endif()
endif()

# Main target for testing sample applications

add_executable(animation-app src/main.cpp)
target_link_libraries(animation-app PRIVATE ${PROJECT_NAME})