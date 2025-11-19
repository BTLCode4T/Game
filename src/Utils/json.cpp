
#include "../../include/Core/GameLoop/json.h"
#include "GamePlay/Avatar/player.h"
#include "GamePlay/Gun/bullet.h"
#include <fstream>
#include <iostream>
#include <cstdio> // Thư viện cho std::remove

using json = nlohmann::json;
using namespace std;

// Tên file và key gốc (đặt trong file cpp để ẩn chi tiết)
const string SAVE_FILE = "data.json";
const string ROOT_KEY = "DinoSurvivalData";

// ============================================================
// 1. LƯU GAME
// ============================================================
// [QUAN TRỌNG]: Tham số bullets phải là vector<unique_ptr<...>>
void SaveGame(const PlayerManager& player, const std::vector<std::unique_ptr<Bullet>>& bullets, int score) {
    if (!player.IsAlive()) return;

    json root;
    root["Player"] = player.SaveState();
    root["GameData"]["score"] = score;

    json bullet_arr = json::array();
    for (const auto& b : bullets) {
        // [QUAN TRỌNG]: Dùng mũi tên (->) vì b là con trỏ
        if (b && !b->IsDestroyed() && !b->IsExpired()) {
            bullet_arr.push_back(b->SaveState());
        }
    }
    root["Bullets"] = bullet_arr;

    json final_save;
    final_save[ROOT_KEY] = root;

    ofstream o(SAVE_FILE);
    if (o.is_open()) {
        o << final_save.dump(4);
        cout << "[System] Game Saved. Score: " << score << "\n";
    }
}

// ============================================================
// 2. TẢI GAME
// ============================================================
// [QUAN TRỌNG]: Tham số bullets phải là vector<unique_ptr<...>>
bool LoadGame(PlayerManager& player, std::vector<std::unique_ptr<Bullet>>& bullets, int& score) {
    ifstream f(SAVE_FILE);
    if (!f.is_open()) return false;

    json raw_data;
    try {
        f >> raw_data;
    } catch (...) {
        return false;
    }

    if (!raw_data.contains(ROOT_KEY)) return false;

    json data = raw_data[ROOT_KEY];

    if (data.contains("Player")) {
        player.LoadState(data["Player"]);
    }

    if (data.contains("GameData") && data["GameData"].contains("score")) {
        score = data["GameData"]["score"].get<int>();
    } else {
        score = 0;
    }

    bullets.clear();
    if (data.contains("Bullets")) {
        for (auto& b_json : data["Bullets"]) {
            // Tạo con trỏ unique_ptr mới
            auto bullet = std::make_unique<Bullet>("assets/Images/bullet/image6.png", 0, 0, 30.f, 40.f, 1, sf::Vector2f(0,0), 0);
            bullet->LoadState(b_json);
            bullets.push_back(std::move(bullet));
        }
    }
    
    cout << "[System] Game Loaded Successfully.\n";
    return true;
}

// ============================================================
// 3. CÁC HÀM KHÁC
// ============================================================
bool IsSaveFileValid() {
    ifstream f(SAVE_FILE);
    if (!f.is_open()) return false;

    json raw_data;
    try {
        f >> raw_data;
    } catch (...) {
        return false;
    }

    if (!raw_data.contains(ROOT_KEY)) return false;
    
    json data = raw_data[ROOT_KEY];
    
    // 1. Kiểm tra nhân vật còn sống không
    bool isAlive = false;
    if (data.contains("Player") && data["Player"].value("is_alive", false)) {
        isAlive = true;
    }

    // 2. [MỚI] Kiểm tra xem đã chơi được tí nào chưa (Điểm > 0)
    bool hasScore = false;
    if (data.contains("GameData") && data["GameData"].contains("score")) {
        int score = data["GameData"]["score"].get<int>();
        if (score > 0) { 
            hasScore = true; 
        }
    }

    // CHỈ HIỆN NÚT CONTINUE NẾU: Còn sống VÀ Có điểm
    return isAlive && hasScore;
}

void DeleteSaveFile() {
    if (std::remove(SAVE_FILE.c_str()) == 0) {
        cout << "[System] Save file deleted.\n";
    }
}