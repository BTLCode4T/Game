#include "Core/Audio/MusicManager.h"
#include <iostream>

MusicManager::MusicManager() {
    // Tự load file nhạc theo ngữ cảnh
    musicFiles = {
        {"menu",    "assets/music/menu.ogg"},
        {"HighScores",    "assets/music/HighScores.ogg"},
        // {"Game",    "assets/music/Game.ogg"},
        {"Help",    "assets/music/Help.ogg"},
        // {"Settings",    "assets/music/Settings.ogg"},
        {"run",   "assets/Sounds/run.wav"},
    };
}

void MusicManager::Play(const std::string& id, bool loop) {
    if (muted) return;

    auto it = musicFiles.find(id);
    if (it == musicFiles.end()) {
        std::cout << "Music '" << id << "' khong ton tai!\n";
        return;
    }

    currentMusic.stop();

    if (!currentMusic.openFromFile(it->second)) {
        std::cout << "Khong the load nhac: " << it->second << "\n";
        return;
    }

    currentMusic.setLooping(loop);
    currentMusic.setVolume(volume);
    currentMusic.play();

    currentID = id;
}

void MusicManager::Stop() {
    currentMusic.stop();
}

void MusicManager::SetMute(bool m) {
    muted = m;

    if (muted)
        currentMusic.setVolume(0);
    else
        currentMusic.setVolume(volume);
}

bool MusicManager::IsMuted() const {
    return muted;
}

void MusicManager::SetVolume(float v) {
    volume = v;
    if (!muted)
        currentMusic.setVolume(volume);
}

float MusicManager::GetVolume() const {
    return volume;
}

void MusicManager::FadeTo(const std::string& id, float fadeTime) {
    if (muted) return;

    if (id == currentID) return;

    fadeTarget = id;
    fadeDuration = fadeTime;
    fadeTimer = fadeTime;
    isFading = true;
}

void MusicManager::Update(float dt) {
    if (!isFading) return;

    fadeTimer -= dt;

    float t = fadeTimer / fadeDuration;
    if (t < 0) t = 0;

    currentMusic.setVolume(volume * t);

    if (t <= 0) {
        Play(fadeTarget);
        isFading = false;
    }
}
