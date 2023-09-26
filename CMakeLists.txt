cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(gdrender)

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
)

target_include_directories(gdrender PRIVATE
    GD/include
    GD/include/imgui
    GD/include/imgui-SFML
    GD/include/SFML
    GD/include/fmod
)

target_link_libraries(gdrender
    ${CMAKE_CURRENT_SOURCE_DIR}/GD/lib/sfml-system.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/GD/lib/sfml-graphics.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/GD/lib/sfml-window.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/GD/lib/sfml-audio.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/GD/lib/sfml-network.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/GD/lib/zlib.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/GD/lib/fmod_vc.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/GD/lib/cpr.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/GD/lib/libcurl.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/GD/lib/flac.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/GD/lib/freetype.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/GD/lib/ogg.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/GD/lib/openal32.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/GD/lib/vorbis.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/GD/lib/vorbisenc.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/GD/lib/vorbisfile.lib
    opengl32
)