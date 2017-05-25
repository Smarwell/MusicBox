#pragma once

#define VOICE1 0b1 << 5
#define VOICE2 0b1 << 6

extern const float tempo;

const int q = 1 * (60000 / tempo);
const int h = 2 * (60000 / tempo);
const int w = 4 * (60000 / tempo);
const int e = 0.5 * (60000 / tempo);
const int s = 0.25 * (60000 / tempo);
const int dq = 1.5 * (60000 / tempo);
const int dh = 3 * (60000 / tempo);
const int de = 0.75 * (60000 / tempo);
const int ds = 0.375 * (60000 / tempo);
const int ts = 0.125 * (60000 / tempo);

const int instant = 5;

const float C3 = 130.81;
const float CS3 = 138.59;
const float D3 = 146.83;
const float DS3 = 155.56;
const float E3 = 164.81;
const float F3 = 174.61;
const float FS3 = 185;
const float G3 = 196;
const float GS3 = 207.65;
const float nA3 = 220;
const float AS3 = 233.08;
const float B3 = 246.94;
const float C4 = C3 * 2;
const float CS4 = CS3 * 2;
const float D4 = D3 * 2;
const float DS4 = DS3 * 2;
const float E4 = E3 * 2;
const float F4 = F3 * 2;
const float FS4 = FS3 * 2;
const float G4 = G3 * 2;
const float GS4 = GS3 * 2;
const float nA4 = nA3 * 2;
const float AS4 = AS3 * 2;
const float B4 = B3 * 2;
const float C5 = C4 * 2;
const float CS5 = CS4 * 2;
const float D5 = D4 * 2;
const float DS5 = DS4 * 2;
const float E5 = E4 * 2;
const float F5 = F4 * 2;
const float FS5 = FS4 * 2;
const float G5 = G4 * 2;
const float GS5 = GS4 * 2;
const float nA5 = nA4 * 2;
const float AS5 = AS4 * 2;
const float B5 = B4 * 2;
const float C6 = C5 * 2;
const float CS6 = CS5 * 2;
const float D6 = D5 * 2;


long int pulse_duration;
long int pulse1_duration;
long int pulse2_duration;
long int pulse1_remaining;
long int pulse2_remaining;
int cycles;
int overflows_needed;
int overflows;
long int delay_time;
int periods;

void work_plz() {
	DDRD |= VOICE1 | VOICE2;
	//sets the output pins to output

	TCCR1A = 0b00000000;
	TCCR1B = 0b00001001;
	//sets timer1 to ctc mode, no prescaler
}

inline void fancy_delay(long int microseconds) {
	TCNT1 = 0;
	cycles = microseconds << 4;
	overflows_needed = microseconds >> 12;
	for (overflows = 0; overflows < overflows_needed; overflows++) {
		while (TCNT1 < 65520);
		TCNT1 = 0;
	}
	while (TCNT1 < cycles);
}

inline void set_timer() {
	TCNT1 = 0;
}

inline void fancier_delay(long int microseconds) {
	cycles = microseconds << 4;
	overflows_needed = microseconds >> 12;
	for (overflows = 0; overflows < overflows_needed; overflows++) {
		while (TCNT1 < 65520);
		TCNT1 = 0;
	}
	while (TCNT1 < cycles);
}

void tone(float freq, int mills) {
	noInterrupts();
	pulse_duration = (long int)(494500. / freq);
	periods = (int)(mills * 1000. / (494500. / freq) / 2.);
	for (int i = 0; i < periods; i++) {
		PORTD ^= VOICE1;
		fancy_delay(pulse_duration);
		PORTD ^= VOICE1;
		fancy_delay(pulse_duration);
	}
	interrupts();
}

void tone(float freq1, float freq2, int mills) {
	noInterrupts();
	pulse1_duration = (long int)(494500. / freq1);
	pulse2_duration = (long int)(494500. / freq2);
	pulse1_remaining = 0;
	pulse2_remaining = 0;
	periods = (int)(mills * 1000. / (494500. / freq2));
	for (int i = 0; i < periods;) {
		set_timer();
		if (pulse1_remaining <= 16) {
			pulse1_remaining = pulse1_duration;
			PORTD ^= VOICE1;
		}
		if (pulse2_remaining <= 16) {
			pulse2_remaining = pulse2_duration;
			PORTD ^= VOICE2;
			i++;
		}
		delay_time = min(pulse1_remaining, pulse2_remaining);
		pulse1_remaining = pulse1_remaining - delay_time;
		pulse2_remaining = pulse2_remaining - delay_time;
		fancier_delay(delay_time);
	}
	interrupts();
}
