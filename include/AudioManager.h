#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

// Called from PatientUI, DoctorUI, AdminUI
// Real implementation set by SFMLUI when HMS_ENABLE_SFML is defined
void playClick();
void playError();

// Called by SFMLUI to install real audio callbacks
typedef void (*AudioCallback)();
void setAudioCallbacks(AudioCallback onClick, AudioCallback onError);

#endif
