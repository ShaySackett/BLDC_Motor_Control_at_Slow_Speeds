
 
#define PWM_A_MOTOR1 3 // arduino pin 3, PD3,OC2B, --> physical pin 1 --> martinez schematic PWM 1 --> L6234 IN1
#define PWM_B_MOTOR1 5 // arduino pin 5, PD5,OCOB, --> physical Pin 9 --> martinez schematic PWM2 --> L6234 IN2 
#define PWM_C_MOTOR1 6 // arduino pin 6, PD6,OCOA, --> physical Pin 10 --> martinez schematic PWM3 --> L6234 IN3 

#define PWM_A_MOTOR2 9 // arduino pin 9, PB1,OC1A, --> physical Pin 13 --> martinez schematic PWM4 --> L6234 IN1 
#define PWM_B_MOTOR2 10 // arduino pin 10, PB2,OC1B, --> physical Pin 14 --> martinez schematic PWM5 --> L6234 IN2  
#define PWM_C_MOTOR2 11 // arduino pin 11, PD3,OC2A, --> physical Pin 15 --> martinez schematic PWM6 --> L6234 IN3 

const byte elect_to_mech_factor = 7; // With every x amount of this variable we get 1 mechanical rotation. 

float motor_diameter = .028; // diameter of wheel/motor combo in meters
float motor_velocity = 0;   //set desired motor velocity here in meters/sec

unsigned long loop_counter = 0;

volatile unsigned long array_pos = 0; 
volatile unsigned long ISR_counter = 0;
volatile unsigned long  motor_pause_time = 0;  //time in milliseconds to pause between each "step" of the motor; 1000 microseconds = 1 millisecond
volatile byte dir = 1; //motor direction, 1 = forward, 0 = backward

//EN1, 2 and 3 ARE TIED TO 5V!!! DON'T NEED TO SET THEM LOGIC HIGH!

// One period of sine in SineArraySize # of approximations, this whole array is equivalent to 1 "electrical" rotation. 
const int pwmSin[] = 
{127,128,129,130,131,133,134,135,136,137,138,139,140,141,142,144,145,146,147,148,149,150,151,152,153,154,156,157,158,159,160,161,162,163,164,165,166,167,
168,169,170,171,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,190,191,192,193,194,195,196,197,198,199,200,201,202,203,203,204,
205,206,207,208,209,209,210,211,212,213,214,214,215,216,217,218,218,219,220,221,221,222,223,224,224,225,226,226,227,228,228,229,230,230,231,232,232,233,
234,234,235,235,236,236,237,238,238,239,239,240,240,241,241,242,242,243,243,243,244,244,245,245,246,246,246,247,247,247,248,248,248,249,249,249,250,250,
250,250,251,251,251,251,252,252,252,252,252,253,253,253,253,253,253,253,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,
254,253,253,253,253,253,253,253,252,252,252,252,252,251,251,251,251,250,250,250,250,249,249,249,248,248,248,247,247,247,246,246,246,245,245,244,244,243,
243,243,242,242,241,241,240,240,239,239,238,238,237,236,236,235,235,234,234,233,232,232,231,230,230,229,228,228,227,226,226,225,224,224,223,222,221,221,
220,219,218,218,217,216,215,214,214,213,212,211,210,209,209,208,207,206,205,204,203,203,202,201,200,199,198,197,196,195,194,193,192,191,190,190,189,188,
187,186,185,184,183,182,181,180,179,178,177,176,175,174,173,171,170,169,168,167,166,165,164,163,162,161,160,159,158,157,156,154,153,152,151,150,149,148,
147,146,145,144,142,141,140,139,138,137,136,135,134,133,131,130,129,128,127,126,125,124,123,121,120,119,118,117,116,115,114,113,112,110,109,108,107,106,
105,104,103,102,101,100,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,63,62,61,60,59,58,57,56,
55,54,53,52,51,51,50,49,48,47,46,45,45,44,43,42,41,40,40,39,38,37,36,36,35,34,33,33,32,31,30,30,29,28,28,27,26,26,25,24,24,23,22,22,21,20,20,19,19,18,18,
17,16,16,15,15,14,14,13,13,12,12,11,11,11,10,10,9,9,8,8,8,7,7,7,6,6,6,5,5,5,4,4,4,4,3,3,3,3,2,2,2,2,2,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,6,6,6,7,7,7,8,8,8,9,9,10,10,11,11,11,12,12,13,13,14,14,15,15,16,16,17,18,18,19,19,20,20,21,22,22,23,
24,24,25,26,26,27,28,28,29,30,30,31,32,33,33,34,35,36,36,37,38,39,40,40,41,42,43,44,45,45,46,47,48,49,50,51,51,52,53,54,55,56,57,58,59,60,61,62,63,63,64,
65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,100,101,102,103,104,105,106,107,108,109,110,112,113,
114,115,116,117,118,119,120,121,123,124,125,126,127};

int sineArraySize = sizeof(pwmSin)/sizeof(pwmSin[0]);   
int phaseShift = sineArraySize / 3; 
int phaseOffsetA = 0;
int phaseOffsetB = phaseOffsetA + phaseShift;
int phaseOffsetC = phaseOffsetB + phaseShift;

void setup() {
  Serial.begin(9600);
  timer_setup(); 
   motor_pause_time = ISR_counter_delay(motor_velocity,motor_diameter, sineArraySize,elect_to_mech_factor);
  dir = 0;
  delay(500000);
}

void loop() {

    if(loop_counter >= 100){
    motor_velocity += .001;
    motor_pause_time = ISR_counter_delay(motor_velocity,motor_diameter, sineArraySize,elect_to_mech_factor);  //calculate how long to make the motor delay to get a certain velocity
    Serial.println(motor_pause_time);
 
   loop_counter = 0;
    }
   loop_counter++;
   setMotorPosition(2,array_pos,128,sineArraySize);
   
   
   /*
   motor_velocity = 1;
   motor_pause_time = ISR_counter_delay(motor_velocity,motor_diameter, sineArraySize,elect_to_mech_factor);
   setMotorPosition(2,array_pos,128,sineArraySize);
   */

/*Doesn't work quite right
   motor_pause_time = ISR_RPM_counter_delay(1);
   setMotorPosition(2,array_pos,128,sineArraySize);
   */
}

void setMotorPosition(int motor, unsigned long posStep, int power, int arraySize) {
  int pin1, pin2, pin3;
  int pwm_a, pwm_b, pwm_c;

  power = constrain(power, 0, 255); 

  if (motor == 1) {
    pin1 = PWM_A_MOTOR1;
    pin2 = PWM_B_MOTOR1;
    pin3 = PWM_C_MOTOR1;
  }
  if (motor == 2) {
    pin1 = PWM_A_MOTOR2;
    pin2 = PWM_B_MOTOR2;
    pin3 = PWM_C_MOTOR2;
  }

  // get number from the sin table, change amplitude from max
  pwm_a = ((unsigned long)pwmSin[(posStep + (unsigned long)phaseOffsetA) % (unsigned long)arraySize]) * ((unsigned long)power / 255.0);   //duty cycle of each pwm 
  pwm_b = ((unsigned long)pwmSin[(posStep + (unsigned long)phaseOffsetB) % (unsigned long)arraySize]) * ((unsigned long)power / 255.0);
  pwm_c = ((unsigned long)pwmSin[(posStep + (unsigned long)phaseOffsetC) % (unsigned long)arraySize]) * ((unsigned long)power / 255.0);

  analogWrite(pin1, pwm_a);
  analogWrite(pin2, pwm_b);
  analogWrite(pin3, pwm_c);
}

void timer_setup() {  //TCCR0B,TCCR0A initialize to 0x03, Fast PWM, TCCR1A/B initialize to 0x01, Phase Correct PWM, so set TCCR0B and TCCR0A to phase correct PWM. 
  // sets the speed of PWM signals. 
 
  TCCR0A = 0b1010001;
  TCCR0B = 0b00000001;
  TCCR0B = 0b00000001;
  TCCR1B = 0b00000001;
  TCCR2B = 0b00000001;

  noInterrupts();

  TIMSK1 = 0b00000001;  //set TO1E to on

  interrupts();
  
  pinMode(PWM_A_MOTOR1, OUTPUT); 
  pinMode(PWM_B_MOTOR1, OUTPUT); 
  pinMode(PWM_C_MOTOR1, OUTPUT); 
  
  pinMode(PWM_A_MOTOR2, OUTPUT); 
  pinMode(PWM_B_MOTOR2, OUTPUT); 
  pinMode(PWM_C_MOTOR2, OUTPUT); 
}

ISR(TIMER1_OVF_vect){    //ISR runs every .063 milliseconds or 63 microseconds, KEEP ISR FAST, DO NOT PUT SERIAL MONITORS DOWN HERE OR ANYTHING EXTRANEOUS
  ISR_counter++;

  if (ISR_counter >= motor_pause_time && dir == 1){ //15873 counts ~1 second,
  array_pos++;
  ISR_counter = 0;
  }
  if (ISR_counter >= motor_pause_time && dir == 0){ //15873 counts ~1 second,
  array_pos--;
  ISR_counter = 0;
  
  }

}

float ISR_counter_delay(float velocity, float diameter, int array_length, int commutation_factor){   //Given a velocity and motor diameter, this function calculates a necessary ISR timer delay to achieve the velocity of the motor with open loop control.  Feed velocity in m/s. Although time cancels out here. 
 float isr_comparison_value = (3.1415*diameter)/(velocity*.00001598*(float)array_length*(float)commutation_factor);
 return isr_comparison_value;
}
/*   //This function doesn't work the way I want it to. 
float ISR_RPM_counter_delay(float RPM){  //convert a desired RPM into a step delay for the ISR
  float isr_comparison_value = 174.603*RPM; //formula for 720 steps and 7 mechanical rotations is .011*RPM = desired delay time in seconds
  return isr_comparison_value;
}
*/
