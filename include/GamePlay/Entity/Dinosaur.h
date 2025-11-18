#ifndef DINOSAUR_H
#define DINOSAUR_H
#include "Utils/Entity.h" // Bắt buộc phải include class cha

class Dinosaur : public Entity {
private:
    // Thêm các thuộc tính riêng của Khủng long nếu cần (ví dụ: tầm nhìn, sát thương cơ bản)
    float attackRange; 
public:
    /**
     * @brief Constructor cho Khủng long.
     * * @param name Tên.
     * @param x Tọa độ X ban đầu.
     * @param y Tọa độ Y ban đầu.
     * @param maxHealth Máu tối đa.
     * @param speed Tốc độ di chuyển.
     * @param texturePath Đường dẫn ảnh.
     */
  Dinosaur(const string &name, float x, float y, int maxHealth, float speed, const string &texturePath, float width,
           float height, sf::Vector2i frameNum, float frameTime);

    virtual ~Dinosaur() = default;

    // Hàm riêng cho Khủng long (ví dụ: AI di chuyển, tấn công)
    void ChasePlayer(float playerX, float playerY);
    
    // Ghi đè hàm hiển thị trạng thái nếu cần
    void DisplayStatus() const override;
};

#endif // DINOSAUR_H