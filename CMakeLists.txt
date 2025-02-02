cmake_minimum_required(VERSION 3.23)
project(DungeonZ LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

# SFML
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# tmxlite ('Tiled' editor loader)
FetchContent_Declare(
    TMXLITE
    GIT_REPOSITORY https://github.com/fallahn/tmxlite.git
    GIT_TAG v1.4.4
    SOURCE_SUBDIR tmxlite)
FetchContent_MakeAvailable(TMXLITE)


set(PROJECT_HEADERS
    include/Render/SFMLOrthogonalLayer.h
    include/Assets/SpriteSheet.h

    include/UI/MainMenu.h

    include/Enums/Difficulty.h
    include/Enums/DirectionEnum.h
    include/Enums/EnemyType.h
    include/Enums/RoomType.h

    include/Interfaces/IDamageable.h

    include/Utils/Constants.h
    include/Utils/Pathfinding.h
    include/Utils/Random.h
    include/Utils/Vector.h

    include/Core/AssetManager.h
    include/Core/CollisionManager.h
    include/Core/DungeonManager.h
    include/Core/Game.h
    include/Core/SpawnPool.h
    include/Core/WindowManager.h
    include/Core/World.h

    include/Map/EnemyRoom.h
    include/Map/Room.h

    include/Gameplay/Dungeon.h
    include/Gameplay/DynamiteGoblin.h
    include/Gameplay/Enemy.h
    include/Gameplay/GameObject.h
    include/Gameplay/Player.h
    include/Gameplay/TorchGoblin.h
    include/Gameplay/Villager.h
)

set(PROJECT_SOURCES
    src/UI/MainMenu.cpp

    src/Utils/Pathfinding.cpp
    src/Utils/Random.cpp
    src/Utils/Vector.cpp

    src/Core/AssetManager.cpp
    src/Core/CollisionManager.cpp
    src/Core/DungeonManager.cpp
    src/Core/Game.cpp
    src/Core/SpawnPool.cpp
    src/Core/WindowManager.cpp
    src/Core/World.cpp

    src/Map/EnemyRoom.cpp
    src/Map/Room.cpp

    src/Gameplay/Dungeon.cpp
    src/Gameplay/DynamiteGoblin.cpp
    src/Gameplay/Enemy.cpp
    src/Gameplay/Player.cpp
    src/Gameplay/TorchGoblin.cpp
    src/Gameplay/Villager.cpp

    src/main.cpp
)

set(PROJECT_FILES
  ${PROJECT_HEADERS}
  ${PROJECT_SOURCES}
)

include_directories(${PROJECT_NAME} PRIVATE ${tmxlite_INCLUDE_DIR} include)

add_executable(${PROJECT_NAME} ${PROJECT_FILES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics tmxlite)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG_MODE=0)

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM)
endif()
