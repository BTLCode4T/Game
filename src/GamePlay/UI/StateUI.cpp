#include "GamePlay/UI/StateUI.h"
#include "GamePlay/UI/Scores.h"
#include "GamePlay/UI/UI.h"
#include <SFML/Graphics.hpp>
#include <cstdint> // đảm bảo có
#include <iomanip>
#include <iostream>
#include <sstream>
#include "Core/Audio/Audio.h"
#include "Core/Audio/MusicManager.h"
#include "Core/GameLoop/game.h"

/* ============================================================
 * CLASS: MainMenuUI — Giao diện chính của game
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

    // --- [6] Mở nhạc nền

    MusicManager::Get().Play("menu");

    
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
 * CLASS: HighScoresUI — Màn hình bảng xếp hạng
 * ============================================================ */
HighScoresUI::HighScoresUI(const sf::Sprite &bg, const sf::Sprite &homeBtn, const sf::Font &font)
    : backgroundSprite(bg), btnHomeSprite(homeBtn) {     
    // --- [1] Tiêu đề ---
    titleText = std::make_unique<sf::Text>(createText(font, L"BẢNG XẾP HẠNG", 40, sf::Color::Red, 350.0f, 125.0f));

    // --- [2] Dòng hiển thị điểm số người chơi ---

    tableListSprite = std::make_unique<sf::Sprite>(
        createSprite(tableListTexture, "assets/Images/list.png", 670.5f, 502.5f, 25.0f, 60.0f));

    btnNoneSprite1 = std::make_unique<sf::Sprite>(
        createSprite(btnNoneTexture1, "assets/Images/none.png", 225.0f, 55.5f, 750.0f, 125.0f));

    btnNoneSprite2 = std::make_unique<sf::Sprite>(
        createSprite(btnNoneTexture2, "assets/Images/none.png", 225.0f, 55.5f, 750.0f, 225.0f));

    btnNoneSprite3 = std::make_unique<sf::Sprite>(
        createSprite(btnNoneTexture3, "assets/Images/none.png", 225.0f, 55.5f, 750.0f, 325.0f));

    btnNoneSprite4 = std::make_unique<sf::Sprite>(
        createSprite(btnNoneTexture4, "assets/Images/none.png", 225.0f, 55.5f, 750.0f, 425.0f));

    btnTextInputSprite = std::make_unique<sf::Sprite>(
        createSprite(btnTextInputTexture, "assets/Images/TextInput.png", 225.0f, 55.5f, 750.0f, 525.0f));

    notFoundSprite = std::make_unique<sf::Sprite>(
        createSprite(notFoundSpriteTexture, "assets/Images/btnone.png", 420.0f, 333.0f, 250.0f, 150.0f));

    // Khu vực tạo text sắp xếp
    // std::unique_ptr<sf::Text> decreaingScore;
    // std::unique_ptr<sf::Text> increaingScore;
    // std::unique_ptr<sf::Text> decreaingTime;
    // std::unique_ptr<sf::Text> decreaingTime;

    // --- THAY THẾ: Khởi tạo prompt và input riêng biệt ---
    promptText =
        std::make_unique<sf::Text>(createText(font, L"Tìm theo điểm", 22, sf::Color(128, 128, 128), 862.0f, 550.0f, true));
    inputTextDisplay =
        std::make_unique<sf::Text>(font, "", 22); // SỬA: Sử dụng constructor có tham số đúng thứ tự: font, string, size
    inputTextDisplay->setFillColor(sf::Color::Red); 
    inputTextDisplay->setPosition(sf::Vector2f(840.0f, 540.5f)); // SỬA: Sử dụng Vector2f
    inputTextDisplay->setStyle(sf::Text::Style::Bold);            // Giữ bold nếu có (tùy chọn)

    // --- THÊM: Khởi tạo con trỏ "|" ---
    cursorShape.setSize(sf::Vector2f(2.0f, 22.0f)); // Kích thước "|" (rộng 2px, cao bằng font size)
    cursorShape.setFillColor(sf::Color::Black);     // Màu đen
    cursorShape.setPosition(sf::Vector2f(
        862.5f + 100.0f, 552.75f)); // SỬA: Sử dụng Vector2f Vị trí sau prompt/input (điều chỉnh offset nếu cần)

    // currentInput giữ nguyên rỗng ban đầu
    currentInput = "";

    decreaingScore =
        std::make_unique<sf::Text>(createText(font, L"Điểm giảm dần", 20, sf::Color::White, 855.0f, 150.0f));

    increaingScore =
        std::make_unique<sf::Text>(createText(font, L"Điểm tăng dần", 20, sf::Color::White, 855.0f, 250.0f));

    decreaingTime =
        std::make_unique<sf::Text>(createText(font, L"Thời gian giảm dần", 20, sf::Color::White, 855.0f, 350.0f));

    increaingTime =
        std::make_unique<sf::Text>(createText(font, L"Thời gian tăng dần", 20, sf::Color::White, 855.0f, 450.0f));
    
    searchNone =
        std::make_unique<sf::Text>(createText(font, L"Không tìm thấy !", 30, sf::Color::Yellow, 450.0f, 250.0f));
    searchNone2 =
        std::make_unique<sf::Text>(createText(font, L"Ấn vị trí trống bất \n\nkì để tiếp tục !", 25, sf::Color::Blue, 450.0f, 350.0f));
    
        scrollText =
        std::make_unique<sf::Text>(createText(font, L"< Ấn phím mũi tên ^/v để cuộn! >", 15, sf::Color::Yellow, 350.0f, 515.0f));


    // Load scores từ file ngay khi tạo UI
    initList(scoresList);
    readFile("Scores.txt", scoresList);


    // Phát nhạc


}

/* --- Render HighScoresUI ---
 * Vẽ nền, nút home và các text liên quan đến bảng điểm.
 */
void HighScoresUI::Render(sf::RenderWindow &window, const sf::Font &font) {
    // List l; // >> Bỏ dòng này
    // initList(l); // >> Bỏ dòng này
    // readFile("Scores.txt", l); // >> Bỏ dòng này

    window.draw(backgroundSprite);
    window.draw(btnHomeSprite);
    window.draw(*tableListSprite);
    window.draw(*titleText);
    window.draw(*scrollText);

    // *** DÙNG scoresList THÀNH VIÊN ĐÃ TẢI ***
    drawScoresList(window, scoresList, font, 120.0f, 200.0f, scrollIndex);

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
 * CLASS: SettingsUI — Màn hình cài đặt
 * ============================================================ */
SettingsUI::SettingsUI(const sf::Sprite &bg, const sf::Sprite &homeBtn, const sf::Font &font)
    : backgroundSprite(bg), btnHomeSprite(homeBtn) {

    // --- [1] Text thông báo đang phát triển ---
    settingsText = std::make_unique<sf::Text>(
        createText(font, L"Tùy chỉnh (đang phát triển)", 28, sf::Color::White, 500.0f, 250.0f));

    // --- [2] Text Âm thanh

    AudioSettingText = std::make_unique<sf::Text>(
        createText(font, L"Âm thanh", 28, sf::Color::White, 320.0f, 200.0f));

    MusicSettingText = std::make_unique<sf::Text>(
        createText(font, L"Âm thanh nhạc nền", 28, sf::Color::White, 385.0f, 350.0f));


    btnNoneSprite = std::make_unique<sf::Sprite>(
        createSprite(btnNone, "assets/Images/btnone.png", 630.0f, 500.0f, 170.0f, 50.0f));


    mutedSprite = std::make_unique<sf::Sprite>(
        createSprite(mutedTexture, "assets/Images/muted_true.png",  50.0f, 50.0f, 650.0f, 175.0f));
    
    unmutedSprite = std::make_unique<sf::Sprite>(
        createSprite(unmutedTexture, "assets/Images/muted_false.png",  50.0f, 50.0f, 650.0f, 175.0f));


    musicMutedSprite = std::make_unique<sf::Sprite>(
        createSprite(musicMutedTexture, "assets/Images/muted_true.png",  50.0f, 50.0f, 650.0f, 325.0f)); // Vị trí mới
    
    musicUnmutedSprite = std::make_unique<sf::Sprite>(
        createSprite(musicUnmutedTexture, "assets/Images/muted_false.png",  50.0f, 50.0f, 650.0f, 325.0f)); // Vị trí mới

    
}

/* --- Render SettingsUI ---
 * Vẽ nền, nút home và thông tin placeholder.
 */
void SettingsUI::Render(sf::RenderWindow &window, const sf::Font &font) {
    window.draw(backgroundSprite);
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

    btnNoneSprite = std::make_unique<sf::Sprite>(
        createSprite(btnNone, "assets/Images/btnone.png", 630.0f, 500.0f, 170.0f, 50.0f));

    btnHomeSprite = std::make_unique<sf::Sprite>(
        createSprite(btnHomeTexture,"assets/Images/Home.png", 75.0f, 75.0f, 350.0f, 325.0f));

    btnUndoSprite = std::make_unique<sf::Sprite>(
        createSprite(btnUndoTexture, "assets/Images/Undo.png", 75.0f, 75.0f, 550.0f, 325.0f));

    gameOverText = std::make_unique<sf::Text>(
        createText(font, L"Bạn đã thua!\nĐiểm: ******\nThời gian: **/**/*****", 28, sf::Color::White, 500.0f, 250.0f, true));

}

void GameOverUI::Render(sf::RenderWindow &window, const sf::Font &font){
    window.draw(backgroundSprite);
    window.draw(*btnNoneSprite);
    window.draw(*gameOverText);
    window.draw(*btnHomeSprite);
    window.draw(*btnUndoSprite);
}


void drawScoresList(sf::RenderWindow &window, const List &l, const sf::Font &font, float startX, float startY, int startIndex) {
    Node *p = l.head;
    float currentY = startY;

    // --- [1] Vẽ Tiêu đề ---
    sf::Text headerText(font, L"ĐIỂM\t\t\tTHỜI GIAN\t\t\tNGÀY", 22);
    headerText.setFillColor(sf::Color::Yellow);
    headerText.setPosition(sf::Vector2f(135.0f, 175.0f));
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
        cursorShape.setPosition(sf::Vector2f(inputTextDisplay->getPosition().x + textBounds.size.x + 2.0f, inputTextDisplay->getPosition().y));
}

// Xử lý khi nhấn Enter (Giả định là tìm kiếm điểm) - THÊM: Đảm bảo caller pass scoresList
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