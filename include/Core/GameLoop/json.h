// json_manager.h

#ifndef JSON_MANAGER_H
#define JSON_MANAGER_H

#include <string>
#include "../../src/Utils/json.hpp" // Đảm bảo thư viện nlohmann/json có sẵn

using namespace std;
using json = nlohmann::json;

// ================================
// CẤU HÌNH CHUNG
// ================================

// Khóa mặc định cho các hàm quản lý data structure
const string ROOT_KEY = "container"; 

// ------------------------------------------
// --- KHAI BÁO HÀM (Prototypes) ---
// ------------------------------------------

// HÀM CƠ BẢN FILE VÀ LƯU SAVE GAME
json LoadJSON(const string& filename);
void SaveJSON(const json& data, const string& filename);
void SaveFullData(const json& full_data, const string& filename); // Lưu toàn bộ JSON (Không dùng ROOT_KEY)

// HÀM THAO TÁC CẤU TRÚC
void CreateBranch(json& data, const string& parent_name, const string& new_branch);
void AddOrUpdateObject(json& data, const string& parent_branch, 
                       const string& object_name, const json& properties);
void DisplayJSON(const json& data, const string& branch_name);

#endif // JSON_MANAGER_H