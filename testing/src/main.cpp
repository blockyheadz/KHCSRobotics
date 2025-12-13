#include "main.h"

// Global objects for the controller and motors
pros::Controller master(pros::E_CONTROLLER_MASTER);

// Drivetrain motors
pros::Motor BackLeft (11);
pros::Motor BackRight (-12);
pros::Motor FrontLeft (13);
pros::Motor FrontRight (-14);
pros::Motor IntakeIntake (-15);
pros::Motor FloorIntake (-16);

// Mobile goal pneumatic system (updated with non-deprecated type)
pros::adi::DigitalOut mobileGoal('A');  // Fixed syntax error




float inchToTick (float input) {
    int tick = 900; // 900 ticks per rev
    float inch = 12.60; // inch per revolution
    return ( tick/inch * input);
}

int rotateByDegree(int input) {
    int CHANGE_THIS_LATER = 1;
    return input * (500 / CHANGE_THIS_LATER);
}

void resetMotor() {
	FrontLeft.tare_position();
	BackLeft.tare_position();
	FrontRight.tare_position();
	BackRight.tare_position();
}


// Center button callback for LCD display
void on_center_button() {
    static bool pressed = false;
    pressed = !pressed;
    if (pressed) {
        pros::lcd::set_text(2, "I was pressed!");
    } else {
        pros::lcd::clear_line(2);
    }
}

// Initialization
void initialize() {
    pros::lcd::initialize();
    pros::lcd::set_text(1, "Hello PROS User!");
    pros::lcd::register_btn1_cb(on_center_button);
}

// Disabled mode function
void disabled() {}

// Competition initialization function
void competition_initialize() {}

// Autonomous Mode
void autonomous() {
	/*
    int speed = 100;    // Speed for autonomous movement
    int distance = inchToTick(34);  // Arbitrary tick value for distance
    
    while (FrontLeft.get_position() <  distance) {
	    FrontLeft.move_absolute(distance, speed);
	    FrontRight.move_absolute(distance, speed);
	    BackLeft.move_absolute(distance, speed);
	    BackRight.move_absolute(distance, speed);
	    pros::delay(10);
    }    

    resetMotor();


    distance = rotateByDegree(90);

    while (FrontRight.get_position() < distance) {
	    FrontLeft.move_absolute(-distance, speed);
	    FrontRight.move_absolute(distance, speed);
	    BackLeft.move_absolute(-distance, speed);
	    BackRight.move_absolute(distance, speed);
	    pros::delay(10);
    }


    resetMotor();

    distance = inchToTick(18);

    while (FrontRight.get_position() < distance) {
	    FrontLeft.move_absolute(distance, speed);
	    FrontRight.move_absolute(distance, speed);
	    BackLeft.move_absolute(distance, speed);
	    BackRight.move_absolute(distance, speed);
	    pros::delay(6);
    }


    resetMotor();

    IntakeIntake.move_absolute(inchToTick(10), speed);
    FloorIntake.move_absolute(inchToTick(10), speed);
    // Wait until motors finish moving
    distance = -1 * inchToTick(39);
    // Wait until the turn is complete

    while (FrontLeft.get_position() < distance) {
	    FrontLeft.move_absolute(distance, speed);
	    FrontRight.move_absolute(distance, speed);
	    BackLeft.move_absolute(distance, speed);
	    BackRight.move_absolute(distance, speed);
	    pros::delay(6);
    }


    FloorIntake.move_absolute(inchToTick(20), speed);
    IntakeIntake.move_absolute(inchToTick(30), speed);
    pros::delay(3000);
    */
}
//pls no break

void opcontrol() {
   pros::lcd::initialize();
   pros::Controller master (pros::E_CONTROLLER_MASTER);
   pros::Motor BackLeft (11);
   pros::Motor BackRight (-12);
   pros::Motor FrontLeft (13);
   pros::Motor FrontRight (-14);
   pros::Motor IntakeIntake (-15);
   pros::Motor FloorIntake (-16);
   
   int joystickright; 
   int joystickleft;
   int personalizedValue = 127;
   int leftPower = 0;
   int rightPower = 0;
   bool flagPersonalValuePressed = false;
   bool lowerIntakeOn = false;
   bool upperIntakeOn = false;


  int speed = 100;
  int distance = rotateByDegree(360);
  /**
    while (FrontRight.get_position() < distance) {
	    FrontLeft.move_absolute(distance, speed);
	    FrontRight.move_absolute(-1 * distance, speed);
	    BackLeft.move_absolute(distance, speed);
	    BackRight.move_absolute(-1 * distance, speed);
	    pros::delay(6);
    }

*/
   while (true) {
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        lowerIntakeOn = true;
    }
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        lowerIntakeOn = false;
    }
    if (lowerIntakeOn) {
        FloorIntake.move(127);
    } else {
        FloorIntake.move(0);
    }

    joystickleft = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    joystickright = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    
    //This sets the personal preference of the player
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) && !flagPersonalValuePressed && personalizedValue < 127) {
	    personalizedValue = personalizedValue + 1;
	    flagPersonalValuePressed = true;
    }
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) && !flagPersonalValuePressed && personalizedValue > 0) {
	    personalizedValue = personalizedValue - 1;
	    flagPersonalValuePressed = true;
    }

    if (!master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
	    flagPersonalValuePressed = false;
    }

       // This manages the actual power values
       // When within range for normal operations
    rightPower = (int)(((127.0 - abs(joystickright)) * joystickleft) / 127) - joystickright;
    leftPower = (int)(((127.0 - abs(joystickright)) * joystickleft) / 127) + joystickright;


    //This is the actual part that moves motors
    BackLeft.move(leftPower);
    FrontLeft.move(leftPower);

    BackRight.move(rightPower);
    FrontRight.move(rightPower);
    char value[] = "help please";

    sprintf(value,"%i   %b",personalizedValue, joystickleft < personalizedValue);
	    
	    

     if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        upperIntakeOn = true;
     }
     if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        upperIntakeOn = false;
     }
     if (upperIntakeOn) {
        IntakeIntake.move(127);
	pros::lcd::set_text(1,"it should be spinning");
     }
     else {
        IntakeIntake.move(0);
     }

     }

   pros::delay(10); 

}

