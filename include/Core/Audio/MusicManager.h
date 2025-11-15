#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include <SFML/Audio.hpp>
#include <map>
#include <string>

class MusicManager {
private:
    std::map<std::string, std::string> musicFiles; 
    sf::Music currentMusic;

    float volume = 10.0f;
    bool muted = false;

    std::string currentID = "";
    float fadeSpeed = 80.0f; // volume giảm mỗi giây khi fade out

    MusicManager();

public:
    static MusicManager& Get() {
        static MusicManager instance;
        return instance;
    }

    MusicManager(const MusicManager&) = delete;
    void operator=(const MusicManager&) = delete;

    void Play(const std::string& id, bool loop = true);
    void Stop();
    void Update(float dt); // dùng cho fade

    void FadeTo(const std::string& id, float fadeTime = 1.0f);

    void SetMute(bool m);
    bool IsMuted() const;

    void SetVolume(float v);
    float GetVolume() const;

private:
    bool isFading = false;
    std::string fadeTarget = "";
    float fadeTimer = 0;
    float fadeDuration = 1.0f;
};

#endif
