#include <EEPROM.h>
#include <Wire.h>
int sw[8] = {0, 1, 3, 4, 5, 6, 7, 15};        //15, 7, 6, 5, 4, 3, 1, 0
int led[8] = {17, 18, 19, 20, 16, 9, 8, 14};  //14, 8, 9, 16, 20, 19, 18, 17
int Tx = 12, Rx = 11, extra_2 = 13;

boolean sw_state[8] = {0, 0, 0, 0, 0, 0, 0, 0};
boolean last_switch_state[8] = {0, 0, 0, 0, 0, 0, 0, 0};
boolean ledState[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned long lastDebounceTime[8];
unsigned long debounceDelay = 50;
unsigned long last_data_come;
unsigned long eeprom_time = 30000;
boolean state[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//////////////////////////////////////////////////
int swt_a[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int main_sn[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int main_rv[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int led_a[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//////////////////////////////////////////////////
byte temp = B00000000;
byte temp2 = B00000000;




///////////////////////////////////////////////////////////////////
void requestEvent() {   //this part is to send the data to RPi
	for (int i = 0; i < 8; i++) {
		if (swt_a[i] != main_sn[i]) {
			main_sn[i] = swt_a[i];
		}
	}

	digitalWrite(Tx, HIGH);
	for (int i = 7; i > -1; i--) {
		temp = temp << 1;
		temp = temp + main_sn[i];
	}
	Wire.write(temp);        //Send the switch bit pattarn to the master
	digitalWrite(Tx, LOW);

}



/////////////////////////////////////////////////////////////////////////
void receiveEvent() {               //recieve the data from RPi
	if (Wire.available()) {
		digitalWrite(Rx, HIGH);
		last_data_come = millis();
		byte comming_byte = Wire.read();            //get the bit pattarn from the master
		if (comming_byte != B00001010) {
			temp2 = temp2 << 1;     //produce a bit pattarn for the LEDs
			if (comming_byte) {
				temp2 = temp2 + 1;
			}
		}
		else {
			for (int i = 0; i < 8; i++) {
				main_rv[i] = temp2 & B00000001;
				temp2 = temp2 >> 1;

				if (main_rv[i] != main_sn[i]) {
					swt_a[i] = main_rv[i];
					main_sn[i] = main_rv[i];
				}
				//digitalWrite(led[i], led_a[i]);
			}
		}


		digitalWrite(Rx, LOW);
	}
}


void setup() {
	pinMode(Tx, OUTPUT);
	pinMode(Rx, OUTPUT);
	pinMode(extra_2, OUTPUT);
	for (int i = 0; i < 8; i++) {
		pinMode(sw[i], INPUT);
		pinMode(led[i], OUTPUT);
		//digitalWrite(led[i], swt_a[i]);
	}

	Wire.begin(0x06);                // join i2c bus with address #4
	Wire.onReceive(receiveEvent); // register event
	Wire.onRequest(requestEvent);


	for (int i = 0; i < 8; i++) {
		swt_a[i] = EEPROM.read(i);
	}
}


///////////////////////////////////////////////////////////////////
void loop() {
	for (int i = 0; i < 8; i++) {
		int reading = digitalRead(sw[i]);
		if (reading != last_switch_state[i]) {
			lastDebounceTime[i] = millis();
		}

		if ((millis() - lastDebounceTime[i]) > debounceDelay) {
			if (reading != state[i]) {
				state[i] = reading;
				if (state[i] == HIGH) {
					swt_a[i] = !swt_a[i];

				}
			}
		}
		digitalWrite(led[i], swt_a[i]);
		last_switch_state[i] = reading;
	}

	if ((millis() - last_data_come) > eeprom_time) {
		for (int i = 0; i < 8; i++) {
			//EEPROM.update(i, swt_a[i]);
			digitalWrite(extra_2, 1);
		}
	}
	digitalWrite(extra_2, 0);
}
