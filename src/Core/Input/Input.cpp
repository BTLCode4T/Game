#include "Core/Input/Input.h"


// hàm mẫu để sử dụng cho các thao tác


InputManager::InputManager() {
}

//=============================================================================================================================
// Xử lý khi có sự kiện SFML gửi đến
void InputManager::ProcessEvent(const sf::Event &event) {
    // phím
    if (const auto *key = event.getIf<sf::Event::KeyPressed>()) {
        if (!keyDown[key->scancode])          // Nếu phím chưa được giữ xuống
            keyPressed[key->scancode] = true; // Ghi nhận là vừa nhấn
        keyDown[key->scancode] = true;        // Đánh dấu phím đang được giữ
    }
    // thả phím
    if (const auto *key = event.getIf<sf::Event::KeyReleased>()) {
        keyDown[key->scancode] = false;
        keyReleased[key->scancode] = true;
    }

    // nhấn chuột
    if (const auto *mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (!mouseDown[mouse->button])
            mousePressed[mouse->button] = true;
        mouseDown[mouse->button] = true;
    }
    // thả chuột
    if (const auto *mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
        mouseDown[mouse->button] = false;
        mouseReleased[mouse->button] = true;
    }

    // Khi chuột di chuyển, lưu tọa độ hiện tại
    if (const auto *move = event.getIf<sf::Event::MouseMoved>()) {
        mousePos = {move->position.x, move->position.y};
    }
}
//=============================================================================================================================


// ==================================================== Cập nhật frame ========================================================

void InputManager::Update() {
    // Cờ “vừa nhấn” và “vừa nhả” chỉ tồn tại đúng 1 frame duy nhất,
    //     nên phải reset sau mỗi vòng lặp update() hoặc render() — để game chỉ xử lý sự kiện mới xảy ra,
    //     không bị lặp lại từ frame trước.keyPressed.clear();
    keyPressed.clear();
    keyReleased.clear();
    mousePressed.clear();
    mouseReleased.clear();
}
//=============================================================================================================================


////================================================= Kiểm tra ===========================================================
// Dùng trong vòng lặp update()

// phím
bool InputManager::IsKeyDown(sf::Keyboard::Scancode key) const {
    // Trả về true nếu phím đang được giữ
    auto it = keyDown.find(key);
    return it != keyDown.end() && it->second;
}

bool InputManager::IsKeyPressed(sf::Keyboard::Scancode key) const {
    // Trả về true nếu phím vừa được nhấn ở frame này
    auto it = keyPressed.find(key);
    return it != keyPressed.end() && it->second;
}

bool InputManager::IsKeyReleased(sf::Keyboard::Scancode key) const {
    // Trả về true nếu phím vừa được nhả ở frame này
    auto it = keyReleased.find(key);
    return it != keyReleased.end() && it->second;
}

// chuột
bool InputManager::IsMouseDown(sf::Mouse::Button btn) const {
    auto it = mouseDown.find(btn);
    return it != mouseDown.end() && it->second;
}

bool InputManager::IsMousePressed(sf::Mouse::Button btn) const {
    auto it = mousePressed.find(btn);
    return it != mousePressed.end() && it->second;
}

bool InputManager::IsMouseReleased(sf::Mouse::Button btn) const {
    auto it = mouseReleased.find(btn);
    return it != mouseReleased.end() && it->second;
}
//=============================================================================================================================



//=================================================== Lấy vị trí chuột =========================================================

sf::Vector2i InputManager::GetMousePosition() const {
    return mousePos;
}

//=============================================================================================================================
