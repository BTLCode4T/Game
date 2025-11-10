#include "GamePlay/UI/StateUI.h"
#include "GamePlay/UI/Scores.h"
#include "GamePlay/UI/UI.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

/* ============================================================
 *   CLASS: MainMenuUI — Giao diện chính của game
 * ============================================================ */
MainMenuUI::MainMenuUI(const sf::Sprite &bg, const sf::Sprite &sun, const sf::Sprite &tree, const sf::Font &font)
    : backgroundSprite(bg), sunSprite(sun), treeSprite(tree) {

    // --- [1] Nút “New Game” ---
    // Dùng std::make_unique để tạo sprite, truyền texture và vị trí nút
    btnNewSprite = std::make_unique<sf::Sprite>(
        createSprite(btnNewTexture, "assets/Images/new.png", 300.0f, 70.0f, 333.2f, 150.0f));

    // --- [2] Nút “High Scores” ---
    btnHighScoresSprite = std::make_unique<sf::Sprite>(
        createSprite(btnHighScoresTexture, "assets/Images/HighScores.png", 300.0f, 70.0f, 333.2f, 250.0f));

    // --- [3] Nút “Settings” ---
    btnSettingsSprite = std::make_unique<sf::Sprite>(
        createSprite(btnSettingsTexture, "assets/Images/Settings.png", 300.0f, 70.0f, 333.2f, 350.0f));

    // --- [4] Nút “Help” ---
    btnHelpSprite = std::make_unique<sf::Sprite>(
        createSprite(btnHelpTexture, "assets/Images/Help.png", 300.0f, 70.0f, 333.2f, 450.0f));

    // --- [5] Text hiển thị tiêu đề / thông tin game ---
    infoText = std::make_unique<sf::Text>(
        createText(font, L"https://github.com/BTLCode4T/Game", 15, sf::Color::White, 850.0f, 25.0f));

    versionText = std::make_unique<sf::Text>(createText(font, L"v1.0.0", 15, sf::Color::White, 975.0f, 50.0f));
}

/* --- Hàm Render của MainMenuUI ---
 * Nhiệm vụ: Vẽ toàn bộ các phần tử giao diện chính lên cửa sổ.
 */
void MainMenuUI::Render(sf::RenderWindow &window, const sf::Font &font) {
    window.draw(backgroundSprite);
    window.draw(sunSprite);
    window.draw(treeSprite);
    window.draw(*btnNewSprite);
    window.draw(*btnHighScoresSprite);
    window.draw(*btnSettingsSprite);
    window.draw(*btnHelpSprite);
    window.draw(*infoText);
    window.draw(*versionText);
}

/* ============================================================
 *   CLASS: HighScoresUI — Màn hình bảng xếp hạng
 * ============================================================ */
HighScoresUI::HighScoresUI(const sf::Sprite &bg, const sf::Sprite &homeBtn, const sf::Font &font)
    : backgroundSprite(bg), btnHomeSprite(homeBtn) {

    // --- [1] Tiêu đề ---
    titleText = std::make_unique<sf::Text>(createText(font, L"BẢNG XẾP HẠNG", 40, sf::Color::Red, 350.0f, 125.0f));

    // --- [2] Dòng hiển thị điểm số người chơi ---
    

    tableListSprite = std::make_unique<sf::Sprite>(
        createSprite(tableListTexture, "assets/Images/list.png", 670.5f, 502.5f, 25.0f, 60.0f));

    btnNoneSprite1 = std::make_unique<sf::Sprite>(
        createSprite(btnNoneTexture1, "assets/Images/none.png", 225.0f, 52.5f, 750.0f, 125.0f));


    btnNoneSprite2 = std::make_unique<sf::Sprite>(
        createSprite(btnNoneTexture2, "assets/Images/none.png", 225.0f, 52.5f, 750.0f, 225.0f));

    btnNoneSprite3 = std::make_unique<sf::Sprite>(
        createSprite(btnNoneTexture3, "assets/Images/none.png", 225.0f, 52.5f, 750.0f, 325.0f));

    btnNoneSprite4 = std::make_unique<sf::Sprite>(
        createSprite(btnNoneTexture4, "assets/Images/none.png", 225.0f, 52.5f, 750.0f, 425.0f));

    // Khu vực tạo text sắp xếp
    // std::unique_ptr<sf::Text> decreaingScore;
    // std::unique_ptr<sf::Text> increaingScore;
    // std::unique_ptr<sf::Text> decreaingTime;
    // std::unique_ptr<sf::Text> decreaingTime;

    decreaingScore =
        std::make_unique<sf::Text>(createText(font, L"Điểm giảm dần", 20, sf::Color::White, 855.0f, 150.0f));

    increaingScore =
        std::make_unique<sf::Text>(createText(font, L"Điểm tăng dần", 20, sf::Color::White, 855.0f, 250.0f));

    decreaingTime =
        std::make_unique<sf::Text>(createText(font, L"Thời gian giảm dần", 20, sf::Color::White, 855.0f, 350.0f));

    increaingTime =
        std::make_unique<sf::Text>(createText(font, L"Thời gian tăng dần", 20, sf::Color::White, 855.0f, 450.0f));

}

/* --- Render HighScoresUI ---
 * Vẽ nền, nút home và các text liên quan đến bảng điểm.
 */
/* --- Render HighScoresUI ---
 * Vẽ nền, nút home và các text liên quan đến bảng điểm.
 */
void HighScoresUI::Render(sf::RenderWindow &window, const sf::Font &font) {
    List l;
    initList(l);

    readFile("Scores.txt", l);

    // // Vị trí bắt đầu vẽ danh sách điểm
    // float startX = 50.0f;
    // float startY = 200.0f;

    // GỌI HÀM VẼ ĐIỂM SỐ (KHẮC PHỤC LỖI 87)

    window.draw(backgroundSprite);
    window.draw(btnHomeSprite);
    window.draw(*tableListSprite); // Giả định tableListSprite là nền bảng điểm
    window.draw(*titleText);
    drawScoresList(window, l, font, 120.0f, 200.0f);
    window.draw(*btnNoneSprite1); // decreaingScore
    window.draw(*btnNoneSprite2); // increasingScor
    window.draw(*btnNoneSprite3); // decreaingTime
    window.draw(*btnNoneSprite4); // increaingTime
    window.draw(*decreaingScore);
    window.draw(*increaingScore);
    window.draw(*decreaingTime);
    window.draw(*increaingTime);


    // Khu vực kiểm tra ấn nút và chạy hàm sắp xếp









    // Lưu ý: Sau khi sử dụng, bạn nên có một hàm để giải phóng bộ nhớ (ví dụ: deleteList(l);)
}

/* ============================================================
 *   CLASS: HelpUI — Màn hình hướng dẫn chơi
 * ============================================================ */
HelpUI::HelpUI(const sf::Sprite &bg, const sf::Sprite &homeBtn, const sf::Font &font)
    : backgroundSprite(bg), btnHomeSprite(homeBtn) {

    // --- [1] Text hướng dẫn điều khiển ---
    helpText = std::make_unique<sf::Text>(createText(font, L"Hướng dẫn:\n- Dùng mũi tên để di chuyển\n- Space để nhảy",
                                                     28, sf::Color::White, 500.0f, 200.0f));

    // --- [2] Thông tin tác giả ---
    aboutText =
        std::make_unique<sf::Text>(createText(font, L"Tác giả: Code 4T", 24, sf::Color(200, 200, 200), 500.0f, 550.0f));
}

/* --- Render HelpUI ---
 * Vẽ màn hình hướng dẫn và nút trở về home.
 */
void HelpUI::Render(sf::RenderWindow &window, const sf::Font &font) {
    window.draw(backgroundSprite);
    window.draw(btnHomeSprite);
    window.draw(*helpText);
    window.draw(*aboutText);
}

/* ============================================================
 *   CLASS: SettingsUI — Màn hình cài đặt
 * ============================================================ */
SettingsUI::SettingsUI(const sf::Sprite &bg, const sf::Sprite &homeBtn, const sf::Font &font)
    : backgroundSprite(bg), btnHomeSprite(homeBtn) {

    // --- [1] Text thông báo đang phát triển ---
    settingsText = std::make_unique<sf::Text>(
        createText(font, L"Tùy chỉnh (đang phát triển)", 28, sf::Color::White, 500.0f, 250.0f));
}

/* --- Render SettingsUI ---
 * Vẽ nền, nút home và thông tin placeholder.
 */
void SettingsUI::Render(sf::RenderWindow &window, const sf::Font &font) {
    window.draw(backgroundSprite);
    window.draw(btnHomeSprite);
    window.draw(*settingsText);
}

void drawScoresList(sf::RenderWindow &window, const List &l, const sf::Font &font, float startX, float startY) {
    Node *p = l.head;
    float currentY = startY;

    // --- [1] Vẽ Tiêu đề ---
    // Giữ nguyên phần này
    sf::Text headerText(font, L"ĐIỂM\t\t\tTHỜI GIAN\t\t\tNGÀY", 22);
    headerText.setFillColor(sf::Color::Yellow);
    headerText.setPosition(sf::Vector2f(135.0f, 175.0f));
    window.draw(headerText);
    currentY += 35.0f;

    // --- [2] Duyệt và vẽ từng điểm số ---
    const int maxLines = 5;
    int currentLine = 0;

    // Vòng lặp sẽ dừng khi p là nullptr (cuối danh sách) HOẶC khi đã vẽ 5 dòng
    while (p && currentLine < maxLines) {
        std::wstringstream wss;

        // --- BƯỚC 1: Xây dựng chuỗi Time và Date để setw có thể áp dụng ---

        std::wstring time_str = (p->t.hour < 10 ? L"0" : L"") + std::to_wstring(p->t.hour) + L":" +
                                (p->t.minute < 10 ? L"0" : L"") + std::to_wstring(p->t.minute);

        std::wstring date_str = (p->d.day < 10 ? L"0" : L"") + std::to_wstring(p->d.day) + L"/" +
                                (p->d.month < 10 ? L"0" : L"") + std::to_wstring(p->d.month) + L"/" +
                                std::to_wstring(p->d.year);

        // --- BƯỚC 2: Định dạng wstringstream bằng setw và thêm khoảng trắng phân tách ---

        // 1. Cột Point (Rộng 8, Căn phải)
        wss << std::right << std::setw(8) << p->point;
        wss << L"               "; // <<< THÊM 3 KHOẢNG TRẮNG PHÂN TÁCH SAU ĐIỂM

        // 2. Cột Time (Rộng 12, Căn trái)
        wss << std::left << std::setw(12) << time_str;
        wss << L"      "; // <<< THÊM 3 KHOẢNG TRẮNG PHÂN TÁCH SAU THỜI GIAN

        // 3. Cột Date (Rộng 15, Căn trái)
        wss << std::left << std::setw(15) << date_str;

        // ... (Tiếp tục tạo và vẽ sf::Text) ...
        sf::Text scoreLineText(font, wss.str(), 23);
        scoreLineText.setFillColor(sf::Color::White);
        scoreLineText.setPosition(sf::Vector2f(startX, currentY));

        window.draw(scoreLineText);
        currentY += 54.0f;

        p = p->next;
        currentLine++; // Tăng biến đếm dòng
    }
}