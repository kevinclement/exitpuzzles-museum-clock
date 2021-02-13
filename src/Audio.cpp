#include "Arduino.h"
#include "Audio.h"

bool _playTone = false;

Audio::Audio(Logic &logic)
: _logic(logic)
{  
}

void Audio::setup() {
}

void Audio::handle() {
    if (_playTone) {
        // tone(BUZZER_PIN, 700, 120, BUZZER_CHANNEL);
        // delay(80);
        // tone(BUZZER_PIN, 700, 120, BUZZER_CHANNEL);
        // tone(BUZZER_PIN, 900, 220, BUZZER_CHANNEL);
        tone(BUZZER_PIN, NOTE_C6, 120, BUZZER_CHANNEL);
        delay(80);
        tone(BUZZER_PIN, NOTE_F6, 120, BUZZER_CHANNEL);
        tone(BUZZER_PIN, NOTE_C7, 220, BUZZER_CHANNEL);

        _playTone = false;
    }
}
