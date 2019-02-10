#include "roll.h"

struct event_t {
    unsigned char type;
    unsigned short arg1;
    unsigned char arg2;
};

const struct event_t STEPS[] = {
#include "loop.inc"
};

static const unsigned char SineTable[256] =
{
	128, 131, 134, 137, 140, 143, 146, 149,  152, 156, 159, 162, 165, 168, 171, 174,
	176, 179, 182, 185, 188, 191, 193, 196,  199, 201, 204, 206, 209, 211, 213, 216,
	218, 220, 222, 224, 226, 228, 230, 232,  234, 236, 237, 239, 240, 242, 243, 245,
	246, 247, 248, 249, 250, 251, 252, 252,  253, 254, 254, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 254, 254,  253, 252, 252, 251, 250, 249, 248, 247,
	246, 245, 243, 242, 240, 239, 237, 236,  234, 232, 230, 228, 226, 224, 222, 220,
	218, 216, 213, 211, 209, 206, 204, 201,  199, 196, 193, 191, 188, 185, 182, 179,
	176, 174, 171, 168, 165, 162, 159, 156,  152, 149, 146, 143, 140, 137, 134, 131,

	128, 124, 121, 118, 115, 112, 109, 106,  103,  99,  96,  93,  90,  87,  84,  81,
	 79,  76,  73,  70,  67,  64,  62,  59,   56,  54,  51,  49,  46,  44,  42,  39,
	 37,  35,  33,  31,  29,  27,  25,  23,   21,  19,  18,  16,  15,  13,  12,  10,
	  9,   8,   7,   6,   5,   4,   3,   3,    2,   1,   1,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   1,   1,    2,   3,   3,   4,   5,   6,   7,   8,
	  9,  10,  12,  13,  15,  16,  18,  19,   21,  23,  25,  27,  29,  31,  33,  35,
	 37,  39,  42,  44,  46,  49,  51,  54,   56,  59,  62,  64,  67,  70,  73,  76,
	 79,  81,  84,  87,  90,  93,  96,  99,  103, 106, 109, 112, 115, 118, 121, 124,
};

signed short s_amp;
signed short s_decay;
unsigned short p_offset;
unsigned short s_phase;
unsigned short s_rate;
signed short m_wait;
unsigned short r_samplerate;
unsigned short r_samplerate2;

signed char inline __attribute__((always_inline)) _dumbsynth_sample() {
    s_amp -= s_decay;

    if (s_amp < 0) {
        s_amp = 0;
    }

    s_phase += s_rate;

    return (
        (s_amp >> 8)
        *
        ((short)SineTable[(s_phase >> 6) & 255] - 127)
    ) / 128;
}

void librick_step() {
    struct event_t *event = &STEPS[p_offset];

    switch(event->type) {
        case 1:
            m_wait = (event->arg1 / 4) * r_samplerate2;
            p_offset ++;
            break;

        case 2:
            s_rate = (short)(event->arg1 * 16) / (short)(r_samplerate2);
            s_decay = (signed short)(256 / r_samplerate2);
            s_amp = 230 * 128;
            p_offset ++;
            break;

        default:
            p_offset = 0;
            break;
    }
}

void librick_init(int samplerate) {
    r_samplerate = samplerate;
    r_samplerate2 = samplerate >> 8;
    m_wait = 0;
    s_amp = 0;
    s_rate = 0;
    s_phase = 0;
    s_decay = 0;
    p_offset = 0;
}

void librick_render(signed char *output, unsigned short maxsamples) {
    for(int i=0; i<maxsamples; i++) {
        if (m_wait > 0) {
            m_wait --;
        } else {
            m_wait = 0;
            librick_step();
        }

        output[i] = _dumbsynth_sample();
    }
}
