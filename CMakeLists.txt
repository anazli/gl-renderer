cmake_minimum_required(VERSION 3.25)

project(gl-renderer LANGUAGES C CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)

#----------------------------------
#------------- MATH ---------------
#----------------------------------
FetchContent_Declare(
	math
	GIT_REPOSITORY https://github.com/anazli/math.git
	GIT_TAG main
)
FetchContent_MakeAvailable(math)
set(MATH_BUILD_TESTS OFF CACHE BOOL "" FORCE)

#----------------------------------
#------------- GLFW ---------------
#----------------------------------
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(glfw)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

#----------------------------------
#------------- GLAD ---------------
#----------------------------------
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(glad)

#----------------------------------
#----------- STB IMAGE ------------
#----------------------------------
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(stb)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/stb_impl.cpp"
	"#define STB_IMAGE_IMPLEMENTATION\n#include \"stb_image.h\"")

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_SOURCE_DIR})

add_library(stb_image_impl STATIC "${CMAKE_CURRENT_BINARY_DIR}/stb_impl.cpp")
target_link_libraries(stb_image_impl PUBLIC stb_image)

add_library(${PROJECT_NAME}
	include/application.h
	include/attribute.h
	include/draw.h
	include/index_buffer.h
	include/shader.h
	include/texture.h
	include/uniform.h
	include/window.h

	src/application.cpp
	src/draw.cpp
	src/index_buffer.cpp
	src/shader.cpp
	src/texture.cpp
	src/window.cpp
	src/sample1/sample1.cpp
	src/sample1/sample1.h
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

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

target_link_libraries(${PROJECT_NAME}
	PRIVATE
	 	glfw
	 	glad
		math
		stb_image_impl
)

target_include_directories(${PROJECT_NAME} PRIVATE ${glad_BINARY_DIR}/include)

# Main target for testing sample applications
add_executable(${PROJECT_NAME}-app src/main.cpp)
target_link_libraries(${PROJECT_NAME}-app
	PRIVATE
		${PROJECT_NAME}
		glfw
		glad
)

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  FILE_SET headers DESTINATION include
)