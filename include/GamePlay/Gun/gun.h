#ifndef GUN_H
#define GUN_H

#include "Utils/Entity.h"
#include <SFML/System/Clock.hpp>
#include <cmath> // Cho atan2

class Gun : public Entity {
  private:
    int currentAmmo;
    int maxAmmo;
    float fireRate; // Thời gian (giây) giữa mỗi phát bắn
    sf::Clock fireTimer; // Đồng hồ để đếm fireRate

    // Điểm đặt súng (so với tâm player), ví dụ (0, 0) là ngay giữa
    sf::Vector2f holdOffset; 

  public:
    Gun(const string &name, const string &texturePath, float width, float height,
        sf::Vector2i frameNum, float frameTime,
        int maxAmmo, float fireRate, sf::Vector2f holdOffset = {0.f, 0.f});

    virtual ~Gun() = default;

    /**
     * @brief Cập nhật vị trí và góc quay của súng.
     * @param playerCenter Vị trí tâm của người chơi.
     * @param mousePos Vị trí chuột (tọa độ thế giới game).
     */
    void UpdateGun(sf::Vector2f playerCenter, sf::Vector2f mousePos);

    /**
     * @brief Thử bắn. Trả về true nếu bắn thành công (đủ đạn, đủ thời gian).
     */
    bool Shoot();

    /**
     * @brief Lấy vị trí đầu nòng súng (nơi đạn sẽ xuất hiện).
     * @return Tọa độ Vector2f của đầu nòng.
     */
    sf::Vector2f GetMuzzlePosition() const;

    void Reload();
    int GetCurrentAmmo() const { return currentAmmo; }
};

#endif // GUN_H