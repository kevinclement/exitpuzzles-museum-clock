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
        
        tone(BUZZER_PIN, NOTE_A4, 500, BUZZER_CHANNEL);
        delay(100);
        tone(BUZZER_PIN, NOTE_F4, 500, BUZZER_CHANNEL);
        delay(100);
        tone(BUZZER_PIN, NOTE_G4, 500, BUZZER_CHANNEL);
        delay(100);
        tone(BUZZER_PIN, NOTE_C4, 500, BUZZER_CHANNEL);

        _playTone = false;
    }
}
