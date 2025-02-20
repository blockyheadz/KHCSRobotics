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
    bool mobileGoalToggle = false;
    bool mobileGoalPressed = false;

    while (true) {
        // Getting joystick input from the controller
        int forward = master.get_analog(ANALOG_LEFT_Y);   // Forward/backward
        int turn = master.get_analog(ANALOG_RIGHT_X);     // Turning

        // Ensure values are within motor speed range (-127 to 127)
        int leftPower = std::max(-127, std::min(127, forward + turn));
        int rightPower = std::max(-127, std::min(127, forward - turn));

        // Set motor power for drivetrain
        frontLeft.move(leftPower);
        middleLeft.move(leftPower);
        backLeft.move(leftPower);

        frontRight.move(rightPower);
        middleRight.move(rightPower);
        backRight.move(rightPower);

        // Mobile Goal Lift Control (Button Y)
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
            if (!mobileGoalPressed) {
                mobileGoalToggle = !mobileGoalToggle;  // Toggle state
                mobileGoalPressed = true;
            }
        } else {
            mobileGoalPressed = false;
        }
        mobileGoal.set_value(mobileGoalToggle);

        // Intake Control (Buttons R1 and R2)
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            intake.move(100);  // Intake in
        } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            intake.move(-100);  // Intake out
        } else {
            intake.move(0);  // Stop intake
        }

        // Optional Debugging on LCD
        char buffer[100];
        sprintf(buffer, "Forward: %d | Turn: %d", forward, turn);
        pros::lcd::set_text(1, buffer);

        pros::delay(20);  // Run this loop every 20 milliseconds
    }
}
