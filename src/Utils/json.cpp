// File: json.cpp

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

// --------------------------------------------------------
// 1. LƯU GAME
// --------------------------------------------------------
void SaveGame(const PlayerManager& player, const std::vector<std::unique_ptr<Bullet>>& bullets) {
    
    if (!player.IsAlive()) return;

    json root;
    root["Player"] = player.SaveState();

    json bullet_arr = json::array();
    
    // Duyệt qua danh sách con trỏ
    for (const auto& b : bullets) {
        // SỬA DÒNG NÀY: Dùng dấu mũi tên (->) thay vì dấu chấm (.)
        if (!b->IsDestroyed() && !b->IsExpired()) {
            bullet_arr.push_back(b->SaveState());
        }
    }
    root["Bullets"] = bullet_arr;

    json final_save;
    final_save[ROOT_KEY] = root;

    ofstream o(SAVE_FILE);
    if (o.is_open()) {
        o << final_save.dump(4);
        cout << "[System] Game Saved.\n";
    }
}

// --------------------------------------------------------
// 2. TẢI GAME
// --------------------------------------------------------
json LoadGame() {
    ifstream f(SAVE_FILE);
    if (!f.is_open()) {
        cout << "[System] No save file found. New Game.\n";
        return json::object(); 
    }

    json raw_data;
    try {
        f >> raw_data;
    } catch (...) {
        cerr << "[Error] Save file corrupted. New Game.\n";
        return json::object();
    }

    if (raw_data.contains(ROOT_KEY)) {
        cout << "[System] Save file loaded.\n";
        return raw_data[ROOT_KEY];
    }
    
    return json::object();
}

// --------------------------------------------------------
// 3. XÓA FILE (KHI CHẾT)
// --------------------------------------------------------
void DeleteSaveFile() {
    if (std::remove(SAVE_FILE.c_str()) == 0) {
        cout << "[System] You Died. Save file deleted.\n";
    }
}