#include "GamePlay/UI/StateUI.h"
#include "Core/Audio/Audio.h"
#include "Core/Audio/MusicManager.h"
#include "Core/GameLoop/game.h"
#include "GamePlay/UI/Scores.h"
#include "GamePlay/UI/UI.h"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <cstdint> // đảm bảo có
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "Core/Audio/Audio.h"
#include "Core/Audio/MusicManager.h"
#include "Core/GameLoop/game.h"
#include <chrono> 
#include <ctime>
#include "Core/GameLoop/json.h"
/* ============================================================
 * CLASS: MainMenuUI — Giao diện chính của game
 * ============================================================ */
MainMenuUI::MainMenuUI(const sf::Sprite &bg, const sf::Sprite &sun, const sf::Sprite &tree, const sf::Font &font)
    : backgroundSprite(bg), sunSprite(sun), treeSprite(tree) {
    isLinkVisible = false;
    // --- [1] Nút “New Game” ---
    // Dùng std::make_unique để tạo sprite, truyền texture và vị trí nút
    btnNewSprite = std::make_unique<sf::Sprite>(
        createSprite(btnNewTexture, "assets/Images/play.png", 250.0f, 150.0f, 810.f, 490.0f));
    // --- Nút “Continue”(Xuất hiện khi nhân vật chưa chết) ---
    btnContinueSprite = std::make_unique<sf::Sprite>(
        createSprite(btnContinueTexture, "assets/Images/continue.png", 250.0f, 150.0f, 810.0f,650.0f));
    // --- [2] Nút “High Scores” ---
    btnHighScoresSprite = std::make_unique<sf::Sprite>(
        createSprite(btnHighScoresTexture, "assets/Images/prize.png", 250.0f, 150.0f, 1630.0f, 650.0f));

    // --- [3] Nút “Settings” ---
    btnSettingsSprite = std::make_unique<sf::Sprite>(
        createSprite(btnSettingsTexture, "assets/Images/setting.png", 250.0f, 150.0f, 1630.f, 30.0f));

    // --- [4] Nút “Help” ---
    btnHelpSprite = std::make_unique<sf::Sprite>(
        createSprite(btnHelpTexture, "assets/Images/faq.png", 250.0f, 150.0f, 30.f, 30.0f));

    // --- [4.5] Nút “about” ---
    btnExtraSprite = std::make_unique<sf::Sprite>(
        createSprite(btnExtraTexture, "assets/Images/about.png", 250.0f, 150.0f, 30.0f, 650.0f));

    // --- [5] Text hiển thị tiêu đề / thông tin game ---
    infoText = std::make_unique<sf::Text>(
        createText(font, L"https://github.com/BTLCode4T/Game", 15, sf::Color::White, 850.0f, 25.0f));

    versionText = std::make_unique<sf::Text>(createText(font, L"v1.0.0", 15, sf::Color::White, 975.0f, 50.0f));
    
    // --- [6] Mở nhạc nền

    MusicManager::Get().Play("menu");
    
    checkContinueAvailable();

    myNewImageSprite = std::make_unique<sf::Sprite>(createSprite(myNewImageTexture,
                                                                 "assets/Images/bg.png", // 👈 THAY ĐƯỜNG DẪN
                                                                 WINDOW_WIDTH,           // 👈 Chiều rộng mong muốn
                                                                 WINDOW_HEIGHT,          // 👈 Chiều cao mong muốn
                                                                 0.0f,                   // 👈 Vị trí X
                                                                 0.0f                    // 👈 Vị trí Y
                                                                 ));
    logoSprite =
        std::make_unique<sf::Sprite>(createSprite(logoTexture,
                                                  "assets/Images/LogoDino.png", // 👈 Nhớ thay tên file ảnh logo của bạn
                                                  1000.0f, // Chiều rộng Logo (bằng chiều rộng nút start cho đẹp)
                                                  590.0f,  // Chiều cao Logo
                                                  420.0f,  // Vị trí X (bằng X của nút Start để thẳng hàng)
                                                  -20.0f   // Vị trí Y (nhỏ hơn 200 để nằm PHÍA TRÊN nút Start)
                                                  ));
    githubLinkText = std::make_unique<sf::Text>(
        createText(font, L"https://github.com/BTLCode4T/Game", 24, sf::Color::Yellow, 300.0f, 650.0f));

    isLinkVisible = false; // Mặc định ẩn
}

/* --- Hàm Render của MainMenuUI ---
 * Nhiệm vụ: Vẽ toàn bộ các phần tử giao diện chính lên cửa sổ.
 */
// --- CHỈNH SỬA HÀM NÀY: CHỈ CHECK TRẠNG THÁI, KHÔNG ĐỔI VỊ TRÍ ---
void MainMenuUI::checkContinueAvailable() {
    m_canContinue = IsSaveFileValid();
    
    // Sắp xếp vị trí nút
    if (m_canContinue) {
        btnContinueSprite->setPosition({810.0f,650.0f});
    }
}
void MainMenuUI::Render(sf::RenderWindow &window, const sf::Font &font) {
    checkContinueAvailable();
    window.draw(backgroundSprite);

    window.draw(sunSprite);
    window.draw(treeSprite);
    window.draw(*myNewImageSprite);
    window.draw(*logoSprite);
    window.draw(*btnNewSprite);

   if (m_canContinue) 
        window.draw(*btnContinueSprite);

    window.draw(*btnHighScoresSprite);
    window.draw(*btnSettingsSprite);
    window.draw(*btnHelpSprite);
    window.draw(*btnExtraSprite);
    if (isLinkVisible) {
        window.draw(*githubLinkText);
    }
    // window.draw(*versionText);
}

/* ============================================================
 * CLASS: HighScoresUI — Màn hình bảng xếp hạng
 * ============================================================ */
HighScoresUI::HighScoresUI(const sf::Sprite &bg, const sf::Sprite &homeBtn, const sf::Font &font)
    : backgroundSprite(bg), btnHomeSprite(homeBtn) {
    // --- [1] Tiêu đề ---
    titleText = std::make_unique<sf::Text>(createText(font, L"BẢNG XẾP HẠNG", 40, sf::Color::Red, 960.0f, 205.0f));

    // --- [2] Dòng hiển thị điểm số người chơi ---

    tableListSprite = std::make_unique<sf::Sprite>(
        createSprite(tableListTexture, "assets/Images/list.png", 670.5f, 502.5f, 615.0f, 130.0f));

    btnNoneSprite1 = std::make_unique<sf::Sprite>(
        createSprite(btnNoneTexture1, "assets/Images/none.png", 225.0f, 55.5f, 1300.0f, 200.0f));

    btnNoneSprite2 = std::make_unique<sf::Sprite>(
        createSprite(btnNoneTexture2, "assets/Images/none.png", 225.0f, 55.5f, 1300.0f, 300.0f));

    btnNoneSprite3 = std::make_unique<sf::Sprite>(
        createSprite(btnNoneTexture3, "assets/Images/none.png", 225.0f, 55.5f, 1300.0f, 400.0f));

    btnNoneSprite4 = std::make_unique<sf::Sprite>(
        createSprite(btnNoneTexture4, "assets/Images/none.png", 225.0f, 55.5f, 1300.0f, 500.0f));

    btnTextInputSprite = std::make_unique<sf::Sprite>(
        createSprite(btnTextInputTexture, "assets/Images/TextInput.png", 225.0f, 55.5f, 837.5f, 630.0f));

    notFoundSprite = std::make_unique<sf::Sprite>(
        createSprite(notFoundSpriteTexture, "assets/Images/btnone.png", 420.0f, 333.0f, 200.0f, 150.0f));

    // Khu vực tạo text sắp xếp
    // std::unique_ptr<sf::Text> decreaingScore;
    // std::unique_ptr<sf::Text> increaingScore;
    // std::unique_ptr<sf::Text> decreaingTime;
    // std::unique_ptr<sf::Text> decreaingTime;

    // --- THAY THẾ: Khởi tạo prompt và input riêng biệt ---

    promptText = std::make_unique<sf::Text>(
        createText(font, L"Tìm theo điểm", 22, sf::Color(128, 128, 128), 950.0f, 655.0f, true));
    inputTextDisplay =
        std::make_unique<sf::Text>(font, "", 22); // SỬA: Sử dụng constructor có tham số đúng thứ tự: font, string, size
    inputTextDisplay->setFillColor(sf::Color::Red);
    inputTextDisplay->setPosition(sf::Vector2f(890.0f, 645.5f)); // SỬA: Sử dụng Vector2f
    inputTextDisplay->setStyle(sf::Text::Style::Bold);           // Giữ bold nếu có (tùy chọn)

    // --- THÊM: Khởi tạo con trỏ "|" ---
    cursorShape.setSize(sf::Vector2f(2.0f, 22.0f)); // Kích thước "|" (rộng 2px, cao bằng font size)
    cursorShape.setFillColor(sf::Color::Black);     // Màu đen
    cursorShape.setPosition(
        sf::Vector2f(892.f, 560.0f)); // SỬA: Sử dụng Vector2f Vị trí sau prompt/input (điều chỉnh offset nếu cần)

    // currentInput giữ nguyên rỗng ban đầu
    currentInput = "";

    decreaingScore =
        std::make_unique<sf::Text>(createText(font, L"Điểm giảm dần", 20, sf::Color::White, 1410.0f, 222.0f));

    increaingScore =
        std::make_unique<sf::Text>(createText(font, L"Điểm tăng dần", 20, sf::Color::White, 1410.0f, 322.0f));

    decreaingTime =
        std::make_unique<sf::Text>(createText(font, L"Thời gian giảm dần", 20, sf::Color::White, 1410.0f, 422.0f));

    increaingTime =
        std::make_unique<sf::Text>(createText(font, L"Thời gian tăng dần", 20, sf::Color::White, 1410.0f, 522.0f));

    searchNone =
        std::make_unique<sf::Text>(createText(font, L"Không tìm thấy !", 30, sf::Color::Yellow, 400.0f, 220.0f));
    searchNone2 = std::make_unique<sf::Text>(
        createText(font, L"Ấn vị trí trống bất \n\nkì để tiếp tục !", 25, sf::Color::Blue, 400.0f, 330.0f));

    scrollText = std::make_unique<sf::Text>(
        createText(font, L"< Ấn phím mũi tên ^/v để cuộn! >", 15, sf::Color::Yellow, 950.0f, 585.0f));

    // Load scores từ file ngay khi tạo UI
    initList(scoresList);
    readFile("Scores.txt", scoresList);

    menuBgSprite = std::make_unique<sf::Sprite>(
        createSprite(menuBgTexture, "assets/Images/bg.png", WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 0.0f));
    // Phát nhạc
}

/* --- Render HighScoresUI ---
 * Vẽ nền, nút home và các text liên quan đến bảng điểm.
 */
void HighScoresUI::Render(sf::RenderWindow &window, const sf::Font &font) {
    /*window.draw(backgroundSprite);*/
    window.draw(*menuBgSprite);
    window.draw(btnHomeSprite);
    window.draw(*tableListSprite);
    window.draw(*titleText);
    window.draw(*scrollText);

    // *** DÙNG scoresList THÀNH VIÊN ĐÃ TẢI ***
    drawScoresList(window, scoresList, font, 745.0f, 270.0f, scrollIndex);

    window.draw(*btnNoneSprite1);
    window.draw(*btnNoneSprite2);
    window.draw(*btnNoneSprite3);
    window.draw(*btnNoneSprite4);

    window.draw(*decreaingScore);
    window.draw(*increaingScore);
    window.draw(*decreaingTime);
    window.draw(*increaingTime);

    window.draw(*btnTextInputSprite);

    if (!isInputActive) {
        window.draw(*promptText);
    } else {
        window.draw(*inputTextDisplay);
        float elapsed = cursorBlinkClock.getElapsedTime().asSeconds();
        if (elapsed > blinkInterval) {
            cursorVisible = !cursorVisible;
            cursorBlinkClock.restart();
        }
        if (cursorVisible) {
            sf::Color cursorColor = cursorShape.getFillColor();
            cursorColor.a = 200;
            cursorShape.setFillColor(cursorColor);
            window.draw(cursorShape);
        }
    }
    window.draw(*decreaingScore);

    if (isNotFoundVisible) {
        window.draw(*notFoundSprite);
        window.draw(*searchNone);
        window.draw(*searchNone2);
    }
}

/* ============================================================
 * CLASS: HelpUI — Màn hình hướng dẫn chơi
 * ============================================================ */
HelpUI::HelpUI(const sf::Sprite &bg, const sf::Sprite &homeBtn, const sf::Font &font)
    : backgroundSprite(bg), btnHomeSprite(homeBtn) {
    menuBgSprite = std::make_unique<sf::Sprite>(
        createSprite(menuBgTexture, "assets/Images/bg.png", WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 0.0f));
     helpText = std::make_unique<sf::Text>(createText(font, L"HƯỚNG DẪN:\n\n- Mục tiêu: Chạy vô tận, né chướng ngại vật (bẫy, vật cản) để đạt điểm cao.\n\n- Nhảy: Spacebar hoặc Mũi tên Lên\n- Bắn: Dùng chuột trái\n- Di chuyển: Mũi tên Trái/Phải hoặc phím A, D\n\n- Game có Mạng sống (Trái tim); hết mạng Game Over.\n- Điều chỉnh Âm thanh/Nhạc trong Cài đặt.\n- Xem thành tích tại Bảng Điểm Cao.",
                                                28, sf::Color::Black, 950.0f, 300.0f));

    // --- [2] Thông tin tác giả ---
    aboutText =
        std::make_unique<sf::Text>(createText(font, L"CHƠI GAME QUÁ 180 PHÚT MỖI NGÀY\n\t\tSẼ CÓ HẠI CHO SỨC KHỎE", 24, sf::Color::Yellow, 950.0f, 600.0f));
}

/* --- Render HelpUI ---
 * Vẽ màn hình hướng dẫn và nút trở về home.
 */
void HelpUI::Render(sf::RenderWindow &window, const sf::Font &font) {
    /* window.draw(backgroundSprite);*/
    window.draw(*menuBgSprite);
    window.draw(btnHomeSprite);
    window.draw(*helpText);
    window.draw(*aboutText);
}

/* ============================================================
 * CLASS: SettingsUI — Màn hình cài đặt
 * ============================================================ */
SettingsUI::SettingsUI(const sf::Sprite &bg, const sf::Sprite &homeBtn, const sf::Font &font)
    : backgroundSprite(bg), btnHomeSprite(homeBtn) {
    menuBgSprite = std::make_unique<sf::Sprite>(
        createSprite(menuBgTexture, "assets/Images/bg.png", WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 0.0f));
    // --- [1] Text thông báo đang phát triển ---
    settingsText = std::make_unique<sf::Text>(
        createText(font, L"Tùy chỉnh (đang phát triển)", 28, sf::Color::White, 950.0f, 200.0f));

    // --- [2] Text Âm thanh

    AudioSettingText = std::make_unique<sf::Text>(createText(font, L"Âm thanh", 28, sf::Color::White, 830.0f, 320.0f));

    MusicSettingText =
        std::make_unique<sf::Text>(createText(font, L"Âm thanh nhạc nền", 28, sf::Color::White, 830.0f, 450.0f));

    btnNoneSprite =
        std::make_unique<sf::Sprite>(createSprite(btnNone, "assets/Images/btnone.png", 630.0f, 500.0f, 635.0f, 160.0f));

    btnNoneSprite =
        std::make_unique<sf::Sprite>(createSprite(btnNone, "assets/Images/btnone.png", 630.0f, 500.0f, 630.0f, 160.0f));

    mutedSprite = std::make_unique<sf::Sprite>(
        createSprite(mutedTexture, "assets/Images/music_off.png", 100.0f, 100.0f, 1050.0f, 270.0f));

    unmutedSprite = std::make_unique<sf::Sprite>(
        createSprite(unmutedTexture, "assets/Images/misic.png", 100.0f, 100.0f, 1050.0f, 270.0f));

    musicMutedSprite = std::make_unique<sf::Sprite>(
        createSprite(musicMutedTexture, "assets/Images/sound_off.png", 100.0f, 100.0f, 1050.0f, 410.0f)); // Vị trí mới

    musicUnmutedSprite = std::make_unique<sf::Sprite>(
        createSprite(musicUnmutedTexture, "assets/Images/sound.png", 100.0f, 100.0f, 1050.0f, 410.0f)); // Vị trí mới
}

/* --- Render SettingsUI ---
 * Vẽ nền, nút home và thông tin placeholder.
 */
void SettingsUI::Render(sf::RenderWindow &window, const sf::Font &font) {
    /*window.draw(backgroundSprite);*/
    window.draw(*menuBgSprite);
    window.draw(btnHomeSprite);
    // window.draw(*settingsText);
    window.draw(*btnNoneSprite);

    window.draw(*AudioSettingText);
    window.draw(*MusicSettingText);

    // --- BỔ SUNG: Vẽ nút Muted/Unmuted ---
    if (Audio::Get().IsMuted()) {
        window.draw(*mutedSprite);
    } else {
        window.draw(*unmutedSprite);
    }

    // BỔ SUNG: Music Mute/Unmute
    if (MusicManager::Get().IsMuted()) {
        window.draw(*musicMutedSprite);
    } else {
        window.draw(*musicUnmutedSprite);
    }
}

GameOverUI::GameOverUI(const sf::Sprite &bg, const sf::Font &font) : backgroundSprite(bg) {

    btnNoneSprite =
        std::make_unique<sf::Sprite>(createSprite(btnNone, "assets/Images/btnone.png", 630.0f, 500.0f, 620.0f, 150.0f));

    btnHomeSprite = std::make_unique<sf::Sprite>(
        createSprite(btnHomeTexture, "assets/Images/Home.png", 200.0f, 200.0f, 560.0f, 325.0f));

    btnUndoSprite = std::make_unique<sf::Sprite>(
        createSprite(btnUndoTexture, "assets/Images/restart.png", 140.0f, 140.0f, 530.0f, 325.0f));

    gameOverText = std::make_unique<sf::Text>(createText(
        font, L"Bạn đã thua!\n\nDIEM CUA BAN: 0\n\nThời gian: **/**/****", 28, sf::Color::White, 450.0f, 220.0f, true));

    finalScore = 0; // Khởi tạo điểm
}

void GameOverUI::Render(sf::RenderWindow &window, const sf::Font &font) {
    // 1. Reset Camera
    window.setView(window.getDefaultView());
    sf::Vector2f center(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    // 2. Background Full màn hình
    sf::Sprite bg = backgroundSprite;
    const sf::Texture &tex = bg.getTexture();
    bg.setTextureRect(sf::IntRect({0, 0}, {int(tex.getSize().x), int(tex.getSize().y)}));
    bg.setScale({window.getSize().x / float(tex.getSize().x), window.getSize().y / float(tex.getSize().y)});
    bg.setPosition({0, 0});
    window.draw(bg);

    // 3. Nút bấm
    if (btnNoneSprite)
        window.draw(*btnNoneSprite);
    if (btnHomeSprite) {
        btnHomeSprite->setPosition({center.x + 10, center.y + 0});
        window.draw(*btnHomeSprite);
    }
    if (btnUndoSprite) {
        btnUndoSprite->setPosition({center.x - 190, center.y + 30});
        window.draw(*btnUndoSprite);
    }
    
    if (gameOverText) {
        sf::FloatRect b = gameOverText->getLocalBounds();
        gameOverText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});

        gameOverText->setPosition({center.x, center.y - 100});
        window.draw(*gameOverText);
    }
}

void GameOverUI::setScore(int score) {
    finalScore = score;

    // 1. Lấy thời điểm hiện tại
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm *ltm = std::localtime(&now);

    // 2. Định dạng chuỗi thời gian (HH:MM DD/MM/YYYY)
    std::wstringstream time_ss;
    // Dùng setw và setfill để thêm số 0 đứng đầu nếu cần
    time_ss << std::setw(2) << std::setfill(L'0') << ltm->tm_hour << L":" << std::setw(2) << std::setfill(L'0')
            << ltm->tm_min << L" " << std::setw(2) << std::setfill(L'0') << ltm->tm_mday << L"/" << std::setw(2)
            << std::setfill(L'0') << (ltm->tm_mon + 1) << L"/" << (ltm->tm_year + 1900);

    // 3. Cập nhật chuỗi hiển thị
    std::wstringstream wss;
    wss << L"Bạn đã thua!\n\nĐiểm của bạn: " << finalScore << L"\n\nThời gian: "
        << time_ss.str(); // Dùng chuỗi đã định dạng

    gameOverText->setString(wss.str());
}

void drawScoresList(sf::RenderWindow &window, const List &l, const sf::Font &font, float startX, float startY,
                    int startIndex) {
    Node *p = l.head;
    float currentY = startY;

    // --- [1] Vẽ Tiêu đề ---
    sf::Text headerText(font, L"ĐIỂM\t\t    THỜI GIAN\t\t  NGÀY", 22);
    headerText.setFillColor(sf::Color(255, 215, 0));
    headerText.setPosition(sf::Vector2f(755.0f, 255.0f));
    window.draw(headerText);
    currentY += 35.0f;

    // --- [2] BỎ QUA CÁC NODE DỰA TRÊN SCROLL INDEX ---
    int skip = startIndex;
    while (p && skip > 0) {
        p = p->next;
        skip--;
    }

    // --- [3] Duyệt và vẽ (Tối đa 5 dòng từ vị trí hiện tại) ---
    const int maxLines = 5;
    int currentLine = 0;

    while (p && currentLine < maxLines) {
        // ... (Code xử lý chuỗi wstringstream giữ nguyên như cũ) ...
        std::wstringstream wss;
        std::wstring time_str = (p->t.hour < 10 ? L"0" : L"") + std::to_wstring(p->t.hour) + L":" +
                                (p->t.minute < 10 ? L"0" : L"") + std::to_wstring(p->t.minute);
        std::wstring date_str = (p->d.day < 10 ? L"0" : L"") + std::to_wstring(p->d.day) + L"/" +
                                (p->d.month < 10 ? L"0" : L"") + std::to_wstring(p->d.month) + L"/" +
                                std::to_wstring(p->d.year);

        wss << std::right << std::setw(8) << p->point;
        wss << L"               ";
        wss << std::left << std::setw(12) << time_str;
        wss << L"      ";
        wss << std::left << std::setw(15) << date_str;

        sf::Text scoreLineText(font, wss.str(), 23);
        scoreLineText.setFillColor(sf::Color::White);
        scoreLineText.setPosition(sf::Vector2f(startX, currentY));

        window.draw(scoreLineText);
        currentY += 54.0f;

        p = p->next;
        currentLine++;
    }
}

HighScoresUI::~HighScoresUI() { // *** KHẮC PHỤC LỖI DESTUCTOR: Định nghĩa hàm ***
    deleteList(scoresList);     // Gọi hàm giải phóng danh sách
}

void HighScoresUI::LoadScores() {
    // 1. Giải phóng danh sách cũ
    deleteList(scoresList);

    // 2. Khởi tạo lại danh sách (initList cũng được gọi trong deleteList nếu head/tail là nullptr)
    initList(scoresList);

    // 3. Đọc dữ liệu mới từ file
    readFile("Scores.txt", scoresList);

    // 4. Reset cuộn về đầu trang (quan trọng khi danh sách thay đổi)
    resetScroll();

    std::cout << "High scores list reloaded from file." << std::endl;
}
// *** SỬA: Định nghĩa member function đúng cách ***
void HighScoresUI::setInputActive(bool active) {
    isInputActive = active;
    if (active) {
        cursorBlinkClock.restart(); // restart blink
        cursorVisible = true;
        sf::FloatRect textBounds = inputTextDisplay->getLocalBounds();
        cursorShape.setPosition(sf::Vector2f(inputTextDisplay->getPosition().x + textBounds.size.x + 2.0f,
                                             inputTextDisplay->getPosition().y));

    } else {
        // Nếu tắt input, ẩn cursor
        cursorVisible = false;
    }
}

// Thêm ký tự (chỉ cho phép số và Backspace), cập nhật vị trí con trỏ
void HighScoresUI::addCharToInput(std::uint32_t unicode) {
    if (!isInputActive)
        return;

    // Backspace (code = 8) hoặc Delete tùy ý
    if (unicode == 8) { // Backspace
        if (!currentInput.empty()) {
            currentInput.pop_back();
        }
    } else if (unicode >= '0' && unicode <= '9') { // chỉ cho phép số
        currentInput += static_cast<char>(unicode);
    }
    // Nếu muốn cho phép xóa toàn bộ với ESC, hoặc dấu khác thêm ở đây

    // Cập nhật text hiển thị
    inputTextDisplay->setString(sf::String::fromUtf8(currentInput.begin(), currentInput.end()));

    // Cập nhật vị trí con trỏ (cursor)
    sf::FloatRect textBounds = inputTextDisplay->getLocalBounds();
    // Nếu text có origin != 0, xử lý tương ứng (ở đây giả định origin là (0,0))
    // sf::Vector2f(inputTextDisplay->getPosition().x + textBounds + 2.0f, inputTextDisplay->getPosition().y);
    cursorShape.setPosition(
        sf::Vector2f(inputTextDisplay->getPosition().x + textBounds.size.x + 2.0f, inputTextDisplay->getPosition().y));
}

void HighScoresUI::handleInputFinished(List &l) {
    if (!isInputActive)
        return;
    isInputActive = false;
    resetScroll();
    try {
        if (currentInput.empty()) {
            // Nếu rỗng thì reset list
            deleteList(l);
            initList(l);
            readFile("Scores.txt", l);
            isNotFoundVisible = false; // Ẩn thông báo nếu có
        } else {
            int point = std::stoi(currentInput);

            // SỬA: Gọi searchByPoint và kiểm tra kết quả
            bool found = searchByPoint(l, point);

            if (!found) {
                isNotFoundVisible = true; // Không thấy -> Bật ảnh
            } else {
                isNotFoundVisible = false; // Thấy -> Tắt ảnh
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Loi chuyen doi diem: " << e.what() << std::endl;
    }

    currentInput.clear();
    inputTextDisplay->setString("");
    cursorVisible = false;
    cursorBlinkClock.restart();
}

void HighScoresUI::resetInputState() {
    currentInput = "";
    inputTextDisplay->setString("");
    cursorVisible = true;
    cursorBlinkClock.restart();
    isNotFoundVisible = false;
}

void HighScoresUI::scrollUp() {
    if (scrollIndex > 0) {
        Audio::Get().Play("switch_type");
        scrollIndex--;
    }
}

void HighScoresUI::scrollDown() {
    int totalNodes = countList(scoresList);
    // Chỉ cho cuộn nếu còn phần tử phía dưới chưa hiển thị
    if (scrollIndex + MAX_LINES < totalNodes) {
        Audio::Get().Play("switch_type");
        scrollIndex++;
    }
}

void HighScoresUI::resetScroll() {
    scrollIndex = 0;
}

GameInfoUI::GameInfoUI(const sf::Sprite &bg, const sf::Sprite &homeBtn, const sf::Font &font)
    : backgroundSprite(bg), btnHomeSprite(homeBtn) {
    menuBgSprite = std::make_unique<sf::Sprite>(
        createSprite(menuBgTexture, "assets/Images/bg.png", WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 0.0f));
    // --- [1] Text hướng dẫn điều khiển ---
        helpText = std::make_unique<sf::Text>(
        createText(font, L"\t\tPhiên bản: v0.0.1\n\nBản quyền thuộc về Code4T - 2025\n", 28, sf::Color::Red, WINDOW_WIDTH / 2.0f,
                                                      WINDOW_HEIGHT / 2.0f + 50.0f ));

    // --- [2] Thông tin tác giả ---
    aboutText =
        std::make_unique<sf::Text>(createText(font, L"Tác giả: Code 4T", 24, sf::Color(200, 200, 200), 500.0f, 550.0f));
    aboutText = std::make_unique<sf::Text>(createText(font,
                                                      L"Github: https://github.com/BTLCode4T/Game\n\n", // Nội dung Link
                                                      30,                                           // Cỡ chữ
                                                      sf::Color::Black,                            // Màu chữ
                                                      WINDOW_WIDTH / 2.0f,
                                                      WINDOW_HEIGHT / 2.0f - 100.0f // Vị trí (x, y) - chỉnh lại cho vừa mắt
                                                      ));
}

/* --- Render HelpUI ---

 * Vẽ màn hình hướng dẫn và nút trở về home.
 */
void GameInfoUI::Render(sf::RenderWindow &window, const sf::Font &font) {
    /* window.draw(backgroundSprite);*/
    window.draw(*menuBgSprite);
    window.draw(btnHomeSprite);
    window.draw(*helpText);
    window.draw(*aboutText);
   /* window.draw(btnHomeSprite);*/
}
