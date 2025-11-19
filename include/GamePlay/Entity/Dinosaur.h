#ifndef DINOSAUR_H
#define DINOSAUR_H
#include "Utils/Entity.h" // Bắt buộc phải include class cha

class Dinosaur : public Entity {
private:
    // Thêm các thuộc tính riêng của Khủng long nếu cần (ví dụ: tầm nhìn, sát thương cơ bản)
    float attackRange; 
    sf::RectangleShape healthBarBg;      // Thanh nền (Màu đỏ)
    sf::RectangleShape healthBarCurrent; // Thanh máu hiện tại (Màu xanh)

    // Hàm phụ để cập nhật vị trí và độ dài thanh máu
    void UpdateHealthBar();

  public:
    /**
     * Constructor cho Khủng long.
     * name Tên.
     * x Tọa độ X ban đầu.
     * y Tọa độ Y ban đầu. 
     * maxHealth Máu tối đa.
     * speed Tốc độ di chuyển.
     * texturePath Đường dẫn ảnh.
     */
  Dinosaur(const string &name, float x, float y, int maxHealth, float speed, const string &texturePath, float width,
           float height, sf::Vector2i frameNum, float frameTime);

    virtual ~Dinosaur() = default;
  void Render(sf::RenderWindow &window) override;
    // Hàm riêng cho Khủng long (ví dụ: AI di chuyển, tấn công)
    void ChasePlayer(float playerX, float playerY);
    
    // Ghi đè hàm hiển thị trạng thái nếu cần
    void DisplayStatus() const override;

    
};

#endif // DINOSAUR_H