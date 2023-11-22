cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /O2 /Ob3 /Oi /Oy /Ot /GL /GF /Gy") 
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob3 /Oi /Oy /Ot /GL /GF /Gy") 

project(gdrenderGL)

include(get_cpm.cmake)

add_executable(gdrenderGL
src/main.cpp
src/Application.cpp
src/Layer.cpp
src/Input.cpp
src/RenderTexture.cpp
src/Camera.cpp
src/Batcher.cpp
src/Transformable.cpp
src/GameLayer.cpp
src/Common.cpp
src/Material.cpp
src/Texture.cpp
src/stb_image.cpp
src/HSV.cpp
src/base64.cpp
src/ImageLoader.cpp
src/pugixml.cpp
src/Plist.cpp
src/PlistDate.cpp
src/PlatformUtils.cpp
src/ColorChannel.cpp
src/Tween.cpp
src/Sequence.cpp
src/ActionEasing.cpp
src/ActionInterval.cpp
src/ColorAction.cpp
src/Group.cpp
src/Sprite.cpp
src/GameObject.cpp
src/FollowAction.cpp
src/MoveAction.cpp
src/OpacityAction.cpp
src/PulseAction.cpp
src/RotateAction.cpp
src/SpawnAction.cpp
src/CopyColorAction.cpp
src/EffectGameObject.cpp
src/Level.cpp
src/SelectLevelLayer.cpp
src/AudioEngine.cpp
src/QuadSprite.cpp

src/raudio.c

include/imgui/imgui.cpp
include/imgui/imgui_demo.cpp
include/imgui/imgui_draw.cpp
include/imgui/imgui_impl_glfw.cpp
include/imgui/imgui_tables.cpp
include/imgui/imgui_widgets.cpp
include/imgui/imgui_impl_opengl3.cpp
)

CPMAddPackage("gh:libcpr/cpr#1.10.4")

target_include_directories(gdrenderGL PRIVATE
    include
    include/stb
    include/imgui
    include/raudio
)

target_compile_definitions(gdrenderGL PRIVATE 
    RAUDIO_STANDALONE
    SUPPORT_FILEFORMAT_MP3
    SUPPORT_MODULE_RAUDIO
)

target_link_libraries(gdrenderGL
    ${CMAKE_SOURCE_DIR}/lib/glew32.lib
    ${CMAKE_SOURCE_DIR}/lib/glfw3dll.lib
    cpr::cpr
    opengl32
    zlib
)

install(TARGETS gdrenderGL)