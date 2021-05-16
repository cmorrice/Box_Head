////// pinout //////
#define clockPin 13
#define latchPin 12
#define dataPin 11
#define leftTop 10
#define leftUpper 9
#define leftLower 8
#define leftBottom 7
#define rightTop 6
#define rightUpper 5
#define rightLower 4
#define rightBottom 3
/////////////////////
const int size = 16;

const byte pattern[size] = {B00000000, B11100000, B00011100, B00000011, B10000000, B01110000, B00000000, B00001000, B00000000, B00010000, 
B00000000, B00100000, B00000000, B01000000, B00000000, B10000000};

////////////////Accelerometer////////////////
/////////////////////////////////////////////
#include "Wire.h" // This library allows you to communicate with I2C devices.

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.

int16_t accelerationX, accelerationY, accelerationZ; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data

char tmp_str[7]; // temporary variable used in convert function

char* convert_int16_to_str(int16_t i) // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
{
    sprintf(tmp_str, "%6d", i);
    return tmp_str;
}
////////////////Accelerometer////////////////
/////////////////////////////////////////////

void setup() 
{
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(leftTop, OUTPUT);
    pinMode(leftUpper, OUTPUT);
    pinMode(leftLower, OUTPUT);
    pinMode(leftBottom, OUTPUT);
    pinMode(rightTop, OUTPUT);
    pinMode(rightUpper, OUTPUT);
    pinMode(rightLower, OUTPUT);
    pinMode(rightBottom, OUTPUT);
    digitalWrite(leftTop, HIGH);
    digitalWrite(leftUpper, HIGH);
    digitalWrite(leftLower, HIGH);
    digitalWrite(leftBottom, HIGH);
    digitalWrite(rightTop, HIGH);
    digitalWrite(rightUpper, HIGH);
    digitalWrite(rightLower, HIGH);
    digitalWrite(rightBottom, HIGH);
    Serial.begin(9600);
    
    ///////////////////////////
    ///////////////////////////
    Wire.begin();
    Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
    Wire.write(0x6B); // PWR_MGMT_1 register
    Wire.write(0); // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);
    ///////////////////////////
    ///////////////////////////

}

void shiftOne(uint8_t dataPin, uint8_t clockPin, bool val)
{
    digitalWrite(latchPin, LOW);
    
    digitalWrite(dataPin, val);
    
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
    digitalWrite(latchPin, HIGH);
}

void send(byte numberToDisplayFirst, byte numberToDisplaySecond)
{
    digitalWrite(latchPin, LOW);
    
    //shiftOut(dataPin, clockPin, LSBFIRST, (numberToDisplay >> 8));
    shiftOut(dataPin, clockPin, LSBFIRST, numberToDisplayFirst);
    shiftOut(dataPin, clockPin, LSBFIRST, numberToDisplaySecond);
    
    digitalWrite(latchPin, HIGH);
}

void sendYuh(unsigned int numberToDisplay)
{
    digitalWrite(latchPin, LOW);
    
    shiftOut(dataPin, clockPin, LSBFIRST, (numberToDisplay >> 8));
    shiftOut(dataPin, clockPin, LSBFIRST, numberToDisplay);
    
    digitalWrite(latchPin, HIGH);
    
    Serial.println(numberToDisplay);
}


void loop() 
{
    if (accelerationX > 15000 || accelerationX < -15000) // head tilted almost all the way to the left
    {
        if (accelerationX > 15000)
        {
            digitalWrite(leftTop, LOW);
            digitalWrite(leftUpper, HIGH);
            digitalWrite(leftLower, HIGH);
            digitalWrite(leftBottom, HIGH);
            digitalWrite(rightTop, HIGH);
            digitalWrite(rightUpper, HIGH);
            digitalWrite(rightLower, HIGH);
            digitalWrite(rightBottom, LOW);
        }
        else
        {
            digitalWrite(rightTop, LOW);
            digitalWrite(rightUpper, HIGH);
            digitalWrite(rightLower, HIGH);
            digitalWrite(rightBottom, HIGH);
            digitalWrite(leftTop, HIGH);
            digitalWrite(leftUpper, HIGH);
            digitalWrite(leftLower, HIGH);
            digitalWrite(leftBottom, LOW);
        }
        //send(pattern[5], pattern[0]);
        //delay(25);
    }
    else if (accelerationX > 12000 || accelerationX < -12000) // head tilted a lot to the left
    {
        if (accelerationX > 12000)
        {
            digitalWrite(leftTop, HIGH);
            digitalWrite(leftUpper, LOW);
            digitalWrite(leftLower, HIGH);
            digitalWrite(leftBottom, HIGH);
            digitalWrite(rightTop, HIGH);
            digitalWrite(rightUpper, HIGH);
            digitalWrite(rightLower, LOW);
            digitalWrite(rightBottom, HIGH);
        }
        else
        {
            digitalWrite(rightTop, HIGH);
            digitalWrite(rightUpper, LOW);
            digitalWrite(rightLower, HIGH);
            digitalWrite(rightBottom, HIGH);
            digitalWrite(leftTop, HIGH);
            digitalWrite(leftUpper, HIGH);
            digitalWrite(leftLower, LOW);
            digitalWrite(leftBottom, HIGH);
        }
        //send(pattern[4], pattern[3]);
        //delay(25);
    }
    else if (accelerationX > 6000 || accelerationX < -6000) // head tilted to the left
    {
        if (accelerationX > 6000)
        {
            digitalWrite(leftTop, HIGH);
            digitalWrite(leftUpper, HIGH);
            digitalWrite(leftLower, LOW);
            digitalWrite(leftBottom, HIGH);
            digitalWrite(rightTop, HIGH);
            digitalWrite(rightUpper, LOW);
            digitalWrite(rightLower, HIGH);
            digitalWrite(rightBottom, HIGH);
        }
        else
        {
            digitalWrite(rightTop, HIGH);
            digitalWrite(rightUpper, HIGH);
            digitalWrite(rightLower, LOW);
            digitalWrite(rightBottom, HIGH);
            digitalWrite(leftTop, HIGH);
            digitalWrite(leftUpper, LOW);
            digitalWrite(leftLower, HIGH);
            digitalWrite(leftBottom, HIGH);
        }
        //send(pattern[0], pattern[2]);
        //delay(25);
    }
    else // head straight
    {
        digitalWrite(leftTop, LOW);
        digitalWrite(leftUpper, LOW);
        digitalWrite(leftLower, LOW);
        digitalWrite(leftBottom, LOW);
        digitalWrite(rightTop, LOW);
        digitalWrite(rightUpper, LOW);
        digitalWrite(rightLower, LOW);
        digitalWrite(rightBottom, LOW);
        //send(pattern[0], pattern[1]);
        //delay(25);
    }

    ///////////////////////////////////////
    ///////////////////////////////////////
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
    Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
    Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
    
    // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
    accelerationX = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
    accelerationY = Wire.read() << 8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
    accelerationZ = Wire.read() << 8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
    
    // print out data
    Serial.print("aX = "); Serial.print(convert_int16_to_str(accelerationX));
    Serial.print(" | aY = "); Serial.print(convert_int16_to_str(accelerationY));
    Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(accelerationZ));
    Serial.println("");
    // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30] 
    // delay
    delay(100);
    ///////////////////////////////////////
    ///////////////////////////////////////
}
