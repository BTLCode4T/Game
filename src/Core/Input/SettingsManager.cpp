#include "Core/Input/SettingsManager.h"
#include "Core/Audio/Audio.h"
#include "Core/Audio/MusicManager.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

SettingsManager::SettingsManager() {
    LoadSettings();
}

void SettingsManager::LoadSettings() {
    std::ifstream file(SETTINGS_FILE);
    
    // Giá trị mặc định
    musicVolume = 50.0f; 
    musicMuted = false;
    soundMuted = false;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string key;
            std::string valueStr;
            
            size_t delimiterPos = line.find(':');
            if (delimiterPos != std::string::npos) {
                key = line.substr(0, delimiterPos);
                valueStr = line.substr(delimiterPos + 1);

              
                key.erase(0, key.find_first_not_of(" \t\r\n"));
                key.erase(key.find_last_not_of(" \t\r\n") + 1);
                valueStr.erase(0, valueStr.find_first_not_of(" \t\r\n"));
                valueStr.erase(valueStr.find_last_not_of(" \t\r\n") + 1);

                if (key == "Volume") {
                    try {
                        musicVolume = std::stof(valueStr);
                    } catch (...) { /* Bỏ qua lỗi định dạng */ }
                } else if (key == "MusicMuted") {
                    musicMuted = (valueStr == "1");
                } else if (key == "SoundMuted") {
                    soundMuted = (valueStr == "1");
                }
            }
        }
        file.close();
    } else {
        // Nếu không mở được (file không tồn tại), lưu cài đặt mặc định
        std::cerr << "Settings file not found. Creating default settings." << std::endl;
        SaveSettingsInternal();
    }
    
    // Áp dụng cài đặt đã tải cho Audio và MusicManager
    MusicManager::Get().SetVolume(musicVolume);
    MusicManager::Get().SetMute(musicMuted);
    Audio::Get().SetMute(soundMuted);
}


void SettingsManager::SaveSettingsInternal() {
    std::cout << "Dang luu cai dat vao file...\n"; // THÊM DÒNG NÀY
    std::ofstream file(SETTINGS_FILE);
    if (file.is_open()) {
        file << "Volume: " << musicVolume << "\n";
        file << "MusicMuted: " << (musicMuted ? 1 : 0) << "\n";
        file << "SoundMuted: " << (soundMuted ? 1 : 0) << "\n";
        file.close();
        std::cout << "Luu cai dat thanh cong.\n"; // THÊM DÒNG NÀY
    } else {
        std::cerr << "Failed to save settings file!" << std::endl;
    }
}

void SettingsManager::SaveSettings() {
    SaveSettingsInternal();
}


void SettingsManager::SetMusicVolume(float v) {
    musicVolume = std::max(0.0f, std::min(100.0f, v)); 
    MusicManager::Get().SetVolume(musicVolume);
    SaveSettingsInternal();
}

void SettingsManager::SetMusicMuted(bool m) {
    musicMuted = m;
    MusicManager::Get().SetMute(musicMuted);
    SaveSettingsInternal();
}

void SettingsManager::SetSoundMuted(bool m) {
    soundMuted = m;
    Audio::Get().SetMute(soundMuted);
    SaveSettingsInternal();
}