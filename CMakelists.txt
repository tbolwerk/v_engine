if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif()
project(v_engine)
cmake_minimum_required(VERSION 3.14)


if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /W4 /O2")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -std=c++17  " )
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
	else()
		#Nothing for gcc
	endif()
endif()

FILE(COPY models DESTINATION "${CMAKE_BINARY_DIR}")
FILE(COPY textures DESTINATION "${CMAKE_BINARY_DIR}")

AUX_SOURCE_DIRECTORY(. SOURCE_FILES) 





# GLFW
set(GLFW_DIR libs/glfw-3.3.4) # Set this to point to an up-to-date GLFW repo
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)
include_directories(${GLFW_DIR}/include)

# Dear ImGui
set(IMGUI_DIR libs/imgui)
include_directories(${IMGUI_DIR})

file(GLOB IM_GUI_SOURCES libs/imgui/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${IM_GUI_SOURCES})

if (CMAKE_GENERATOR MATCHES "Visual Studio")
    # Do Visual Studio specific stuff
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	add_dependencies("${PROJECT_NAME}" BuildShaderHeader)
else()
    # Other stuff
endif()

find_package(Vulkan REQUIRED)

if (VULKAN_FOUND)
    message(STATUS "Found Vulkan, Including and Linking now")
    include_directories(${Vulkan_INCLUDE_DIRS})
    target_link_libraries (${PROJECT_NAME} ${Vulkan_LIBRARIES} glfw)

endif (VULKAN_FOUND)

FILE(GLOB glslc-folders ${VULKAN_SDK}/bin)
FIND_PROGRAM(GLSLC glslc HINTS ${glslc-folders})

add_subdirectory( libs/glm )
target_link_libraries(${PROJECT_NAME} glm)



IF(GLSLC)
	SET(SHADER_DIR "shaders")
	SET(SHADER_FILES ${SHADER_DIR}/*.frag ${SHADER_DIR}/*.vert ${SHADER_DIR}/*.geom  ${SHADER_DIR}/*.comp)
	FILE(GLOB_RECURSE SHADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SHADER_FILES})
	FOREACH(SHADER ${SHADERS})
		SET(shader-input ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER})
		SET(shader-output ${CMAKE_CURRENT_BINARY_DIR}/${SHADER}.spv)
		ADD_CUSTOM_COMMAND (
		  OUTPUT ${shader-output}
		  COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_DIR}"
		  COMMAND ${GLSLC} ${shader-input} -o ${shader-output}
		  COMMENT "Compiling ${shader-input}"
		  DEPENDS ${SHADER}
		  VERBATIM
		)
		SET(SHADER_OUTPUTS ${SHADER_OUTPUTS} ${shader-output})
	ENDFOREACH()

	ADD_CUSTOM_TARGET(BuildShaderHeader ALL DEPENDS ${SHADER_OUTPUTS})

	

	FOREACH(shad_spv ${SHADER_OUTPUTS})
		GET_FILENAME_COMPONENT(SPV ${shad_spv} NAME)
		ADD_CUSTOM_COMMAND (TARGET BuildShaderHeader
			POST_BUILD
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${SHADER_DIR}
		)
	ENDFOREACH()
	#add_definitions( -DDEBUG_VK_PERF=true )
ENDIF()
