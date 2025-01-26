cmake_minimum_required(VERSION 3.23)
project(DungeonZ LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

set(PROJECT_HEADERS
    include/Enums/FaceDirection.h

    include/Interfaces/IDamageable.h

    include/Utils/Constants.h
    include/Utils/Vector.h

    include/Core/AssetManager.h
    include/Core/SpawnPool.h
    include/Core/Game.h
    include/Core/World.h

    include/Map/Background.h
    include/Map/Room.h
    include/Map/Tile.h

    include/Gameplay/Enemy.h
    include/Gameplay/GameObject.h
    include/Gameplay/Villager.h
    include/Gameplay/Player.h
)

set(PROJECT_SOURCES
    src/Utils/Vector.cpp

    src/Core/AssetManager.cpp
    src/Core/SpawnPool.cpp
    src/Core/Game.cpp
    src/Core/World.cpp

    src/Map/Background.cpp
    src/Map/Room.cpp
    src/Map/Tile.cpp

    src/Gameplay/Enemy.cpp
    src/Gameplay/Villager.cpp
    src/Gameplay/Player.cpp

    src/main.cpp
)

set(PROJECT_FILES
  ${PROJECT_HEADERS}
  ${PROJECT_SOURCES}
)

include_directories(${PROJECT_NAME} PRIVATE include)

add_executable(${PROJECT_NAME} ${PROJECT_FILES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM)
endif()
