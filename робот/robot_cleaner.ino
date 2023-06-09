// ������ ������������ � ������� M1+,M1-,M2+,M2-  
// Motor shield ���������� ������ �������� 6,5,7,4 ��� ���������� �������� 
#define SPEED_LEFT       6
#define SPEED_RIGHT      5 
#define DIR_LEFT         7
#define DIR_RIGHT        4
#define LEFT_SENSOR_PIN  8
#define RIGHT_SENSOR_PIN 9
 
// ��������, � ������� �� �������� ����� (0-255)
#define SPEED            255
 
// �������� ����������� ������� ��������
#define SLOW_SPEED       35
 
#define BACK_SLOW_SPEED  45
#define BACK_FAST_SPEED  255
 
// �����������, �������� �� ������� ��� ����� �����������
// ���� �� ���� ��� ��������
#define BRAKE_K          4
 
#define STATE_FORWARD    0
#define STATE_RIGHT      1
#define STATE_LEFT       2
 
#define SPEED_STEP       2
 
#define FAST_TIME_THRESHOLD     500
 
int state = STATE_FORWARD;
int currentSpeed = SPEED;
int fastTime = 0;
 
void runForward() 
{
    state = STATE_FORWARD;
 
    fastTime += 1;
    if (fastTime < FAST_TIME_THRESHOLD) {
        currentSpeed = SLOW_SPEED;
    } else {
        currentSpeed = min(currentSpeed + SPEED_STEP, SPEED);
    }
 
    analogWrite(SPEED_LEFT, currentSpeed);
    analogWrite(SPEED_RIGHT, currentSpeed);
 
    digitalWrite(DIR_LEFT, HIGH);
    digitalWrite(DIR_RIGHT, HIGH);
}
 
void steerRight() 
{
    state = STATE_RIGHT;
    fastTime = 0;
 
    // ��������� ������ ������ ������������ ������,
    // ����� ������ �������
    analogWrite(SPEED_RIGHT, 0);
    analogWrite(SPEED_LEFT, SPEED);
    //delay(100);
    digitalWrite(DIR_LEFT, HIGH);
    digitalWrite(DIR_RIGHT, HIGH);
}
 
void steerLeft() 
{
    state = STATE_LEFT;
    fastTime = 0;
 
    analogWrite(SPEED_LEFT, 0);
    analogWrite(SPEED_RIGHT, SPEED);
   // delay(100);
 
    digitalWrite(DIR_LEFT, HIGH);
    digitalWrite(DIR_RIGHT, HIGH);
}
 
 
void stepBack(int duration, int state) {
    if (!duration)
        return;
 
    // � ����������� �� ����������� �������� ��� �������� ����� �����
    // ������ ��������� �������� 
    int leftSpeed = (state == STATE_RIGHT) ? BACK_SLOW_SPEED : BACK_FAST_SPEED;
    int rightSpeed = (state == STATE_LEFT) ? BACK_SLOW_SPEED : BACK_FAST_SPEED;
 
    analogWrite(SPEED_LEFT, leftSpeed);
    analogWrite(SPEED_RIGHT, rightSpeed);
 
    // ������ ����
    digitalWrite(DIR_RIGHT, LOW);
    digitalWrite(DIR_LEFT, LOW);
 
    delay(duration);
}
 
 
void setup() 
{
    // ����������� ������ ����� 4,5,6,7 �� ����� �������� 
    for(int i = 4; i <= 7; i++)
        pinMode(i, OUTPUT);
 
    // ����� ���� �����
    runForward();
} 
 
void loop() 
{ 
    // ��� ����� ����� �� ������ ���� � ������ ������. � �������� ������ �� �����
    // ������������� �������� � ��������
    boolean left = digitalRead(LEFT_SENSOR_PIN);
    boolean right = digitalRead(RIGHT_SENSOR_PIN);
 
    // � ����� ��������� ����� �������?
    int targetState;
 
    if (left && right) {
        // ��� ��������� �� ����� ��� �� ������
        // ���� �����
        targetState = STATE_FORWARD;
    } else if (left && !right) {
        // ����� ������ ����� � ����
        // ������������ ������
        targetState = STATE_LEFT;
        stepBack(600, targetState);
    }else if (right && !left) {
        // ����� ������ ����� � ����
        // ������������ ������
        targetState = STATE_RIGHT;
        stepBack(600, targetState);
    }
    else if (!left && !right) {
      
       int shit = random(0,2);
      if(shit == 0){
        targetState = STATE_LEFT;
      }
      else if(shit == 1){
        targetState = STATE_RIGHT;
      }
      
      stepBack(700, targetState);

      
       //targetState = !STATE_FORWARD;
      
      /*if(!left && right) {steerRight(); }
      if (!right && left) {steerLeft(); }
      */ 
       

    } 
    
 
   /* if (state == STATE_FORWARD && targetState != STATE_FORWARD) {
        int brakeTime = (currentSpeed > SLOW_SPEED) ?
            currentSpeed : 0;
        stepBack(brakeTime, targetState);
    }*/
 
    switch (targetState) {
        case STATE_FORWARD:
            runForward();
            break;
 
        case STATE_RIGHT:
            steerRight();
            break;
 
        case STATE_LEFT:
            steerLeft();
            break;
    }
 
}