#############

#to emscripten : emconfigure cmake ../..
#make
#python -m SimpleHTTPServer 8000
#in browser 0.0.0.0:8000
#memleak detection detect with
#valgrind --leak-check=full ./v
##############


#needs to run in ubuntu
#sudo apt install libext-dev
#sudo apt install libx11-dev
#sudo apt install libgl1-mesa-dev
#sudo apt install libglu1-mesa-dev
#sudo apt install libxext-dev


cmake_minimum_required(VERSION 3.1)

#name of project in directory
get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

#SET(CMAKE_BUILD_TYPE Release)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


#set bin dir to compile
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories( ${PROJECT_SOURCE_DIR}/include)


file (GLOB_RECURSE SRC_LIST src/*.cpp src/*.c   src/*.h  src/*.hpp)


#find sdl2
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(SDL2 REQUIRED sdl2)

#if you can't use SDL2 via sudo apt install,you can uncomment these
#lines and place SDL2 src's in libs folder - it will build statically

ADD_SUBDIRECTORY(libs/SDL2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SDL_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/libs/SDL2/include)
include_directories( ${SDL_INCLUDE_DIRS})

 
#flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -ggdb3 ")

ADD_SUBDIRECTORY(libs/camera_pos)
include_directories( libs/camera_pos/include)


add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} PRIVATE m )
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2-static )
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARY})
target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
target_link_libraries(${PROJECT_NAME} PRIVATE camera_pos)





if(UNIX OR WIN32 OR APPLE)
    configure_file("${PROJECT_SOURCE_DIR}/resources/DroidSans.ttf"  ${CMAKE_BINARY_DIR}/bin  COPYONLY)
    configure_file("${PROJECT_SOURCE_DIR}/resources/droid.ttf"  ${CMAKE_BINARY_DIR}/bin  COPYONLY)
    configure_file("${PROJECT_SOURCE_DIR}/resources/background.png"  ${CMAKE_BINARY_DIR}/bin  COPYONLY)
    configure_file("${PROJECT_SOURCE_DIR}/resources/key.json"  ${CMAKE_BINARY_DIR}/bin  COPYONLY)
endif()
#emscripten

if(EMSCRIPTEN)
configure_file("${PROJECT_SOURCE_DIR}/resources/index.html"  ${CMAKE_BINARY_DIR}/bin  COPYONLY)
configure_file("${PROJECT_SOURCE_DIR}/resources/DroidSans.ttf" ${CMAKE_BINARY_DIR} COPYONLY)
configure_file("${PROJECT_SOURCE_DIR}/resources/droid.ttf" ${CMAKE_BINARY_DIR} COPYONLY)
configure_file("${PROJECT_SOURCE_DIR}/resources/background_emc.png" ${CMAKE_BINARY_DIR} COPYONLY)
#see debug info
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS " -s ASSERTIONS=1 -s DEMANGLE_SUPPORT=1  --bind -s WASM=1 -Os -g4 -s ALLOW_MEMORY_GROWTH=1 --preload-file DroidSans.ttf --preload-file droid.ttf --preload-file background_emc.png")
#release
#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS " -O3 --preload-file DroidSans.ttf --preload-file droid.ttf ")
#-s SAFE_HEAP=1


option(JS_ONLY "Compiles to native JS (No WASM)" OFF)
if(JS_ONLY)
        #message(STATUS "Setting compilation target to native JavaScript")
        #set(CMAKE_EXECUTABLE_SUFFIX ".js")
       # set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s EXPORTED_FUNCTIONS='[_main]'")
else(JS_ONLY)
        #message(STATUS "Setting compilation target to WASM")
        #set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")
       # set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s WASM=1 -s BINARYEN_METHOD='native-wasm' -s EXPORTED_FUNCTIONS='[_main]'")
endif(JS_ONLY)

endif(EMSCRIPTEN)
