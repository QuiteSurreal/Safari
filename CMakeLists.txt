# IMPORTANT: Set it to your vcpkg root folder! Use Forwardslash!!!
set(VCPKG_PATH "F:/ComputerStuff/OpenGL/vcpkg")

cmake_minimum_required(VERSION 3.16)
project(MySafari)

option(HEADLESS "Enable headless mode (no graphics)" OFF)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_TOOLCHAIN_FILE "${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

if(WIN32)
    set(VCPKG_TRIPLET "x64-windows")
else()
    set(VCPKG_TRIPLET "x64-linux")
endif()

set(VCPKG_TARGET_TRIPLET "${VCPKG_TRIPLET}")

file(GLOB DLL_FILES "${VCPKG_PATH}/installed/${VCPKG_TRIPLET}/debug/bin/*.dll")

### Sometimes CMake won't search in the directory where it is installed, you can make sure here it searches there
set(CMAKE_PREFIX_PATH "${VCPKG_PATH}/installed/${VCPKG_TRIPLET}/share" ${CMAKE_PREFIX_PATH})

find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

if(HEADLESS)
find_package(GTest REQUIRED)
endif()

file(GLOB_RECURSE SRC_FILES "src/*.cpp" "src/*.c" "src/*.h" "src/*.hpp")
file(GLOB_RECURSE TEST_FILES "tests/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES})

if(HEADLESS)
add_library(${PROJECT_NAME}_lib STATIC ${SRC_FILES})

add_executable(${PROJECT_NAME}Tests ${TEST_FILES})

target_link_libraries(MySafariTests PRIVATE GTest::GTest GTest::Main ${PROJECT_NAME}_lib)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${GLEW_INCLUDE_DIRS} 
    ${SDL2_INCLUDE_DIRS} 
    ${SDL2_TTF_INCLUDE_DIRS} 
    ${CMAKE_SOURCE_DIR}/src/framework 
    ${CMAKE_SOURCE_DIR}/src/game
	#${PROJECT_NAME}_lib
)

if(HEADLESS)
target_include_directories(${PROJECT_NAME}_lib PRIVATE 
    ${GLEW_INCLUDE_DIRS} 
    ${SDL2_INCLUDE_DIRS} 
    ${SDL2_TTF_INCLUDE_DIRS} 
    ${CMAKE_SOURCE_DIR}/src/framework 
)

target_include_directories(MySafariTests PRIVATE
    ${GLEW_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src/framework
    ${CMAKE_SOURCE_DIR}/src/game
	${CMAKE_SOURCE_DIR}/tests
	${PROJECT_NAME}_lib
)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE 
    GLEW::GLEW 
    SDL2::SDL2 
    SDL2_ttf::SDL2_ttf 
    #${PROJECT_NAME}_lib
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
endif()


if(HEADLESS)
    target_compile_definitions(${PROJECT_NAME} PRIVATE HEADLESS)
    target_compile_definitions(${PROJECT_NAME}_lib PRIVATE HEADLESS)
    target_compile_definitions(MySafariTests PRIVATE HEADLESS)
endif()


set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ".."
    VS_GLOBAL_VcpkgConfiguration "Debug"
)

if(HEADLESS)
set_target_properties(${PROJECT_NAME}_lib PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ".."
    VS_GLOBAL_VcpkgConfiguration "Debug"
)

set_target_properties(MySafariTests PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ".."
    VS_GLOBAL_VcpkgConfiguration "Debug"
)

endif()

foreach(DLL_FILE ${DLL_FILES})
   add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DLL_FILE}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endforeach()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
