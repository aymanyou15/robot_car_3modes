#define yellow_led  13
#define blue_led  12
#define red_led  11

#define push_button 10

#define echo_pin 2
#define trig_pin 3

#define motor1_en 9
#define motor1_pin1 8
#define motor1_pin2 7
#define motor2_en 5
#define motor2_pin1 6
#define motor2_pin2 4

#define Rx 1

#define ir_sensor A0

// global varibles
long duration = 0;   // ultrsonic parameters
int distance = 0;

int button_value = 0;
int mode = 0;
long time_passed = 0;   

char command;   // command received from bluetooth module

int ir_value = 0;


// functions decleration
void led_initialize(void);
void ultrasonic_initialize(void);
void motor_initialize(void);

void ultrasonic_sensor (void);
void car_forward(int pwm);
void car_backward(int pwm);
void car_rotate_right(int pwm);
void car_rotate_left(int pwm);
void car_stop(void);


void setup() {
  Serial.begin(9600);
  led_initialize();
  ultrasonic_initialize();
  motor_initialize();
  pinMode(Rx, INPUT);
  pinMode(ir_sensor, INPUT);

}

void loop() {

  button_value = digitalRead(push_button);

  if (millis() - time_passed >= 150)
  {
    time_passed = millis();
      if (button_value == HIGH && mode == 0)
      {
        digitalWrite(yellow_led, HIGH);
        digitalWrite(blue_led, LOW);
        digitalWrite(red_led, LOW);
        car_stop();
        mode = 1;
        Serial.println("yelow");
      }
      else if (button_value == HIGH && mode == 1)
      {
        digitalWrite(blue_led, HIGH);
        digitalWrite(yellow_led, LOW);
        digitalWrite(red_led, LOW);
        car_stop();
        mode = 2;
        Serial.println("blue");
      }
      else if (button_value == HIGH && mode == 2)
      {
        digitalWrite(red_led, HIGH);
        digitalWrite(blue_led, LOW);
        digitalWrite(yellow_led, LOW);
        car_stop();
        mode = 0;
        Serial.println("red");
      }
      else
      {
        // do nothing
      }
  }
  if (mode == 0)
  {
    ultrasonic_sensor();
    car_forward(180);
    if (distance < 20)
    {
      car_stop();
      car_rotate_right(150);
    }
  }
  else if (mode == 1)
  {
    if(Serial.available() > 0)
    { 
      command = Serial.read(); 
      Serial.println(command);
      car_stop();
      switch(command)
      {
        case 'F':  
          car_forward(180);
          break;
        case 'B':  
           car_backward(180);
          break;
        case 'L':  
          car_rotate_left(180);
          break;
        case 'R':
          car_rotate_right(180);
          break;
        default:
        car_stop();
      }
    }
  }
  else if (mode == 2)
  {
    ir_value = digitalRead(ir_sensor);
    Serial.println(ir_value);
 /*   if (ir_value == 1)
    {
      car_forward(80);
    }
    else
    {
      car_rotate_right(80);
    }*/
   }
   
  else
  {
    //do nothing
  }

}





// functions implementation

void led_initialize(void)
{
  pinMode(yellow_led , OUTPUT);
  pinMode(blue_led , OUTPUT);       // initialize the leds pins as output
  pinMode(red_led , OUTPUT);
  pinMode(push_button , INPUT);     // initialize the push button as input
}

void ultrasonic_initialize(void)
{
  pinMode(echo_pin, INPUT);         
  pinMode(trig_pin, OUTPUT);
}

void motor_initialize(void)
{
  pinMode(motor1_en, OUTPUT);
  pinMode(motor1_pin1, OUTPUT);
  pinMode(motor1_pin2, OUTPUT);
  pinMode(motor2_en, OUTPUT);
  pinMode(motor2_pin1, OUTPUT);
  pinMode(motor2_pin2, OUTPUT);
}

void ultrasonic_sensor(void)
{
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(5);
  
  digitalWrite(trig_pin, HIGH);     // triggering the ultrasonic sensor 
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  duration = pulseIn(echo_pin , HIGH);    // calculate the duration after the triggering pulse
  distance = (duration * 0.034 / 2);      // calculate the distance

  Serial.println(distance);
}

void car_forward(int pwm)
{
  digitalWrite(motor1_pin1, HIGH);
  digitalWrite(motor1_pin2, LOW);
  digitalWrite(motor2_pin1, LOW);
  digitalWrite(motor2_pin2, HIGH);
  
  analogWrite(motor1_en, pwm);
  analogWrite(motor2_en, pwm);
}


void car_backward(int pwm)
{
  digitalWrite(motor1_pin1, LOW);
  digitalWrite(motor1_pin2, HIGH);
  digitalWrite(motor2_pin1, HIGH);
  digitalWrite(motor2_pin2, LOW);
  
  analogWrite(motor1_en, pwm);
  analogWrite(motor2_en, pwm);
}


void car_rotate_right(int pwm)
{
  digitalWrite(motor1_pin1, HIGH);
  digitalWrite(motor1_pin2, LOW);
  digitalWrite(motor2_pin1, HIGH);
  digitalWrite(motor2_pin2, LOW);
  
  analogWrite(motor1_en, pwm);
  analogWrite(motor2_en, pwm);
}


void car_rotate_left(int pwm)
{
  digitalWrite(motor1_pin1, LOW);
  digitalWrite(motor1_pin2, HIGH);
  digitalWrite(motor2_pin1, LOW);
  digitalWrite(motor2_pin2, HIGH);
  
  analogWrite(motor1_en, pwm);
  analogWrite(motor2_en, pwm);
}


void car_stop(void)
{
  digitalWrite(motor1_pin1, LOW);
  digitalWrite(motor1_pin2, LOW);
  digitalWrite(motor2_pin1, LOW);
  digitalWrite(motor2_pin2, LOW);
}
