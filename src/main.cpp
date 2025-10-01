/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       dominiqueoppenheimer                                      */
/*    Created:      6/2/2025, 10:53:02 AM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
 
#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

controller Controller1 (primary);

motor LF (PORT3,ratio6_1,true);
motor LM (PORT2,ratio6_1,true);
motor LB (PORT1,ratio6_1,true);
motor RF (PORT13,ratio6_1,false);
motor RM (PORT12,ratio6_1,false);
motor RB (PORT11,ratio6_1,false);
motor Intake1(PORT5,ratio6_1);
motor Intake2(PORT14,ratio6_1, true);
motor Ramp2(PORT15,ratio6_1,false);
motor Ramp1(PORT16,ratio6_1,false);
inertial Inertial_Sensor (PORT10);
bool R1_old;
bool R2_old;
bool L1_old;
bool toggle;
bool toggleR;
bool toggleL;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
 Inertial_Sensor.calibrate();
  while (Inertial_Sensor.isCalibrating()) {
    wait(100, msec);
  }
  /*Intake1.setTimeout(1,sec);
  Intake2.setTimeout(1,sec);*/
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

float diameter=3.25;
float circumference=M_PI*diameter;
float gear_ratio=3./4.;

void Drive (int distance, int speed){
  float angle;
  angle=360*distance/(circumference*gear_ratio);
  LF.spinFor (forward,angle,deg,speed,velocityUnits::pct, false);
  LM.spinFor (forward,angle,deg,speed,velocityUnits::pct, false);
  LB.spinFor (forward,angle,deg,speed,velocityUnits::pct, false);
  RF.spinFor (forward,angle,deg,speed,velocityUnits::pct, false);
  RM.spinFor (forward,angle,deg,speed,velocityUnits::pct, false);
  RB.spinFor (forward,angle,deg,speed,velocityUnits::pct, true);
}
void stopmotors (){
  LF.setStopping(brake);
  LM.setStopping(brake);
  LB.setStopping(brake);
  RF.setStopping(brake);
  RM.setStopping(brake);
  RB.setStopping(brake);
  
  LF.stop();
  LM.stop();
  LB.stop();
  RF.stop();
  RM.stop();
  RB.stop();
}
void Turn (float angle){

  int top_speed=45;
  float speed;
  float error=180;
  float kp=0.5;
  while (fabs(error)>1.0){
    error=angle-Inertial_Sensor.rotation(deg);
    speed=kp*error;

    if (speed>top_speed)speed=top_speed;
    if (speed<-top_speed)speed=-top_speed;

    LF.spin(forward,speed,pct);
    LM.spin(forward,speed,pct);
    LB.spin(forward,speed,pct);
    RF.spin(forward,-speed,pct);
    RM.spin(forward,-speed,pct);
    RB.spin(forward,-speed,pct);
    
    wait (10,msec);
  }
  stopmotors();
 Inertial_Sensor.resetRotation();
}


void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  Drive (30,75);
  Turn (-90);
  Intake1.spin(forward,100,pct);
  Intake2.spin(forward,100,pct);
  Ramp1.spin(forward,50,pct);
  Ramp2.spin(forward,50,pct);
  Drive (9,25);
  wait (1,sec);
  Intake1.stop();
  Intake2.stop();
  Ramp1.stop();
  Ramp2.stop();
  Drive (-30,25);
  wait (150,msec);
  Intake1.spin(forward,100,pct);
  Intake2.spin(forward,100,pct);
  Ramp1.spin(forward,100,pct);
  Ramp2.spin(forward,100,pct);
  wait (3,sec);
  Drive (8,75);
  Turn (-90);
  Drive (22,75); 
  Turn (-90);
  Drive (18,25);
  Ramp2.stop();
  Ramp1.stop();
  Turn (48);
  Drive (16,45);
  Ramp2.spin(forward,100,pct);
  Ramp1.spin(reverse,100,pct);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.
    int J3 = Controller1.Axis3.position(pct);
    int J1 = 0.65*Controller1.Axis1.position(pct);

    bool R1 = Controller1.ButtonR1.pressing();
    bool R2 = Controller1.ButtonR2.pressing();
    bool BY = Controller1.ButtonY.pressing();
    bool L1 = Controller1.ButtonL1.pressing();

    LF.spin(forward, J3+J1,pct);
    LM.spin(forward, J3+J1,pct);
    LB.spin(forward, J3+J1,pct);
    RF.spin(forward, J3-J1,pct);
    RM.spin(forward, J3-J1,pct);
    RB.spin(forward, J3-J1,pct);

    if (R1 && !R1_old){
      toggle = !toggle;
      if (toggle){
        Ramp1.spin(forward,100,pct);
        Ramp2.spin(forward,100,pct);
        Intake1.spin(forward,100,pct);
        Intake2.spin(forward,100,pct);
      } else {
        Ramp1.stop();
        Ramp2.stop();
        Intake1.stop();
        Intake2.stop();
      }
    }
    R1_old=R1;

    if (R2 && !R2_old){
      toggleR = !toggleR;
      if (toggleR){
        Ramp1.spin(reverse,100,pct);
        Ramp2.spin(forward,100,pct);
        Intake1.spin(forward,100,pct);
        Intake2.spin(forward,100,pct);
      } else {
        Ramp1.stop();
        Ramp2.stop();
        Intake1.stop();
        Intake2.stop();
      }
    }
    R2_old=R2;

   if (L1 && !L1_old){
      toggleL = !toggleL;
      if (toggleL){
        Ramp1.spin(reverse,100,pct);
        Ramp2.spin(reverse,100,pct);
        Intake1.spin(reverse,100,pct);
        Intake2.spin(reverse,100,pct);
      } else {
        Ramp1.stop();
        Ramp2.stop();
        Intake1.stop();
        Intake2.stop();
      }
    }
    L1_old=L1;

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
