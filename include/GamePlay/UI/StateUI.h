#ifndef STATE_UI_H
#define STATE_UI_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "GamePlay/UI/UI.h"
#include "Core/Constants.h"
#include "Core/GameLoop/game.h"
#include "GamePlay/UI/Scores.h"


enum class GameState;

/* --- INTERFACE VẼ TRẠNG THÁI --- */
class IGameStateUI {
public:
    virtual ~IGameStateUI() = default;
    virtual void Render(sf::RenderWindow& window, const sf::Font& font) = 0;
};

/* --- MENU CHÍNH --- */
class MainMenuUI : public IGameStateUI {
private:
    // Textures
    sf::Texture btnNewTexture, btnHighScoresTexture, btnSettingsTexture, btnHelpTexture;

    // Sprites (dùng con trỏ để không cần constructor mặc định)
    std::unique_ptr<sf::Sprite> btnNewSprite;
    std::unique_ptr<sf::Sprite> btnHighScoresSprite;
    std::unique_ptr<sf::Sprite> btnSettingsSprite;
    std::unique_ptr<sf::Sprite> btnHelpSprite;

    std::unique_ptr<sf::Text> infoText;
    std::unique_ptr<sf::Text> versionText;

    const sf::Sprite& backgroundSprite;
    const sf::Sprite& sunSprite;
    const sf::Sprite& treeSprite;

public:
    MainMenuUI(const sf::Sprite& bg, const sf::Sprite& sun, const sf::Sprite& tree, const sf::Font& font);
    void Render(sf::RenderWindow& window, const sf::Font& font) override;

    // Getters (trả về tham chiếu để dùng cho click detection)
    const sf::Sprite& getBtnNewSprite() const { return *btnNewSprite; }
    const sf::Sprite& getBtnHighScoresSprite() const { return *btnHighScoresSprite; }
    const sf::Sprite& getBtnSettingsSprite() const { return *btnSettingsSprite; }
    const sf::Sprite& getBtnHelpSprite() const { return *btnHelpSprite; }
};

/* --- HIGH SCORES --- */
class HighScoresUI : public IGameStateUI {
private:
    sf::Texture tableListTexture, btnNoneTexture1, btnNoneTexture2, btnNoneTexture3, btnNoneTexture4;
    std::unique_ptr<sf::Text> titleText;
    std::unique_ptr<sf::Text> decreaingScore;
    std::unique_ptr<sf::Text> increaingScore;
    std::unique_ptr<sf::Text> decreaingTime;
    std::unique_ptr<sf::Text> increaingTime;
    const sf::Sprite& backgroundSprite;
    const sf::Sprite& btnHomeSprite;
    std::unique_ptr<sf::Sprite> tableListSprite;
    std::unique_ptr<sf::Sprite> btnNoneSprite1;
    std::unique_ptr<sf::Sprite> btnNoneSprite2;
    std::unique_ptr<sf::Sprite> btnNoneSprite3;
    std::unique_ptr<sf::Sprite> btnNoneSprite4;

   
public: 
    List scoresList;
    HighScoresUI(const sf::Sprite& bg, const sf::Sprite& homeBtn, const sf::Font& font);
    void Render(sf::RenderWindow& window, const sf::Font& font) override;
    virtual ~HighScoresUI();
    const sf::Sprite& getBtnDecreasingScore() const { return *btnNoneSprite1; }
    const sf::Sprite& getBtnIncreasingScore() const { return *btnNoneSprite2; }
    const sf::Sprite& getBtnDecreasingTime() const { return *btnNoneSprite3; }
    const sf::Sprite& getBtnIncreasingTime() const { return *btnNoneSprite4; }
};

/* --- HELP --- */
class HelpUI : public IGameStateUI {
private:
    std::unique_ptr<sf::Text> helpText;
    std::unique_ptr<sf::Text> aboutText;
    const sf::Sprite& backgroundSprite;
    const sf::Sprite& btnHomeSprite;

public:
    HelpUI(const sf::Sprite& bg, const sf::Sprite& homeBtn, const sf::Font& font);
    void Render(sf::RenderWindow& window, const sf::Font& font) override;
};

/* --- SETTINGS --- */
class SettingsUI : public IGameStateUI {
private:
    std::unique_ptr<sf::Text> settingsText;
    const sf::Sprite& backgroundSprite;
    const sf::Sprite& btnHomeSprite;

public:
    SettingsUI(const sf::Sprite& bg, const sf::Sprite& homeBtn, const sf::Font& font);
    void Render(sf::RenderWindow& window, const sf::Font& font) override;
};


void drawScoresList(sf::RenderWindow &window, const List &l, const sf::Font &font, float startX, float startY);




#endif
