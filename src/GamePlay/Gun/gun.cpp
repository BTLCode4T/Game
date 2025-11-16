#include "GamePlay/Gun/gun.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Gun::Gun(const string &name, const string &texturePath, float width, float height,
         sf::Vector2i frameNum, float frameTime,
         int maxAmmo, float fireRate, sf::Vector2f holdOffset)
    : Entity("Gun", name, 0, 0, 1, 0.f, // type, name, x, y, maxHealth, speed (không cần)
             texturePath, width, height, frameNum, frameTime),
      currentAmmo(maxAmmo), maxAmmo(maxAmmo), fireRate(fireRate), holdOffset(holdOffset) 
{
    // Đặt tâm của sprite súng vào giữa (hoặc vị trí báng súng)
    // Giả sử báng súng là ở giữa bên trái
    sf::Vector2i frameSize = animation->getFrameSize();
    animation->setOrigin(sf::Vector2f(frameSize.x * 0.1f, frameSize.y / 2.f));
}

void Gun::UpdateGun(sf::Vector2f playerCenter, sf::Vector2f mousePos) {
    // 1. Đặt vị trí súng theo người chơi + offset
    sf::Vector2f gunPos = playerCenter + holdOffset;
    animation->setPosition(gunPos);

    // 2. Tính toán góc quay theo chuột
    float dx = mousePos.x - gunPos.x;
    float dy = mousePos.y - gunPos.y;
    
    // atan2 trả về radian, sau đó chuyển sang độ
    float angleDeg = std::atan2(dy, dx) * 180.f / M_PI;

    animation->setRotation(sf::degrees(angleDeg));
}

bool Gun::Shoot() {
    // Kiểm tra xem đã đủ thời gian chờ (fireRate) và còn đạn chưa
    if (fireTimer.getElapsedTime().asSeconds() >= fireRate && currentAmmo > 0) {
        currentAmmo--;
        fireTimer.restart();
        std::cout << "BAN! Dan con lai: " << currentAmmo << "/" << maxAmmo << std::endl;
        return true;
    }
    return false;
}

sf::Vector2f Gun::GetMuzzlePosition() const {
    // Giả sử đầu nòng súng là ở bên phải, tâm
    sf::Vector2i frameSize = animation->getFrameSize();
    sf::Vector2f muzzleOffset(frameSize.x * 0.9f, frameSize.y / 2.f); // Vị trí đầu nòng (local)
    
    // TransformPoint sẽ tính toán vị trí global dựa trên vị trí, góc quay, và origin
    return animation->getTransform().transformPoint(muzzleOffset);
}

void Gun::Reload() {
    currentAmmo = maxAmmo;
    std::cout << "DA NAP DAN!" << std::endl;
}