#include "libServo.h"
#include "assert_print.h"

int main()
{
  wiringPiSetup()

Servo servo(1);

if (servo.m_pin!=1){assert_print("Pin for servo is not set to that which is assigned.")}
if (servo.m_pwmClock!=384){assert_print("The PWM clock is not set to 384.")}
if (servo.m_pwmRange!=1000){assert_print("The PWM range is not set to 1000.")}
if (m_clearAngle<55 || m_clearAngle>125){assert_print("Centre angle is not between 55 degress and 125 degrees (inclusive). Check 'calibration_values' file".)}
if (m_SepAngle<5 || m_SepAngle>45){assert_print("Separation angle is not between 5 degress and 45 degress (inclusive). Check 'calibration_values' file".)}
if ((m_clearAngle-m_SepAngle)<45 || (m_clearAngle+m_SepAngle)>135){assert_print("Range of motion of servo is not within 45 degress and 135 degress (inclusive). Check 'calibration_values' file".)}
if (m_HaltTime<200 || m_HaltTime>2000){assert_print("Halt time is not between 200ms and 2000ms (inclusive). Check 'calibration_values' file".)}
if (m_clearAngle==m_greenAngle || m_clearAngle==m_brownAngle || m_brownAngle==m_greenAngle)(assert_print("Output angles are calculated invalidly."))
servo.setRestPosition(9999);
if (servo.getRestPosition!=9999){assert_print("Getting or Setting of Rest Position dysfunctional.")}
servo.setHaltTime(9999);
if (servo.getHaltTime!=9999){assert_print("Getting or Setting of Halt Time dysfunctional.")}
servo.setSepAngle(9999);
if (servo.getSepAngle!=9999){assert_print("Getting or Setting of Separation Angle dysfunctional.")}

servo.saveCalibValues();

ifstream calibFile ("../include/calibration_values");
getline (calibFile,line);
m_clearAngle = std::stoi( line );
getline (calibFile,line);
m_HaltTime = std::stoi( line );
getline (calibFile,line);
m_SepAngle = std::stoi( line );
if (m_SepAngle!=9999 || m_clearAngle!=9999 || m_HaltTime!=9999){assert_print("Setting of calibration values failed")}

m_clearAngle=90;
m_SepAngle=30;
m_HaltTime=500;
servo.saveCalibValues();
}
