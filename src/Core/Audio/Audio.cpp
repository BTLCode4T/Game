#include "Core/Audio/Audio.h"
#include <iostream>

Audio::Audio() {
    struct SoundInfo {
        std::string id;
        std::string file;
    };

    std::vector<SoundInfo> loadList = {
        {"click",   "assets/Sounds/open_win.mp3"},
        {"click_off",   "assets/Sounds/close_win.mp3"},
        {"switch_type",   "assets/Sounds/switch_type.mp3"},
        {"switch_task",   "assets/Sounds/switch_task.mp3"},
        {"run",   "assets/Sounds/run.wav"},
        {"dinosaur",   "assets/Sounds/dinosaur.mp3"},
        {"jump", "assets/Sounds/jump.wav"},
        {"gameOver", "assets/Sounds/game-over.mp3"},
    };

    for (const auto& snd : loadList) {

        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(snd.file)) {
            std::cout << "Khong the load: " << snd.file << "\n";
            continue;
        }

        buffers.emplace(snd.id, buffer);

        sounds.emplace(
            snd.id,
            sf::Sound(buffers[snd.id])
        );
    }
}

// =============================
// Getter / Setter mute
// =============================
void Audio::SetMute(bool m) {
    muted = m;

    if (muted) {
        for (auto& it : sounds) {
            it.second.stop();
        }
    }
}

bool Audio::IsMuted() const {
    return muted;
}

// =============================
// Play
// =============================
void Audio::Play(const std::string& id) {
    if (muted) return; 

    auto it = sounds.find(id);

    if (it == sounds.end()) {
        std::cout << "Sound '" << id << "' khong ton tai!\n";
        return;
    }

    it->second.play();
}

// =============================
bool Audio::Exists(const std::string& id) {
    return sounds.find(id) != sounds.end();
}

// =============================
// PlayTime
// =============================
void Audio::PlayTime(const std::string& id, int time_seconds) {
    if (muted) return; 

    auto it = sounds.find(id);

    if (it == sounds.end()) {
        std::cout << "Sound '" << id << "' khong ton tai!\n";
        return;
    }
    
    // 1. Dừng trước để đảm bảo lặp lại từ đầu
    it->second.stop(); 

    // 2. Thiết lập âm thanh lặp lại vô tận (Looping)
    it->second.setLooping(true); 

    // 3. Bắt đầu phát
    it->second.play();

    // ⚠️ LƯU Ý QUAN TRỌNG:
    // Hàm này CHỈ THIẾT LẬP âm thanh lặp lại.
    // Việc DỪNG âm thanh sau `time_seconds` và LẶP LẠI toàn bộ chu trình 
    // phải được quản lý bên ngoài, trong vòng lặp chính của ứng dụng (dùng sf::Clock).
    // Nếu Sleep/Delay ở đây, ứng dụng sẽ bị treo (blocked).
}

// =============================
// PlayVol
// =============================
void Audio::PlayVol(const std::string& id, float volume) {
    // Nếu bị mute, ta sẽ không phát
    if (muted) return; 

    auto it = sounds.find(id);

    if (it == sounds.end()) {
        std::cout << "Sound '" << id << "' khong ton tai!\n";
        return;
    }

    // 1. Thiết lập âm lượng cho âm thanh này
    // Volume trong SFML: 0.0f (tắt) đến 100.0f (max)
    it->second.setVolume(volume);

    // 2. Phát âm thanh
    it->second.play();
}


// =============================
// Stop
// =============================
void Audio::Stop(const std::string& id) {
    auto it = sounds.find(id);

    if (it != sounds.end()) {
        it->second.stop();
        // Quan trọng: Tắt cờ lặp (loop) khi dừng
        it->second.setLooping(false); 
    }
}

void Audio::PlayLoopVol(const std::string& id, float volume) {
    if (muted) return; 

    auto it = sounds.find(id);

    if (it == sounds.end()) {
        std::cout << "Sound '" << id << "' khong ton tai!\n";
        return;
    }

    it->second.stop(); 
    
    it->second.setLooping(true); 
    
    it->second.setVolume(volume);

    it->second.play();
}