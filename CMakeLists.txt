cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Output file (.exe) nằm ở thư mục build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Tự tải và build SFML từ GitHub
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2       # phiên bản SFML
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(SFML)

# File nguồn
add_executable (main
                src/game.cpp
                src/Core/GameLoop/gameloop.cpp 
                src/GamePlay/UI/UI.cpp 
                src/GamePlay/Physics/PhysicsSystem.cpp 
                src/Utils/json.cpp
                src/GamePlay/UI/StateUI.cpp
                src/GamePlay/UI/Scores.cpp
                src/Core/Input/Input.cpp
                src/Utils/Entity.cpp
                src/GamePlay/Avatar/player.cpp
                src/GamePlay/Gun/gun.cpp
                src/GamePlay/Gun/bullet.cpp
                src/GamePlay/Entity/Dinosaur.cpp
                  src/Core/Audio/Audio.cpp
                src/Core/Audio/MusicManager.cpp
                src/GamePlay/UI/Animation.cpp
                 src/GamePlay/Map/map1.cpp
                src/Core/Input/SettingsManager.cpp
                 "include/GamePlay/Gun/Trap.h" "src/GamePlay/Gun/Trap.cpp" "src/GamePlay/Entity/Meteor.cpp")

# Sử dụng C++17
target_compile_features(main PRIVATE cxx_std_17)

# Link đầy đủ các thư viện SFML cần thiết
target_link_libraries(main PRIVATE
    SFML::Graphics      # Bắt buộc khi dùng hình ảnh
    SFML::Window        # Bắt buộc khi dùng RenderWindow
    SFML::System        # Bắt buộc
    SFML::Audio         # Nếu dùng âm thanh (Sound/Music) → có thể bỏ nếu không dùng
    
)

target_include_directories(main PUBLIC 
include 
src/Utils # <-- ĐÃ SỬA: Trỏ đến src/Utils để tìm json.h và json.hpp
)
# Copy file data.json
file(COPY data/data.json DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY data/Scores.txt DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
# file(COPY data/HighScores.json DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Copy toàn bộ thư mục assets
file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
