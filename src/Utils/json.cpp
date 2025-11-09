#include "Core/GameLoop/json.h" 
#include <iostream>
#include <fstream>
#include <exception>
#include <map> // Bắt buộc cho kiểu map

using namespace std; 
using json = nlohmann::json;

// Định nghĩa dữ liệu mặc định (Fallback Data)
const json DEFAULT_GAME_DATA = {
    {"items", {
        {"SWORD", {{"name", "Kiếm Sắt"}, {"damage", 25}}},
        {"POTION", {{"name", "Bình Hồi Máu"}, {"heal", 50}}}
    }},
    {"skills", {
        {"FIREBALL", {{"name", "Fireball"}, {"damage", 50}, {"cooldown", 5}}},
        {"HEAL", {{"name", "Heal"}, {"heal", 30}, {"cooldown", 8}}}
    }},
    {"player_data", {{"level", 1}, {"gold", 0}, {"current_location", "Start"}}}
};


// =========================================================================
// HÀM GHI DỮ LIỆU (LƯU TRẠNG THÁI GAME)
// =========================================================================
void JsonProcessor::updateAndWriteData(const string& filename) {
    // Luôn cố gắng đọc dữ liệu hiện tại để cập nhật
    ifstream input_file(filename);
    json data;
    bool is_default = false;

    try {
        if (input_file.is_open()) {
            input_file >> data; // Đọc nội dung file
            input_file.close(); 
            cout << "[SAVE] Doc du lieu hien tai de cap nhat." << endl;
        } else {
            // Nếu không mở được (file chưa có), sử dụng dữ liệu mặc định
            data = DEFAULT_GAME_DATA;
            is_default = true;
            cerr << "[SAVE] File khong ton tai. Su dung du lieu mac dinh de ghi." << endl;
        }
    } catch (const json::parse_error& e) {
        cerr << "[SAVE] Loi phan tich JSON: " << e.what() << ". Su dung du lieu mac dinh de ghi." << endl;
        data = DEFAULT_GAME_DATA;
        is_default = true;
    }
    
    // Cập nhật trạng thái người chơi (Đây là logic lưu game)
    cout << "[SAVE] Dang cap nhat trang thai game..." << endl;
    
    data["player_data"] = {
        {"level", 5}, // Ví dụ: Cập nhật level hiện tại
        {"gold", 1500},
        {"current_location", "Village"}
    };
    
    // Ví dụ cập nhật vật phẩm
    if (data["items"].contains("SWORD")) {
        data["items"]["SWORD"]["damage"] = 40; 
    }

    // Ghi đè dữ liệu đã cập nhật vào file
    ofstream output_file(filename);

    if (!output_file.is_open()) {
        cerr << "[SAVE] Loi: Khong the mo file " << filename << " de ghi de." << endl;
        return;
    }

    try {
        output_file << data.dump(4) << endl;
        cout << "[SAVE] Da LUU trang thai moi thanh cong vao file: " << filename << endl;

    } catch (const exception& e) {
        cerr << "[SAVE] Loi khi ghi file JSON: " << e.what() << endl;
    }
}

// =========================================================================
// HÀM ĐỌC DỮ LIỆU (TẢI TRẠNG THÁI GAME)
// =========================================================================
void JsonProcessor::readAndPrintData(const string& filename) {
    ifstream file(filename);
    json data;

    // --- LOGIC FALLBACK (KHẮC PHỤC LỖI ĐỌC FILE TRỐNG/LỖI) ---
    if (!file.is_open() || file.peek() == ifstream::traits_type::eof()) {
        cerr << "[LOAD] Loi: File khong ton tai hoac bi trong. Tai du lieu mac dinh." << endl;
        data = DEFAULT_GAME_DATA;
        goto process_data;
    }

    try {
        file >> data; 
    } catch (const json::parse_error& e) {
        cerr << "[LOAD] Loi phan tich JSON: " << e.what() << ". Tai du lieu mac dinh." << endl;
        data = DEFAULT_GAME_DATA;
    }

    // --- Bắt đầu xử lý dữ liệu ---
    process_data:; 
    
    // In toàn bộ dữ liệu ra console để kiểm tra
    cout << "\n========================================" << endl;
    cout << "Dang TAI du lieu nguoi choi tu file: " << filename << endl;
    
    // ... (Phần code in dữ liệu của bạn, sử dụng đối tượng 'data') ...
    
    cout << "========================================" << endl;
    // (Ở đây, bạn sẽ lưu đối tượng 'data' vào một biến thành viên hoặc toàn cục
    // để các phần khác của game có thể truy cập được.)
}