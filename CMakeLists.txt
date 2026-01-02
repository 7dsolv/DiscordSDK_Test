cmake_minimum_required(VERSION 3.10)
project(DiscordSDK_Test)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
add_executable(DiscordSDK_Test main.cpp)

# Add include directories
target_include_directories(DiscordSDK_Test PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/External/discord-files/include
)

# Add library directories
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/External/discord-files/lib)

# Link Discord SDK library
target_link_libraries(DiscordSDK_Test PRIVATE 
    discord_game_sdk.dll.lib
)

# For Windows
if(WIN32)
    # Copy DLL to output directory
    add_custom_command(TARGET DiscordSDK_Test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/discord_game_sdk.dll"
        "$<TARGET_FILE_DIR:DiscordSDK_Test>/discord_game_sdk.dll"
    )
endif()
