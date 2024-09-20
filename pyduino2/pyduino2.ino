/*
 * Sketch to control the pins of Arduino via serial interface
 *
 * Commands implemented with examples:
 *
 * - RD13 -> Reads the Digital input at pin 13
 * - RA4 - > Reads the Analog input at pin 4
 * - WD13:1 -> Writes 1 (HIGH) to digital output pin 13
 * - WA6:125 -> Writes 125 to analog output pin 6 (PWM)
 */


# include <Servo.h>

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;

// define the LED digit patterns, from 0 - 9, None (10), 
// and A,b,C,d,F,H,L,N,P,U (11 - 20)
// 1 = LED on, 0 = LED off, in this order:
//                74HC595 pin     Q0,Q1,Q2,Q3,Q4,Q5,Q6,Q7 
//                Mapping to      a,b,c,d,e,f,g of Seven-Segment LED
byte seven_seg_digits[21] = { B11111100,  // = 0
                              B01100000,  // = 1
                              B11011010,  // = 2
                              B11110010,  // = 3
                              B01100110,  // = 4
                              B10110110,  // = 5
                              B10111110,  // = 6
                              B11100000,  // = 7
                              B11111110,  // = 8
                              B11100110,  // = 9
                              B00000000,  // = None
                              B11101110,  // = A
                              B11111010,  // = b
                              B11110000,  // = C
                              B00111110,  // = d
                              B11100010,  // = F
                              B01101110,  // = H
                              B01110000,  // = L
                              B11101100,  // = N
                              B11100110,  // = P
                              B01111100,  // = U
                             };


char operation; // Holds operation (R, W, S)
char mode; // Holds the mode (D, A, Servo Number (1-5))
char seg_choice; // Holds the model of the seg (1 unit or 4 units) 
int pin_number; // Holds the pin number
int pin_number1; // Holds a second pin number
int pin_number2; // Holds a third pin number
int latchPin;
int clockPin;
int dataPin;
int digital_value; // Holds the digital value
int analog_value; // Holds the analog value
int servo_value; // Holds the servo angle
int value_to_write; // Holds the value that we want to write
int wait_for_transmission = 5; // Delay in ms in order to receive the serial data



void set_pin_mode(int pin_number, char mode){
    /*
     * Performs a pinMode() operation depending on the value of the parameter
     * mode :
     * - I: Sets the mode to INPUT
     * - O: Sets the mode to OUTPUT
     * - P: Sets the mode to INPUT_PULLUP
     */

    switch (mode){
        case 'I':
            pinMode(pin_number, INPUT);
            break;
        case 'O':
            pinMode(pin_number, OUTPUT);
            break;
        case 'P':
            pinMode(pin_number, INPUT_PULLUP);
            break;         
    }
}

void servo_attach (char mode,int pin_number){
    // Attaches the correct servo to the the pin number.
     switch (mode){
          case '1':  
              myservo1.attach(pin_number);
              break;
          case '2':
              myservo2.attach(pin_number);
              break;  
          case '3':
              myservo3.attach(pin_number);
              break;
          case '4':
              myservo4.attach(pin_number);
              break;
          case '5':
              myservo5.attach(pin_number);
              break;
          default:
              break;          
    }

    Serial.print('S');
    Serial.print(mode);
    Serial.println(pin_number);

    
}

void sevenSegsetup(int pin_number, int pin_number1, int pin_number2){

  /* Sets up the pins for thee Shift Register
  */

  latchPin = pin_number;
  clockPin = pin_number1;
  dataPin = pin_number2;

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
 
Serial.print('L');
Serial.print(latchPin);
Serial.print(clockPin);
Serial.println(dataPin);
}

void digital_read(int pin_number){
    /*
     * Performs a digital read on pin_number and returns the value read to serial
     * in this format: D{pin_number}:{value}\n where value can be 0 or 1
     */

    digital_value = digitalRead(pin_number);
    Serial.print('D');
    Serial.print(pin_number);
    Serial.print(':');
    Serial.println(digital_value); // Adds a trailing \n
}

void analog_read(int pin_number){
    /*
     * Performs an analog read on pin_number and returns the value read to serial
     * in this format: A{pin_number}:{value}\n where value ranges from 0 to 1023
     */

    analog_value = analogRead(pin_number);
    Serial.print('A');
    Serial.print(pin_number);
    Serial.print(':');
    Serial.println(analog_value); // Adds a trailing \n
}

void servo_read(int pin_number){
    /*
     * Performs an servo read on servo_number and returns the value read to serial
     * in this format: T:{servo_value}\n where value ranges from 1 to 
     * 180 degrees. 
     */

    switch (pin_number){
        case 1:
            servo_value = myservo1.read();
            break;
        case 2:
            servo_value = myservo2.read();
            break;  
        case 3:
            servo_value = myservo3.read();
            break;
        case 4:
            servo_value = myservo4.read();
            break;
        case 5:
            servo_value = myservo5.read();
            break;  
        default: //error handler
            break;      
    }

    Serial.print('T');
    Serial.print(pin_number);
    Serial.print(':');
    Serial.println(servo_value); // Adds a trailing \n
}

void digital_write(int pin_number, int digital_value){
    /*
     * Performs a digital write on pin_number with the digital_value
     * The value must be 1 or 0
     */
  digitalWrite(pin_number, digital_value);
}

void analog_write(int pin_number, int analog_value){
    /*
   * Performs an analog write on pin_number with the analog_value
   * The value must be range from 0 to 255
     */
  analogWrite(pin_number, analog_value);
}

void servo_write(int pin_number,int servo_value){
    /*
   * Performs an servo write on servo_number.
   * The value must be range from 0 to 180 degrees.
     */
   switch (pin_number){
        case 1:
            myservo1.write(servo_value);
            break;
        case 2:
            myservo2.write(servo_value);
            break;  
        case 3:
            myservo3.write(servo_value);
            break;
        case 4:
            myservo4.write(servo_value);
            break;
        case 5:
            myservo5.write(servo_value);
            break;  
        default: //error handler
            break;      
    }
}


void sevenSegWrite(byte digit) {

  /*  Writes a byte to the Shift Register
   *  Lights the LED elemsnts of a One Digit  
   *  Seven Segment Display. Must be (0-11(blank)).
   *  Can be adjusted in this sketch to include letters.
   */ 

  // set the latchPin to low potential, before sending data
  digitalWrite(latchPin, LOW);
     
  // the original data (bit pattern)
  shiftOut(dataPin, clockPin, LSBFIRST, seven_seg_digits[digit]);  
 
  // set the latchPin to high potential, after sending data
  digitalWrite(latchPin, HIGH);

}

void setup() {
    Serial.begin(9600); // Serial Port at 9600 baud
    Serial.setTimeout(100); // Instead of the default 1000ms, in order
                            // to speed up the Serial.parseInt()                                          
}

void loop() {
    // Check if characters available in the buffer
    if (Serial.available() > 0) {
        operation = Serial.read();
        delay(wait_for_transmission); // If not delayed, second character is not correctly read
        mode = Serial.read();
        pin_number = Serial.parseInt(); // Waits for an int to be transmitted
        if (Serial.read()==':'){
            value_to_write = Serial.parseInt(); // Collects the value to be written
        } else {
            pin_number1=Serial.parseInt(); // Collects the second pin for shift register
            if (Serial.read() == ','){
              pin_number2=Serial.parseInt(); // Collects the third pin for shift register
            }
        }    

        switch (operation){
            case 'R': // Read operation, e.g. RD12, RA4
                if (mode == 'D'){ // Digital read
                    digital_read(pin_number);
                } else if (mode == 'A'){ // Analog read
                    analog_read(pin_number);
                } else if (mode == 'S'){ // Servo read
                    servo_read(pin_number);
                } else {
                  break; // Unexpected mode
                }
                break;

            case 'W': // Write operation, e.g. WD3:1, WA8:255
                if (mode == 'D'){ // Digital write
                    digital_write(pin_number, value_to_write);
                } else if (mode == 'A'){ // Analog write
                    analog_write(pin_number, value_to_write);
                } else if (mode == 'S'){ // Servo write
                    servo_write (pin_number,value_to_write);
                } else if (mode == 'L'){ //Seven Segment Write for One Digit
                    sevenSegWrite(value_to_write);
                } else {    
                    break; // Unexpected mode
                }
                break;

            case 'M': // Pin mode, e.g. MI3, MO3, MP3
                set_pin_mode(pin_number, mode); // Mode contains I, O or P (INPUT, OUTPUT or PULLUP_INPUT)
                break;

            case 'S': // Pin mode to servo, e.g. S11, S15, must be a pin that supports PMW
                servo_attach(mode, pin_number);
                break;

            case 'L': // Pins for the shift register for the Seven Segment Displays
                sevenSegsetup (pin_number, pin_number1, pin_number2);  
                break;    

            default: // Unexpected char
                break;
        }
    }
}
