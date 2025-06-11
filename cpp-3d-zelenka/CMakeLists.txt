cmake_minimum_required(VERSION 3.11)

# add_compile_options(-pipe -Wall -Wextra -Werror )
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS NO)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_C_COMPILER "gcc" )
set(CMAKE_CXX_COMPILER "g++" )

set(CMAKE_BINARY_DIR "build")

project( 3D-zelenka )

set(INC     "src" "src/GL_ext" "src/GUI" "src/math")

set(SOURCE
            src/main.cpp                            src/scene3d.cpp         
            src/gl_extentions.cpp                   src/object_type.cpp

            src/frame_buffer.cpp                    src/frame_buffer_image.cpp
            
            src/base_camera.cpp                     src/camera.cpp
            src/orbit_camera.cpp
            
            src/light.cpp                           src/shader.cpp
            
            src/base_model.cpp                      src/mesh.cpp
            src/material.cpp                        src/texture.cpp
            
            src/model_assimp.cpp                    src/model_simple.cpp)

set(SOURCE_GUI
            src/GUI/Qt_scene3d.cpp                  src/GUI/main_window.cpp)

            
# Указываем нужные библиотеки
find_package(OpenGL REQUIRED)

find_package(Qt5Widgets)
find_package(Qt5OpenGL)
set(CMAKE_AUTOMOC ON)

set(CMAKE_INCLUDE_PATH      C:/Dev/MSYS2/mingw64/include)
message( ${CMAKE_INCLUDE_PATH} )

set(CMAKE_LIBRARY_PATH      C:/Dev/MSYS2/mingw64/lib )
message( ${CMAKE_LIBRARY_PATH} )

find_library( ASSIMP_LIB NAME "assimp" PATHS ${CMAKE_LIBRARY_PATH} )
message( ${ASSIMP_LIB} )

find_library( TINYXML_LIB NAMES "tinyxml" PATHS ${CMAKE_LIBRARY_PATH} )
message( ${TINYXML_LIB} )

find_library( DEVIL_LIB     NAMES "il"              PATHS ${CMAKE_LIBRARY_PATH} )
message( ${DEVIL_LIB} )

add_executable(${PROJECT_NAME} ${SOURCE} ${SOURCE_GUI} )


# Пути к *.h файлам
target_include_directories(${PROJECT_NAME} PRIVATE ${INC} ${CMAKE_INCLUDE_PATH})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::OpenGL ${OPENGL_LIBRARIES} 
                                    ${ASSIMP_LIB} ${TINYXML_LIB} ${DEVIL_LIB} )
