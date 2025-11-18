#ifndef STATE_UI_H
#define STATE_UI_H

#include "GamePlay/UI/Scores.h"
#include "GamePlay/UI/UI.h"
#include "Utils/Constants.h"
#include "Utils/GameSate.h"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <memory>
#include <vector>

/* --- INTERFACE VẼ TRẠNG THÁI --- */
class IGameStateUI {
  public:
    virtual ~IGameStateUI() = default;
    virtual void Render(sf::RenderWindow &window, const sf::Font &font) = 0;
};

/* --- MENU CHÍNH --- */
class MainMenuUI : public IGameStateUI {
  private:
    // Textures
    sf::Texture btnNewTexture, btnHighScoresTexture, btnSettingsTexture, btnHelpTexture;
    sf::Texture btnExtraTexture;
    sf::Texture myNewImageTexture;
    sf::Texture logoTexture;
    // Sprites (dùng con trỏ để không cần constructor mặc định)
    std::unique_ptr<sf::Sprite> btnNewSprite;
    std::unique_ptr<sf::Sprite> btnHighScoresSprite;
    std::unique_ptr<sf::Sprite> btnSettingsSprite;
    std::unique_ptr<sf::Sprite> btnHelpSprite;
    std::unique_ptr<sf::Sprite> btnExtraSprite;
    std::unique_ptr<sf::Sprite> myNewImageSprite;
    std::unique_ptr<sf::Sprite> logoSprite;
    std::unique_ptr<sf::Text> infoText;
    std::unique_ptr<sf::Text> versionText;

    const sf::Sprite &backgroundSprite;
    const sf::Sprite &sunSprite;
    const sf::Sprite &treeSprite;

  public:
    MainMenuUI(const sf::Sprite &bg, const sf::Sprite &sun, const sf::Sprite &tree, const sf::Font &font);
    void Render(sf::RenderWindow &window, const sf::Font &font) override;

    // Getters (trả về tham chiếu để dùng cho click detection)
    const sf::Sprite &getBtnNewSprite() const {
        return *btnNewSprite;
    }
    const sf::Sprite &getBtnHighScoresSprite() const {
        return *btnHighScoresSprite;
    }
    const sf::Sprite &getBtnSettingsSprite() const {
        return *btnSettingsSprite;
    }
    const sf::Sprite &getBtnHelpSprite() const {
        return *btnHelpSprite;
    }
    const sf::Sprite &getBtnExtraSprite() const {
        return *btnExtraSprite;
    }
};

/* --- HIGH SCORES --- */
class HighScoresUI : public IGameStateUI {
  private:
    sf::Texture tableListTexture, btnNoneTexture1, btnNoneTexture2, btnNoneTexture3, btnNoneTexture4,
        btnTextInputTexture, notFoundSpriteTexture;
    std::unique_ptr<sf::Sprite> btnTt;
    std::unique_ptr<sf::Text> titleText;
    std::unique_ptr<sf::Text> decreaingScore;
    std::unique_ptr<sf::Text> increaingScore;
    std::unique_ptr<sf::Text> decreaingTime;
    std::unique_ptr<sf::Text> increaingTime;
    std::unique_ptr<sf::Text> searchNone;
    std::unique_ptr<sf::Text> searchNone2;
    std::unique_ptr<sf::Text> scrollText;
    const sf::Sprite &backgroundSprite;
    const sf::Sprite &btnHomeSprite;
    std::unique_ptr<sf::Sprite> tableListSprite;
    std::unique_ptr<sf::Sprite> btnNoneSprite1;
    std::unique_ptr<sf::Sprite> btnNoneSprite2;
    std::unique_ptr<sf::Sprite> btnNoneSprite3;
    std::unique_ptr<sf::Sprite> btnNoneSprite4;
    std::unique_ptr<sf::Sprite> btnTextInputSprite;
    std::unique_ptr<sf::Sprite> notFoundSprite;
    sf::Texture menuBgTexture;
    std::unique_ptr<sf::Sprite> menuBgSprite;

    // TEST NHẬP LIỆU
    std::unique_ptr<sf::Text> inputTextDisplay; // Text để hiển thị nội dung nhập
    std::string currentInput;                   // Chuỗi lưu trữ nội dung nhập liệu hiện tại
    bool isInputActive = false;                 // Cờ cho biết có đang nhập liệu hay không

    // --- THÊM: Biến cho con trỏ nhấp nháy ---
    sf::Clock cursorBlinkClock;     // Clock để tính thời gian nhấp nháy
    sf::RectangleShape cursorShape; // Hình chữ "|" (dùng RectangleShape thay vì Text để dễ control)
    bool cursorVisible = true;      // Trạng thái hiện/ẩn con trỏ
    float blinkInterval = 0.5f;     // Nhấp nháy mỗi 0.5 giây (có thể điều chỉnh)
    // -- -THÊM : Prompt riêng biệt-- -
    std::unique_ptr<sf::Text> promptText;       // Text riêng cho prompt ("Tìm theo điểm"), không lẫn với input
    std::string promptString = "Tìm theo điểm"; // Prompt mặc định (bạn có thể đổi thành "Nhập điểm")

    bool isNotFoundVisible = false;
    int scrollIndex = 0;     // Vị trí bắt đầu vẽ (dòng thứ mấy)
    const int MAX_LINES = 5; // Số dòng tối đa hiển thị 1 lúc

  public:
    List scoresList;
    HighScoresUI(const sf::Sprite &bg, const sf::Sprite &homeBtn, const sf::Font &font);
    void Render(sf::RenderWindow &window, const sf::Font &font) override;
    virtual ~HighScoresUI();

    void scrollUp();
    void scrollDown();
    void resetScroll(); // Để reset về 0 khi tìm kiếm hoặc sắp xếp

    // >> THÊM PUBLIC SETTER / GETTER ĐỂ TRUY CẬP PRIVATE FIELDS <<
    void setInputActive(bool active);

    bool isCurrentlyInputting() const {
        return isInputActive;
    }
    void addCharToInput(std::uint32_t unicode);
    void handleInputFinished(List &l);

    bool getIsNotFoundVisible() const {
        return isNotFoundVisible;
    }
    void setIsNotFoundVisible(bool visible) {
        isNotFoundVisible = visible;
    }

    // Thêm các hàm để reset/cập nhật hiển thị từ GameManager
    void resetInputState(); // Hàm mới: Reset chuỗi nhập và hiển thị

    const sf::Sprite &getBtnDecreasingScore() const {
        return *btnNoneSprite1;
    }
    const sf::Sprite &getBtnIncreasingScore() const {
        return *btnNoneSprite2;
    }
    const sf::Sprite &getBtnDecreasingTime() const {
        return *btnNoneSprite3;
    }
    const sf::Sprite &getBtnIncreasingTime() const {
        return *btnNoneSprite4;
    }
    const sf::Sprite &getBtnTextInputSprite() const {
        return *btnTextInputSprite;
    }
    const sf::Sprite &getNotFoundSprite() const {
        return *notFoundSprite;
    }
};

/* --- HELP --- */
class HelpUI : public IGameStateUI {
  private:
    sf::Texture menuBgTexture;
    std::unique_ptr<sf::Sprite> menuBgSprite;
    std::unique_ptr<sf::Text> helpText;
    std::unique_ptr<sf::Text> aboutText;
    const sf::Sprite &backgroundSprite;
    const sf::Sprite &btnHomeSprite;

  public:
    HelpUI(const sf::Sprite &bg, const sf::Sprite &homeBtn, const sf::Font &font);
    void Render(sf::RenderWindow &window, const sf::Font &font) override;
};

/* --- SETTINGS --- */
class SettingsUI : public IGameStateUI {
  private:
    sf::Texture menuBgTexture;
    std::unique_ptr<sf::Sprite> menuBgSprite;
    std::unique_ptr<sf::Text> settingsText;
    const sf::Sprite &backgroundSprite;
    const sf::Sprite &btnHomeSprite;

    std::unique_ptr<sf::Text> AudioSettingText;
    std::unique_ptr<sf::Text> MusicSettingText;

    sf::Texture mutedTexture, unmutedTexture, btnNone, musicMutedTexture, musicUnmutedTexture;
    std::unique_ptr<sf::Sprite> mutedSprite;
    std::unique_ptr<sf::Sprite> unmutedSprite;
    std::unique_ptr<sf::Sprite> btnNoneSprite;
    std::unique_ptr<sf::Sprite> musicMutedSprite;
    std::unique_ptr<sf::Sprite> musicUnmutedSprite;

  public:
    SettingsUI(const sf::Sprite &bg, const sf::Sprite &homeBtn, const sf::Font &font);
    void Render(sf::RenderWindow &window, const sf::Font &font) override;
    const sf::Sprite &getMutedSprite() const {
        return *mutedSprite;
    }
    const sf::Sprite &getHomeButtonSprite() const {
        return btnHomeSprite;
    }
    const sf::Sprite &getMusicMutedSprite() const {
        return *musicMutedSprite;
    }
};

class GameOverUI : public IGameStateUI {
  private:
    const sf::Sprite &backgroundSprite;
    sf::Texture btnNone, gameOverTexture, btnUndoTexture, btnHomeTexture;
    std::unique_ptr<sf::Sprite> btnNoneSprite;

    std::unique_ptr<sf::Sprite> btnHomeSprite;
    std::unique_ptr<sf::Sprite> btnUndoSprite;




    std::unique_ptr<sf::Text> gameOverText;

  public:
    GameOverUI(const sf::Sprite &bg, const sf::Font &font);
    void Render(sf::RenderWindow &window, const sf::Font &font) override;
    const sf::Sprite &getHomeButtonSprite() const {
        return *btnHomeSprite;
    }
    const sf::Sprite &getUndoButtonSprite() const {
        return *btnUndoSprite;
    }
};

void drawScoresList(sf::RenderWindow &window, const List &l, const sf::Font &font, float startX, float startY,
                    int startIndex);

#endif // STATE_UI_H