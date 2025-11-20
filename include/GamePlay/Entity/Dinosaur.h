#ifndef DINOSAUR_H
#define DINOSAUR_H
#include "Utils/Entity.h" // Bắt buộc phải include class cha

struct Fireball {
    std::unique_ptr<Animation> animation;
    sf::Vector2f velocity;
    bool isDestroyed = false;
};

class Dinosaur : public Entity {
private:
    // Thêm các thuộc tính riêng của Khủng long nếu cần (ví dụ: tầm nhìn, sát thương cơ bản)
    float attackRange; 
    sf::RectangleShape healthBarBg;      // Thanh nền (Màu đỏ)
    sf::RectangleShape healthBarCurrent; // Thanh máu hiện tại (Màu xanh)

    sf::Texture fireballTexture;     // Texture hình cầu lửa
    std::vector<Fireball> fireballs; // Danh sách các cầu lửa đang bay
    float shootCooldown;             // Thời gian hồi chiêu (giây)
    float timeSinceLastShot;         // Thời gian đã trôi qua từ phát bắn cuối

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
    
    void UpdateAttack(float deltaTime, sf::Vector2f playerPos);
    // Hàm lấy danh sách đạn để GameLoop kiểm tra va chạm
    std::vector<Fireball> &GetFireballs() {
        return fireballs;
    }

    // Ghi đè hàm hiển thị trạng thái nếu cần
    void DisplayStatus() const override;

    
};

#endif // DINOSAUR_H