#include "GamePlay/UI/StateUI.h"
#include "GamePlay/UI/UI.h"
#include <iostream>

/* ============================================================
 *   CLASS: MainMenuUI — Giao diện chính của game
 * ============================================================ */
MainMenuUI::MainMenuUI(const sf::Sprite &bg, const sf::Sprite &sun, const sf::Sprite &tree, const sf::Font &font)
    : backgroundSprite(bg), sunSprite(sun), treeSprite(tree) {

    // --- [1] Nút “New Game” ---
    // Dùng std::make_unique để tạo sprite, truyền texture và vị trí nút
    btnNewSprite = std::make_unique<sf::Sprite>(
        createSprite(btnNewTexture, "assets/Images/new.png", 300.0f, 70.0f, 333.2f, 150.0f)
    );

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
        createText(font, L"https://github.com/BTLCode4T/Game", 15, sf::Color::White, 850.0f, 25.0f)
    );

    versionText = std::make_unique<sf::Text>(
        createText(font, L"v1.0.0", 15, sf::Color::White, 975.0f, 50.0f)
    );


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
    titleText = std::make_unique<sf::Text>(
        createText(font, L"Bảng xếp hạng", 40, sf::Color::Yellow, 500.0f, 150.0f)
    );

    // --- [2] Dòng hiển thị điểm số người chơi ---
    idCharacterText = std::make_unique<sf::Text>(
        createText(font, L"Ngọc Tiên #1", 28, sf::Color::White, 500.0f, 250.0f)
    );
}

/* --- Render HighScoresUI ---
 * Vẽ nền, nút home và các text liên quan đến bảng điểm.
 */
void HighScoresUI::Render(sf::RenderWindow &window, const sf::Font &font) {
    window.draw(backgroundSprite);
    window.draw(btnHomeSprite);
    window.draw(*titleText);
    window.draw(*idCharacterText);
}

/* ============================================================
 *   CLASS: HelpUI — Màn hình hướng dẫn chơi
 * ============================================================ */
HelpUI::HelpUI(const sf::Sprite &bg, const sf::Sprite &homeBtn, const sf::Font &font)
    : backgroundSprite(bg), btnHomeSprite(homeBtn) {

    // --- [1] Text hướng dẫn điều khiển ---
    helpText = std::make_unique<sf::Text>(
        createText(font, L"Hướng dẫn:\n- Dùng mũi tên để di chuyển\n- Space để nhảy",
                   28, sf::Color::White, 500.0f, 200.0f)
    );

    // --- [2] Thông tin tác giả ---
    aboutText = std::make_unique<sf::Text>(
        createText(font, L"Tác giả: Code 4T", 24, sf::Color(200, 200, 200), 500.0f, 550.0f)
    );
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
        createText(font, L"Tùy chỉnh (đang phát triển)", 28, sf::Color::White, 500.0f, 250.0f)
    );
}

/* --- Render SettingsUI ---
 * Vẽ nền, nút home và thông tin placeholder.
 */
void SettingsUI::Render(sf::RenderWindow &window, const sf::Font &font) {
    window.draw(backgroundSprite);
    window.draw(btnHomeSprite);
    window.draw(*settingsText);
}
