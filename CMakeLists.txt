cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SFML_BUILD_AUDIO OFF)

project(gdrender)

include(get_cpm.cmake)

add_executable(gdrender
    GD/ActionEasing.cpp
    GD/ActionInterval.cpp
    GD/Application.cpp
    GD/AudioEngine.cpp
    GD/base64.cpp
    GD/Batcher.cpp
    GD/ColorAction.cpp
    GD/ColorChannel.cpp
    GD/Common.cpp
    GD/CopyColorAction.cpp
    GD/EffectGameObject.cpp
    GD/GameLayer.cpp
    GD/GameObject.cpp
    GD/Group.cpp
    GD/HSV.cpp
    GD/ImageLoader.cpp
    GD/include/imgui-sfml/imgui-SFML.cpp
    GD/include/imgui/imgui.cpp
    GD/include/imgui/imgui_demo.cpp
    GD/include/imgui/imgui_draw.cpp
    GD/include/imgui/imgui_tables.cpp
    GD/include/imgui/imgui_widgets.cpp
    GD/Layer.cpp
    GD/Level.cpp
    GD/main.cpp
    GD/MoveAction.cpp
    GD/OpacityAction.cpp
    GD/Plist.cpp
    GD/PlistDate.cpp
    GD/pugixml.cpp
    GD/PulseAction.cpp
    GD/RotateAction.cpp
    GD/SelectLevelLayer.cpp
    GD/Sequence.cpp
    GD/SpawnAction.cpp
    GD/Sprite.cpp
    GD/Tween.cpp
    GD/raudio.c

    .clang-format
)

CPMAddPackage("gh:SFML/SFML#2.6.x")
CPMAddPackage("gh:libcpr/cpr#1.10.4")

target_include_directories(gdrender PRIVATE
    GD/include
    GD/include/imgui
    GD/include/imgui-SFML
    GD/include/raudio
)

target_compile_definitions(gdrender PRIVATE 
    RAUDIO_STANDALONE
    SUPPORT_FILEFORMAT_MP3
    SUPPORT_MODULE_RAUDIO
)

target_link_libraries(gdrender
    sfml-graphics
    cpr::cpr
    opengl32
    zlib
)


install(TARGETS gdrender)