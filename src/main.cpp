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

//motor LF (PORT3,ratio6_1,true);
//motor LM (PORT2,ratio6_1,true);
motor LB (PORT1,ratio6_1,true);
motor RF (PORT13,ratio6_1,false);
motor RM (PORT12,ratio6_1,false);
motor RB (PORT11,ratio6_1,false);
motor Intake1(PORT1,ratio6_1);
motor Intake2(PORT2,ratio6_1,false);
bool R1=0;
bool R1 = Controller1.ButtonR1.pressing();
bool R2 = 0;
bool R2 = Controller1.ButtonR2.pressing();


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

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
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
void stopmotors (){
  //LF.setStopping(brake);
  //LM.setStopping(brake);
  LB.setStopping(brake);
  RF.setStopping(brake);
  RM.setStopping(brake);
  RB.setStopping(brake);
  
  //LF.stop();
  //LM.stop();
  LB.stop();
  RF.stop();
  RM.stop();
  RB.stop();
}
void usercontrol(void) {
  // User control code here, inside the loopo
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.
    int J3 = Controller1.Axis3.position(pct);
    int J1 = Controller1.Axis1.position(pct);

    //LF.spin(forward, J3+J1,pct);
    //LM.spin(forward, J3+J1,pct);
    LB.spin(forward, J3+J1,pct);
    RF.spin(forward, J3-J1,pct);
    RM.spin(forward, J3-J1,pct);
    RB.spin(forward, J3-J1,pct);

    while (R1==!R1){
      if (R1 = 1){
        Intake1.spin(forward,100,pct);
        Intake2.spin(forward,100,pct);
      }
      else {
        Intake1.stop();
        Intake2.stop();
      }
    }
    
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
