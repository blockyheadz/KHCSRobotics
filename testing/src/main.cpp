#include "main.h"

// Global objects for the controller and motors
pros::Controller master(pros::E_CONTROLLER_MASTER);

// Drivetrain motors
pros::Motor frontLeft(1);
pros::Motor middleLeft(2);
pros::Motor backLeft(3);
pros::Motor frontRight(4);
pros::Motor middleRight(5);
pros::Motor backRight(6);
pros::Motor intake(7);

// Mobile goal pneumatic system (updated with non-deprecated type)
pros::adi::DigitalOut mobileGoal('A');  // Fixed syntax error

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
    int speed = 100;    // Speed for autonomous movement
    int distance = 1000;  // Arbitrary tick value for distance

    // Move forward
    frontLeft.move_relative(distance, speed);
    middleLeft.move_relative(distance, speed);
    backLeft.move_relative(distance, speed);

    frontRight.move_relative(distance, speed);
    middleRight.move_relative(distance, speed);
    backRight.move_relative(distance, speed);

    // Wait until motors finish moving
    while (fabs(frontLeft.get_position()) < distance) {
        pros::delay(20);
    }

    // Turn in place
    int turnTicks = 500;  // Arbitrary tick value for turning
    frontLeft.move_relative(-turnTicks, speed);
    middleLeft.move_relative(-turnTicks, speed);
    backLeft.move_relative(-turnTicks, speed);

    frontRight.move_relative(turnTicks, speed);
    middleRight.move_relative(turnTicks, speed);
    backRight.move_relative(turnTicks, speed);

    // Wait until the turn is complete
    while (fabs(frontLeft.get_position()) < turnTicks) {
        pros::delay(20);
    }
}

// Teleoperated (Manual) Control

void opcontrol() {
   pros::Controller master (pros::E_CONTROLLER_MASTER);
   pros::Motor BackLeft (11);
   pros::Motor BackRight (-12);
   pros::Motor FrontLeft (13);
   pros::Motor FrontRight (-14);
   pros::Motor IntakeIntake (15);
   pros::Motor FloorIntake (16);
   
   int joystickright; 
   int joystickleft;
   int personalizedValue = 127;
   int leftPower = 0;
   int rightPower = 0;
   bool flagPersonalValuePressed = false;
   bool lowerIntakeOn = false;
   bool upperIntakeOn = false;

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
    if ( abs(joystickleft) > personalizedValue) {
	    rightPower = joystickleft - (int)(((127.0 - abs(joystickleft)) *joystickright) / 127);
            leftPower = joystickleft + (int)(((127.0 - abs(joystickleft)) * joystickright) / 127);

    } else {
	    
	    //This is when the personal preference starts to come in
	    if ( joystickright > 0) {
		    leftPower = joystickleft + (int)(((127 - joystickleft) / 127) * joystickright);
		    rightPower = joystickleft - (int)(((127 - personalizedValue) / 127) *joystickright);
	    } else {

		    rightPower = joystickleft - (int)(((127 - joystickleft) / 127) *joystickright);
		    leftPower = joystickleft + (int)(((127 - personalizedValue) / 127) * joystickright);
	    }
    }

    //This is the actual part that moves motors
    BackLeft.move(leftPower);
    FrontLeft.move(leftPower);

    BackRight.move(rightPower);
    FrontRight.move(rightPower);
    char value[] = "help please";

    sprintf(value,"%i   %b",personalizedValue, joystickleft < personalizedValue);
    pros::lcd::set_text(1,value);
	    
	    

      pros::delay(6);
     if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        upperIntakeOn = true;
     }
     if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        upperIntakeOn = false;
     }
     if (upperIntakeOn) {
        FloorIntake.move(127);
     }
     else {
        FloorIntake.move(0);
     }

     }

     

}
