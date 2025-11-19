// File: ../../include/Core/GameLoop/json.h

#ifndef JSON_MANAGER_H
#define JSON_MANAGER_H

#include "json.hpp"
#include <vector>

// Forward declaration
class PlayerManager;
class Bullet;

// Hàm lưu game (Player + Đạn)
void SaveGame(const PlayerManager& player, const std::vector<std::unique_ptr<Bullet>>& bullets, int score);

// Hàm tải game (Trả về dữ liệu JSON để main xử lý)
bool LoadGame(PlayerManager& player, std::vector<std::unique_ptr<Bullet>>& bullets, int& score);

// Hàm xóa file save (Dùng khi chết)
void DeleteSaveFile();

// 3. Hàm kiểm tra nhanh xem có file save hợp lệ không (Dùng cho Menu UI)
bool IsSaveFileValid();
#endif // JSON_MANAGER_H