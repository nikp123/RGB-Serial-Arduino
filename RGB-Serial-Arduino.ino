//pins for the LEDs:
#define REDPIN 6
#define GREENPIN 3
#define BLUEPIN 5
 
//#define VERBOSE

bool operation;

double delta[3];
unsigned char procColor[3], curColor[3] = {0,0,0}, newColor[3], instruction, i;
unsigned int steps, additional;
const unsigned char pins[3] = {REDPIN, GREENPIN, BLUEPIN};
unsigned long startTime = 0;

void setup() {
	pinMode(REDPIN, OUTPUT);
	pinMode(GREENPIN, OUTPUT);
	pinMode(BLUEPIN, OUTPUT);

	// send message to be sure if the serial works	
	Serial.begin(9600);
	#ifdef VERBOSE
	Serial.println("Ready!");
	#endif
}

#ifdef VERBOSE
void dumpValues(){
	for(i=0; i<3; i++) {
		Serial.print(F(" "));
		Serial.print(newColor[i], DEC);
	}
	Serial.print(F("\nDuration: "));
	Serial.print(steps, DEC);
	
	Serial.print(F("\nDelta:"));
	for(i=0; i<3; i++) {
		Serial.print(F(" "));
		Serial.print(delta[i], DEC);
	}
	Serial.print(F("\n"));
}
#endif

bool processOperation(void) {
	if(instruction != 0x01) { // abort operation
		for(i=3; i<3; i++) {
			curColor[i] = newColor[i];	
			analogWrite(pins[i], newColor[i]);
		}
	}
	
	// act as a instruction emulator
	switch(instruction) {
		case 0x00: // just change the color
			for(i=0; i<3; i++) {
				curColor[i] = newColor[i];
				analogWrite(pins[i], newColor[i]);
			}
			#ifdef VERBOSE
			Serial.print(F("Change colors:"));
			dumpValues();
			#endif
			return 0;
		case 0x02: // fade between colors
			for(i=0; i<3; i++) {
				delta[i] = (double)(newColor[i]-curColor[i])/steps;
			}	
			startTime = millis();
			#ifdef VERBOSE
			Serial.print(F("Fade colors:"));
			dumpValues();
			#endif
			return 1;
		case 0x03: // clear everything
			for(i=0; i<3; i++) {
				newColor[i] = 0;
				curColor[i] = newColor[i];
				analogWrite(pins[i], newColor[i]);
			}
			#ifdef VERBOSE
			Serial.println(F("Reset."));
			#endif
			return 0;
		case 0x04: // blink color
			analogWrite(pins[0], 0);
			analogWrite(pins[1], 0);
			analogWrite(pins[2], 0);
			if(additional){
				for(i=0; i<3; i++) {
					delta[i] = (double)(newColor[i]-curColor[i])/steps;
				}
			}	
			startTime = millis();
			#ifdef VERBOSE
			Serial.print(F("Blink color:"));
			dumpValues();
			#endif
			return 1;
		case 0x05: // random color fade
			for(i=0; i<3; i++) {
				newColor[i] = random(255);
				delta[i] = (double)(newColor[i]-curColor[i])/steps;
				startTime = millis();
			}	
			#ifdef VERBOSE
			Serial.print(F("Random color generator (with fading):"));
			dumpValues();
			#endif
			return 1;
		case 0x06: // aesthetic
			for(i=0; i<3; i++) {
				procColor[i] = newColor[i];
				if(!procColor[i]) newColor[i] = 0;
					else newColor[i] = procColor[i] - (procColor[i]<additional ? random(procColor[i]) : random(additional));
				delta[i] = (double)(newColor[i]-curColor[i])/steps;
				startTime = millis();
			}
			#ifdef VERBOSE
			Serial.print(F("A E S T H E T I C:"));
			dumpValues();
			#endif
			return 1;
		case 0x07: // rainbow
			analogWrite(pins[0], 255);
			analogWrite(pins[1], 0);
			analogWrite(pins[2], 0);
			startTime = millis();
			#ifdef VERBOSE
			Serial.print(F("Rainbow:"));
			dumpValues();
			#endif
			return 1;
	}
	return 0;	
}

void loop() {
	if(Serial.available()){
		while(isDigit(Serial.peek())){
			newColor[0] =	Serial.parseInt(); 
			newColor[1] =	Serial.parseInt(); 
			newColor[2] =	Serial.parseInt(); 
			instruction =	Serial.parseInt();
			steps =       Serial.parseInt();
			additional =  Serial.parseInt();
			// checks if the execution has finished
			operation = processOperation();
		}
		Serial.read();
	} else if(operation) {
		switch(instruction) {
			case 0x02: // fade between colors
				for(i=0; i<3; i++) {
					curColor[i] = newColor[i]-delta[i]*steps+delta[i]*(millis()-startTime);
					analogWrite(pins[i], curColor[i]);
				}
				if(millis()-startTime>steps) operation = 0;
				break;
			case 0x04:	// blink color
				if(additional) {
					for(i=0;i<3;i++) analogWrite(pins[i], (cos(((millis()-startTime)/(double)steps+0.5)*PI*2)/2+0.5)*newColor[i]);
				} else for(i=0;i<3;i++)
					analogWrite(pins[i], (millis()-startTime)/steps%2 ? newColor[i] : 0);
				break;
			case 0x05: // random color generator with fading and whatnot
				for(i=0; i<3; i++) {
					curColor[i] = newColor[i]-delta[i]*steps+delta[i]*(millis()-startTime);
					analogWrite(pins[i], curColor[i]);
				}
				if(millis()-startTime>steps){
					for(i=0; i<3; i++) {
						newColor[i] = random(255);
						delta[i] = (double)(newColor[i]-curColor[i])/steps;
						startTime = millis();
					}	
				}
				break;
			case 0x06: // aesthetic
				for(i=0; i<3; i++) {
					curColor[i] = newColor[i]-delta[i]*steps+delta[i]*(millis()-startTime);
					analogWrite(pins[i], curColor[i]);
				}
				if(millis()-startTime>steps){
					for(i=0; i<3; i++) {
						if(!procColor[i]) newColor[i] = 0;
						else newColor[i] = procColor[i] - (procColor[i]<additional ? random(procColor[i]) : random(additional));
						delta[i] = (double)(newColor[i]-curColor[i])/steps;
						startTime = millis();
					}	
				}
				break;
			case 0x07:
				for(i=0;i<3;i++)
					analogWrite(pins[i], (cos(((millis()-startTime)/(double)steps+((3.0-(double)i)/3.0))*PI*2)/2+0.5)*255);
				break;
		}
		
	}
}
