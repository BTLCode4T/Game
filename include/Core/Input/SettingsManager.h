#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <string>

class SettingsManager {
private:
    float musicVolume; // Âm lượng nhạc (0.0f - 100.0f)
    bool musicMuted;   // Trạng thái mute nhạc
    bool soundMuted;   // Trạng thái mute âm thanh hiệu ứng

    const std::string SETTINGS_FILE = "SettingsManager.txt";

    // Constructor riêng để đảm bảo Singleton và tải cài đặt lúc khởi tạo
    SettingsManager(); 

    // Phương thức tải cài đặt từ file
    void LoadSettings();
    // Phương thức lưu cài đặt vào file
    void SaveSettingsInternal(); 

public:
    // Singleton Instance
    static SettingsManager& Get() {
        static SettingsManager instance;
        return instance;
    }

    // Xóa copy constructor và assignment operator để ngăn việc tạo bản sao
    SettingsManager(const SettingsManager&) = delete;
    void operator=(const SettingsManager&) = delete;

    // --- Getter ---
    float GetMusicVolume() const { return musicVolume; }
    bool IsMusicMuted() const { return musicMuted; }
    bool IsSoundMuted() const { return soundMuted; }

    // --- Setter ---
    // Cập nhật giá trị và lưu vào file
    void SetMusicVolume(float v);
    void SetMusicMuted(bool m);
    void SetSoundMuted(bool m);
    
    // Phương thức công khai để lưu cài đặt (nếu cần gọi thủ công)
    void SaveSettings(); 
};

#endif