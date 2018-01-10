// Copyright 2016 AUV-IITK
#include <ros.h>
#include <Arduino.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float64.h>
#include <math.h>
#include <Wire.h>
#include "MS5837.h"

#define pwmPinWest 2
#define pwmPinEast 3
#define directionPinEast1 30
#define directionPinEast2 31
#define directionPinWest1 32
#define directionPinWest2 33

#define pwmPinNorthSway 4
#define pwmPinSouthSway 5
#define directionPinSouthSway1 27
#define directionPinSouthSway2 26
#define directionPinNorthSway1 29
#define directionPinNorthSway2 28

#define pwmPinNorthUp 6
#define pwmPinSouthUp 7
#define directionPinNorthUp1 24
#define directionPinNorthUp2 25
#define directionPinSouthUp1 22
#define directionPinSouthUp2 23

#define lowforward 110
#define highforward 205
#define lowsideward 145
#define highsideward 225
#define lowupward 0
#define highupward 0

#define pwmthrusterNorthUp_Low 0
#define pwmthrusterSouthUp_Low 0
#define pwmthrusterNorthUp_Mid 0
#define pwmthrusterSouthUp_Mid 0
#define pwmthrusterNorthUp_High 0
#define pwmthrusterSouthUp_High 0

#define pwmthrusterNorthSway_Low 109
#define pwmthrusterSouthSway_Low 138
#define pwmthrusterNorthSway_Mid 157
#define pwmthrusterSouthSway_Mid 200
#define pwmthrusterNorthSway_High 250
#define pwmthrusterSouthSway_High 254

#define pwmthrusterEast_Low 90
#define pwmthrusterWest_Low 55
#define pwmthrusterEast_Mid 180
#define pwmthrusterWest_Mid 128
#define pwmthrusterEast_High 255
#define pwmthrusterWest_High 237

#define analogPinPressureSensor A0

MS5837 sensor;


float last_pressure_sensor_value, pressure_sensor_value;
std_msgs::Float64 voltage;
ros::NodeHandle nh;

void thrusterNorthUp(int pwm, int isUpward)
{
  pwm = abs(pwm);
  if(pwm < lowupward)
  {
    analogWrite(pwmPinNorthUp, 255 - pwmthrusterNorthUp_Low);
  }
  else if(pwm > highupward)
  {
    analogWrite(pwmPinNorthUp, 255 - pwmthrusterNorthUp_High);
  }
  else
  {
    analogWrite(pwmPinNorthUp, 255 - pwmthrusterNorthUp_Mid);
  }

  if (isUpward)
  {
    digitalWrite(directionPinNorthUp1, HIGH);
    digitalWrite(directionPinNorthUp2, LOW);
  }
  else
  {
    digitalWrite(directionPinNorthUp1, LOW);
    digitalWrite(directionPinNorthUp2, HIGH);
  }
}

void thrusterSouthUp(int pwm, int isUpward)
{
  pwm = abs(pwm);
  if(pwm < lowupward)
  {
    analogWrite(pwmPinSouthUp, 255 - pwmthrusterSouthUp_Low);
  }
  else if(pwm > highupward)
  {
    analogWrite(pwmPinSouthUp, 255 - pwmthrusterSouthUp_High);
  }
  else
  {
    analogWrite(pwmPinSouthUp, 255 - pwmthrusterSouthUp_Mid);
  }

  if (isUpward)
  {
    digitalWrite(directionPinSouthUp1, HIGH);
    digitalWrite(directionPinSouthUp2, LOW);
  }
  else
  {
    digitalWrite(directionPinSouthUp1, LOW);
    digitalWrite(directionPinSouthUp2, HIGH);
  }
}

void thrusterNorthSway(int pwm, int isRight)
{
  pwm = abs(pwm);
  if(pwm < lowsideward && pwm!= 0)
  {
    analogWrite(pwmPinNorthSway, 255 - pwmthrusterNorthSway_Low);
  }
  else if(pwm > highsideward)
  {
    analogWrite(pwmPinNorthSway, 255 - pwmthrusterNorthSway_High);
  }
  else if(pwm > lowsideward && pwm < highsideward)
  {
    analogWrite(pwmPinNorthSway, 255 - pwmthrusterNorthSway_Mid);
  }
  else if(pwm == 0)
  {
    analogWrite(pwmPinNorthSway, 255 - pwm);
  }

  if (isRight)
  {
    digitalWrite(directionPinNorthSway1, HIGH);
    digitalWrite(directionPinNorthSway2, LOW);
  }
  else
  {
    digitalWrite(directionPinNorthSway1, LOW);
    digitalWrite(directionPinNorthSway2, HIGH);
  }
}

void thrusterSouthSway(int pwm, int isRight)
{
  pwm = abs(pwm);
  if(pwm < lowsideward && pwm != 0)
  {
    analogWrite(pwmPinSouthSway, 255 - pwmthrusterSouthSway_Low);
  }
  else if(pwm > highsideward)
  {
    analogWrite(pwmPinSouthSway, 255 - pwmthrusterSouthSway_High);
  }
  else if(pwm > lowsideward && pwm < highsideward)
  {
    analogWrite(pwmPinSouthSway, 255 - pwmthrusterSouthSway_Mid);
  }
  else if(pwm == 0)
  {
    analogWrite(pwmPinSouthSway, 255 - pwm);
  }

  if (isRight)
  {
    digitalWrite(directionPinSouthSway1, HIGH);
    digitalWrite(directionPinSouthSway2, LOW);
  }
  else
  {
    digitalWrite(directionPinSouthSway1, LOW);
    digitalWrite(directionPinSouthSway2, HIGH);
  }
}

void thrusterEast(int pwm, int isForward)
{
  pwm = abs(pwm);
  if(pwm < lowforward && pwm != 0)
  {
    analogWrite(pwmPinEast, 255 - pwmthrusterEast_Low);
  }
  else if(pwm > highforward)
  {
    analogWrite(pwmPinEast, 255 - pwmthrusterEast_High);
  }
  else if(pwm > lowforward && pwm < highforward)
  {
    analogWrite(pwmPinEast, 255 - pwmthrusterEast_Mid);
  }
  else if(pwm == 0)
  {
    analogWrite(pwmPinEast, 255 - pwm);
  }

  if (isForward)
  {
    digitalWrite(directionPinEast1, HIGH);
    digitalWrite(directionPinEast2, LOW);
  }
  else
  {
    digitalWrite(directionPinEast1, LOW);
    digitalWrite(directionPinEast2, HIGH);
  }
}

void thrusterWest(int pwm, int isForward)
{
  pwm = abs(pwm);
  if(pwm < lowforward && pwm != 0)
  {
    analogWrite(pwmPinWest, 255 - pwmthrusterWest_Low);
  }
  else if(pwm > highforward)
  {
    analogWrite(pwmPinWest, 255 - pwmthrusterWest_High);
  }
  else if(pwm > lowforward && pwm < highforward)
  {
    analogWrite(pwmPinWest, 255 - pwmthrusterWest_Mid);
  }
  else if(pwm == 0)
  {
    analogWrite(pwmPinWest, 255 - pwm);
  }

  if (isForward)
  {
    digitalWrite(directionPinWest1, HIGH);
    digitalWrite(directionPinWest2, LOW);
  }
  else
  {
    digitalWrite(directionPinWest1, LOW);
    digitalWrite(directionPinWest2, HIGH);
  }
}

void PWMCbForward(const std_msgs::Int32& msg)
{
  int pwm = msg.data;
  if (pwm > 0)
  {
    thrusterEast(pwm, true);
    thrusterWest(pwm, true);
  }
  else
  {
    thrusterEast(pwm, false);
    thrusterWest(pwm, false);
  }
}

void PWMCbSideward(const std_msgs::Int32& msg)
{
  int pwm = msg.data;
  if (pwm > 0)
  {
    thrusterNorthSway(pwm, true);
    thrusterSouthSway(pwm, true);
  }
  else
  {
    thrusterNorthSway(pwm, false);
    thrusterSouthSway(pwm, false);
  }
}

void PWMCbUpward(const std_msgs::Int32& msg)
{
  int pwm = msg.data;
  if (pwm > 0)
  {
    thrusterNorthUp(pwm, true);
    thrusterSouthUp(pwm, true);
  }
  else
  {
    thrusterNorthUp(pwm, false);
    thrusterSouthUp(pwm, false);
  }
}

void PWMCbTurn(const std_msgs::Int32& msg)
{
  int pwm = msg.data;
  //   if (pwm > 0)
  //   {
  //     thrusterEast(pwm, true);
  //     thrusterWest(pwm, false);
  //   }
  //   else
  //   {
  //     thrusterEast(pwm, false);
  //     thrusterWest(pwm, true);
  //   }
  // }
  // else
  // {
    if (pwm > 0)
    {
      thrusterNorthSway(pwm, true);
      thrusterSouthSway(pwm, false);
    }
    else
    {
      thrusterNorthSway(pwm, false);
      thrusterSouthSway(pwm, true);
    }
}

ros::Subscriber<std_msgs::Int32> subPwmForward("/pwm/forward", &PWMCbForward);
ros::Subscriber<std_msgs::Int32> subPwmSideward("/pwm/sideward", &PWMCbSideward);
ros::Subscriber<std_msgs::Int32> subPwmUpward("/pwm/upward", &PWMCbUpward);
ros::Subscriber<std_msgs::Int32> subPwmTurn("/pwm/turn", &PWMCbTurn);
ros::Publisher ps_voltage("/varun/sensors/pressure_sensor/depth", &voltage);

void setup()
{
  nh.initNode();
  Wire.begin();

  sensor.init();

  sensor.setFluidDensity(997);  // kg/m^3 (freshwater, 1029 for seawater)
  pinMode(pwmPinEast, OUTPUT);
  pinMode(directionPinEast1, OUTPUT);
  pinMode(directionPinEast2, OUTPUT);
  pinMode(pwmPinWest, OUTPUT);
  pinMode(directionPinWest1, OUTPUT);
  pinMode(directionPinWest2, OUTPUT);

  pinMode(directionPinSouthSway1, OUTPUT);
  pinMode(directionPinSouthSway2, OUTPUT);
  pinMode(pwmPinNorthSway, OUTPUT);
  pinMode(directionPinNorthSway2, OUTPUT);
  pinMode(pwmPinSouthSway, OUTPUT);
  pinMode(directionPinNorthSway1, OUTPUT);

  pinMode(directionPinSouthUp1, OUTPUT);
  pinMode(directionPinSouthUp2, OUTPUT);
  pinMode(pwmPinNorthUp, OUTPUT);
  pinMode(directionPinNorthUp2, OUTPUT);
  pinMode(pwmPinSouthUp, OUTPUT);
  pinMode(directionPinNorthUp1, OUTPUT);

  nh.subscribe(subPwmForward);
  nh.subscribe(subPwmSideward);
  nh.subscribe(subPwmUpward);
  nh.subscribe(subPwmTurn);
  nh.advertise(ps_voltage);

  Serial.begin(57600);
  std_msgs::Int32 msg;
  msg.data = 0;
  PWMCbForward(msg);
  PWMCbSideward(msg);
  PWMCbUpward(msg);
  PWMCbTurn(msg);

  sensor.read();
  last_pressure_sensor_value = -(sensor.depth() * 100);
}

void loop()
{
  sensor.read();
  // voltage.data made -ve because pressure sensor data should increase going up
  pressure_sensor_value = -(sensor.depth() * 100);
  // to avoid random high values
  if (abs(last_pressure_sensor_value - pressure_sensor_value) < 100)
  {
    voltage.data = 0.7 * pressure_sensor_value + 0.3 * last_pressure_sensor_value;
    ps_voltage.publish(&voltage);
    last_pressure_sensor_value = pressure_sensor_value;
  }

  delay(200);
  nh.spinOnce();
}
