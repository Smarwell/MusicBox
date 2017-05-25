/*
 Name:		MusicBox.ino
 Created:	5/13/2017 11:35:00 PM
 Author:	Jacob Sacco
*/

const float tempo = 172.; //bpm

#define VOICE1 0b1 << 5
#define VOICE2 0b1 << 6

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


// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	DDRD |= VOICE1 | VOICE2;

	TCCR1A = 0b00000000;
	TCCR1B = 0b00001001;
	//sets timer1 to ctc mode, no prescaler

	//TIMSK1 |= (1 << OCIE1A);
	//tell it to trigger the interrupt on compare match
}



/*
unsigned long int pulse_duration;
unsigned long int pulse_start;
unsigned long int time;
unsigned long int start;
unsigned long int duration;
unsigned long int pulse1_duration;
unsigned long int pulse2_duration;
unsigned long int pulse1_start;
unsigned long int pulse2_start;

void tone(float freq, int mills) {
	pulse_duration = (unsigned long int)(1000000 / freq);
	pulse_start = micros();
	time = micros();
	start = time;
	duration = (unsigned long int)mills * 1000;
	while (time - start < duration) {
		time = micros();
		if (time - pulse_start >= pulse_duration) {
			pulse_start = time;
			PORTD ^= VOICE1;
		}
	}

}

void tone(float freq1, float freq2, int mills) {
	pulse1_duration = (unsigned long int)(1000000 / freq1);
	pulse2_duration = (unsigned long int)(1000000 / freq2);
	time = micros();
	pulse1_start = time;
	pulse2_start = time;
	start = time;
	duration = (unsigned long int)mills * 1000;
	while (time - start < duration) {
		time = micros();
		if (time - pulse1_start >= pulse1_duration) {
			pulse1_start = time;
			PORTD ^= VOICE1;
		}
		if (time - pulse2_start >= pulse2_duration) {
			pulse2_start = time;
			PORTD ^= VOICE2;
		}
	}
}
*/


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

inline void fancy_delay(long int microseconds) {
	TCNT1 = 0;
	cycles = microseconds << 4;
	overflows_needed = microseconds >> 12;
	for (overflows = 0; overflows < overflows_needed; overflows++) {
		while (TCNT1 < 65500);
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
		while (TCNT1 < 65500);
		TCNT1 = 0;
	}
	while (TCNT1 < cycles);
}

void tone(float freq, int mills) {
	pulse_duration = (long int)(494500. / freq);
	periods = (int)(mills * 1000. / (494500. / freq) / 2.);
	for (int i = 0; i < periods; i++) {
		PORTD ^= VOICE1;
		fancy_delay(pulse_duration);
		PORTD ^= VOICE1;
		fancy_delay(pulse_duration);
	}
}

void tone(float freq1, float freq2, int mills) {
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
}



// the loop function runs over and over again until power down or reset

void loop() {
	
	tone(D4, dq);
	tone(D3, dq);
	tone(D4, ds);
	delay(ts);
	tone(D3, ds);
	delay(ts);
	tone(D4, s);
	delay(s);
	tone(D4, q);
	tone(D3, dq);
	tone(D4, ds);
	delay(ts);
	tone(D3, ds);
	delay(ts);
	tone(D4, dq);
	tone(D3, dq);
	tone(D4, ds);
	delay(ts);
	tone(D3, ds);
	delay(ts);
	tone(D4, s);
	delay(s);
	tone(D4, q);
	tone(D3, dq);
	tone(D4, ds);
	delay(ts);
	tone(D3, ds);
	delay(ts);

	tone(D4, nA5, s);
	tone(D4, s);
	tone(D4, nA5, s);
	tone(D4, s);
	tone(D4, F5, s);
	tone(D4, s);
	tone(D4, D5, de);
	tone(D4, s);
	tone(D4, D5, de);
	tone(D4, s);

	tone(G4, G5, de);
	tone(G4, s);
	tone(G4, G5, de);
	tone(G4, s);
	tone(G4, G5, s);
	tone(G4, s);
	tone(G4, B5, s);
	tone(G4, s);
	tone(G4, B5, s);
	tone(G4, s);
	tone(G4, C6, s);
	tone(G4, s);
	tone(G4, D6, s);
	tone(G4, s);

	tone(C4, C6, s);
	tone(C4, s);
	tone(C4, C6, s);
	tone(C4, s);
	tone(C4, C6, s);
	tone(C4, s);
	tone(C4, G5, de);
	tone(C4, s);
	tone(C4, F5, de);
	tone(C4, s);

	tone(F4, nA5, de);
	tone(F4, s);
	tone(F4, nA5, de);
	tone(F4, s);
	tone(F4, nA5, s);
	tone(F4, s);

	tone(E4, G5, s);
	tone(E4, s);
	tone(E4, G5, s);
	tone(E4, s);
	tone(E4, nA5, s);
	tone(E4, s);
	tone(E4, G5, s);
	tone(E4, s);
	
	tone(D4, nA5, s);
	tone(D4, s);
	tone(D4, nA5, s);
	tone(D4, s);
	tone(D4, F5, s);
	tone(D4, s);
	tone(D4, D5, de);
	tone(D4, s);
	tone(D4, D5, de);
	tone(D4, s);

	tone(G4, G5, de);
	tone(G4, s);
	tone(G4, G5, de);
	tone(G4, s);
	tone(G4, G5, s);
	tone(G4, s);
	tone(G4, B5, s);
	tone(G4, s);
	tone(G4, B5, s);
	tone(G4, s);
	tone(G4, C6, s);
	tone(G4, s);
	tone(G4, D6, s);
	tone(G4, s);
	
	tone(C4, C6, s);
	tone(C4, s);
	tone(C4, C6, s);
	tone(C4, s);
	tone(C4, C6, s);
	tone(C4, s);
	tone(C4, nA5, de);
	tone(C4, s);
	tone(C4, F5, de);
	tone(C4, s);
	
	tone(F4, nA5, de);
	tone(F4, s);
	tone(F4, nA5, de);
	tone(F4, s);
	tone(F4, nA5, s);
	tone(F4, s);

	tone(E4, G5, s);
	tone(E4, s);
	tone(E4, G5, s);
	tone(E4, s);
	tone(E4, nA5, s);
	tone(E4, s);
	tone(E4, G5, s);
	tone(E4, s);

	tone(D4, F5, dq);
	tone(D4, F5, s);
	tone(D4, s);
	tone(D4, F5, q);
	tone(D4, E5, s);
	tone(D4, s);

	tone(G4, D5, h);
	tone(G4, dh);

	tone(C4, E5, s);
	tone(C4, s);
	tone(C4, E5, de);
	tone(C4, s);
	tone(C4, E5, de);
	tone(C4, s);
	tone(C4, C5, de);
	tone(C4, s);



	


	delay(5000);
}


/*
tone(E4, e);
tone(FS4, e);
tone(G4, e);
delay(instant);
tone(G4, e);
tone(nA4, e);
tone(FS4, e);
tone(E4, e);
tone(D4, h);
tone(D4, e);

delay(q);

tone(E4, e);
delay(instant);
tone(E4, e);
tone(FS4, e);
tone(G4, e);
tone(E4, q);
tone(D4, e);
tone(D5, e);
delay(e);
tone(D5, e);
tone(nA4, h);

delay(e);

tone(E4, e);
tone(E4, e);
tone(FS4, e);
tone(G4, e);
tone(E4, e);
tone(G4, e);
tone(nA4, q);
tone(FS4, e);
tone(E4, e);
tone(D4, h);

delay(e);

tone(E4, e);
tone(E4, e);
tone(FS4, e);
tone(G4, e);
tone(E4, e);
tone(D4, q);
tone(nA4, e);
delay(instant);
tone(nA4, e);
delay(instant);
tone(nA4, e);
tone(B4, e);
tone(nA4, h);
*/