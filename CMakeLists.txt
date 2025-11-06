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
add_executable(main
    src/main.cpp
)

# Sử dụng C++17
target_compile_features(main PRIVATE cxx_std_17)

# Link đầy đủ các thư viện SFML cần thiết
target_link_libraries(main PRIVATE
    SFML::Graphics      # Bắt buộc khi dùng hình ảnh
    SFML::Window        # Bắt buộc khi dùng RenderWindow
    SFML::System        # Bắt buộc
    SFML::Audio         # Nếu dùng âm thanh (Sound/Music) → có thể bỏ nếu không dùng

)
