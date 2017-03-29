PROJECT(LoS)
CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

ADD_EXECUTABLE(LegendOfSajjad 	src/main.cpp
								src/window.cpp
								src/world.cpp
								src/ui.cpp
								src/player.cpp
								src/penguin.cpp
								src/sfx/soundBank.cpp
								src/gfx/sprite.cpp
								src/boss/boss.cpp
								src/boss/patrick.cpp
								src/boss/aaron.cpp
								src/boss/valentin.cpp
								src/boss/jannik.cpp)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
SET_PROPERTY(TARGET LegendOfSajjad PROPERTY CXX_STANDARD_REQUIRED ON)
SET_PROPERTY(TARGET LegendOfSajjad PROPERTY CXX_STANDARD 11)

FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(SDL2_image REQUIRED)
FIND_PACKAGE(SDL2_mixer REQUIRED)

TARGET_LINK_LIBRARIES(LegendOfSajjad 	${SDL2_LIBRARY}
										${SDL2IMAGE_LIBRARY}
										${SDL2_MIXER_LIBRARIES})
TARGET_INCLUDE_DIRECTORIES(LegendOfSajjad PUBLIC 	${SDL2_INCLUDE_DIR}
													${SDL2IMAGE_INCLUDE_DIR}
													${SDL2_MIXER_INCLUDE_DIRS})
