#include "AudioManager.h"

// Default no-op implementations (used in console mode)
static void defaultClick() {}
static void defaultError() {}

static AudioCallback g_onClick = defaultClick;
static AudioCallback g_onError = defaultError;

void setAudioCallbacks(AudioCallback onClick, AudioCallback onError)
{
    g_onClick = onClick ? onClick : defaultClick;
    g_onError = onError ? onError : defaultError;
}

void playClick() { g_onClick(); }
void playError() { g_onError(); }
