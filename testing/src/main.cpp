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
   pros::Motor BackRight (12);
   pros::Motor FrontLeft (13);
   pros::Motor FrontRight (14);
   pros::Motor IntakeIntake (15);
   pros::Motor FloorIntake (16);
   int power;
   int joystickleft;
   while (true) {
    joystickleft = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    power = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
      BackLeft.move(joystickleft);
      pros::delay(6);
      BackRight.move(-1*power);
      pros::delay(6);
      FrontLeft.move(joystickleft);
      pros::delay(6);
      FrontRight.move(-1*power);
      pros::delay(6);
      if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
        FloorIntake.move(127);

      }
     else {
        FloorIntake.move(0);
    }
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
        IntakeIntake.move(-127);
    }
    else {
        IntakeIntake.move(0);
    }
    }
     

}

