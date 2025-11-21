#include "GamePlay/Map/map.h"
#include "GamePlay/UI/UI.h"

void Map::map1(sf::RenderWindow &window, sf::Font &menuFont, sf::Sprite &backgroundSprite,
               sf::Sprite &backgroundSprite2, sf::Sprite &sunSprite, sf::Sprite &treeSprite, sf::RectangleShape &ground,
               sf::RectangleShape &ground2, std::vector<Obstacle> &obstacles) {
    if (!backgroundTexture.loadFromFile("assets/Images/bg_game/bg1.jpg")) {
        std::cerr << "Lỗi: Không thể tải 'assets/Images/bg_game/bgmap1.jpg'\n";
        return;
    } // ảnh 3500 * 2023 thi chỉnh làm sao thấy được phần muốn thấy bằng cách chỉnh tọa độ

    backgroundSprite.setTexture(backgroundTexture);
    sf::FloatRect bgBounds = backgroundSprite.getLocalBounds();
    // Hiển thị phần giữa ảnh
    int viewLeft = 0;
    int viewTop = 0;
    sf::IntRect viewRect(sf::Vector2i(viewLeft, viewTop),
                         sf::Vector2i(static_cast<int>(WINDOW_WIDTH)*1.0, static_cast<int>(WINDOW_HEIGHT)*1.0));
    backgroundSprite.setTextureRect(viewRect);

    sf::Vector2u windowSize = window.getSize();

    int setScaleInt = 1;
    // 1. Định nghĩa rõ ràng scaleX và scaleY
    float scaleX = (float)windowSize.x * setScaleInt / viewRect.size.x;
    float scaleY = (float)windowSize.y * setScaleInt / viewRect.size.y;
    backgroundSprite.setScale(sf::Vector2f((float)windowSize.x * setScaleInt / viewRect.size.x,
                                           (float)windowSize.y * setScaleInt / viewRect.size.y));

    backgroundSprite.setPosition({0.f, 0.f});

    backgroundSprite2.setTexture(backgroundTexture);
    backgroundSprite2.setTextureRect(viewRect);
    backgroundSprite2.setScale(sf::Vector2f(scaleX, scaleY));
    // Đặt vị trí của sprite 2 ngay bên phải sprite 1
    backgroundSprite2.setPosition({(float)windowSize.x, 0.f});

    // chỗ thêm vật cản =============================================================================

//obstacles.emplace_back("assets/Images/dat3.png", 60.0f, 60.0f, -120.0f, 650.0f - 60.0f);

 // --- 1. CÁC KHỐI ĐẤT LỚN (Da2 - Ground Islands) ---
    // Y = 585 (685 - 100) là mặt đất cơ bản
    // Size Da2: 200x150

    // Đảo khởi đầu (Chỗ đứng an toàn lúc spawn)
    obstacles.emplace_back("assets/Images/PNG/Da2.png", 200.0f, 150.0f, 100.0f, 685.0f - 100.0f);

    // Đảo giữa 1 (Cách đảo đầu 300px - phải nhảy qua bục đệm)
    obstacles.emplace_back("assets/Images/PNG/Da2.png", 200.0f, 150.0f, 600.0f, 685.0f - 100.0f);

    // Đảo giữa 2
    obstacles.emplace_back("assets/Images/PNG/Da2.png", 200.0f, 150.0f, 1100.0f, 685.0f - 100.0f);

    // Đảo cuối
    obstacles.emplace_back("assets/Images/PNG/Da2.png", 200.0f, 150.0f, 1600.0f, 685.0f - 100.0f);

    // --- 2. CÁC BỤC BAY (Da1 - Floating Platforms) ---
    // Size Da1: 170x60
    // Tầng 1 (Thấp): Y ≈ 455 (Dễ nhảy từ đất lên)
    // Tầng 2 (Cao): Y ≈ 325 (Phải nhảy từ Tầng 1 lên)

    // -- Cụm 1: Bắc cầu từ Đảo khởi đầu sang Đảo giữa 1 --
    // Bục này giúp nhân vật nhảy qua khoảng trống giữa X=300 và X=600
    obstacles.emplace_back("assets/Images/PNG/Da1.png", 170.0f, 60.0f, 350.0f, 685.0f - 230.0f); // Cao hơn đất 130px

    // -- Cụm 2: Thử thách leo cao tại Đảo giữa 1 --
    // Bục thấp (Tầng 1)
    obstacles.emplace_back("assets/Images/PNG/Da1.png", 170.0f, 60.0f, 750.0f, 685.0f - 230.0f);
    // Bục cao (Tầng 2) - Nhảy từ bục 750 lên bục này
    obstacles.emplace_back("assets/Images/PNG/Da1.png", 170.0f, 60.0f, 1050.0f, 705.0f - 360.0f); // Cao hơn tầng 1 130px

    // -- Cụm 3: Đường xuống Đảo giữa 2 --
    // Bục trung gian để nhảy xuống đất an toàn
    obstacles.emplace_back("assets/Images/PNG/Da1.png", 170.0f, 60.0f, 1350.0f, 685.0f - 230.0f);

    // -- Cụm 4: Bậc thang về đích --
    obstacles.emplace_back("assets/Images/PNG/Da1.png", 170.0f, 60.0f, 1900.0f, 695.0f - 200.0f);
    obstacles.emplace_back("assets/Images/PNG/Da1.png", 170.0f, 60.0f, 2150.0f, 685.0f - 330.0f);

    //===============================================================================================

    ground.setFillColor(sf::Color(0, 0, 0, 0));
    ground2.setFillColor(sf::Color(0, 0, 0, 0));
}
