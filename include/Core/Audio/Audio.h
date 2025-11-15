#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundStream.hpp>
#include <map>
#include <string>

class Audio {
private:
    std::map<std::string, sf::SoundBuffer> buffers;
    std::map<std::string, sf::Sound> sounds;

    bool muted = false;   // ⚡ Cờ hiệu mute

    Audio(); // constructor riêng để load âm thanh

public:
    static Audio& Get() {
        static Audio instance;
        return instance;
    }

    Audio(const Audio&) = delete;
    void operator=(const Audio&) = delete;

    void Play(const std::string& id);

    bool Exists(const std::string& id);

    // ⚡ Setter & Getter cho mute
    void SetMute(bool m);
    bool IsMuted() const;
};

#endif
