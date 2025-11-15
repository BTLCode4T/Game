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
    };

    for (const auto& snd : loadList) {

        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(snd.file)) {
            std::cout << "Không thể load: " << snd.file << "\n";
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
