#include "main.h"

// Global objects for controllers and motors
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor frontLeft(1); 
pros::Motor frontRight(4);  
pros::Motor backLeft(2);
pros::Motor backRight(5);
pros::Motor midLeft(3);
pros::Motor midRight(6);
pros::Motor intake(7); // Assuming intake is motor 7
pros::ADIDigitalOut mobileGoal('A');

// Arrays for drivetrain motors (left and right)
pros::Motor leftDrive[] = {frontLeft, midLeft, backLeft};
pros::Motor rightDrive[] = {frontRight, midRight, backRight};

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

// Autonomous Drivetrain Movement (Simple Example)
void autonomous() {
    int speed = 100; // Speed for autonomous movement
    int distance = 1000; // Arbitrary tick value for distance

    // Move forward for 'distance' ticks
    for (pros::Motor left : leftDrive) {
        left.move_relative(distance, speed);
    }
    for (pros::Motor right : rightDrive) {
        right.move_relative(distance, speed);
    }

    // Wait until motors are done moving
    while (fabs(leftDrive[0].get_position()) < distance) {
        pros::delay(20);
    }

    // Turn by moving only the right motors
    int turnTicks = 500;  // Arbitrary turn value
    for (pros::Motor left : leftDrive) {
        left.move_relative(-turnTicks, speed);
    }
    for (pros::Motor right : rightDrive) {
        right.move_relative(turnTicks, speed);
    }

    // Wait until the turn is complete
    while (fabs(leftDrive[0].get_position()) < turnTicks) {
        pros::delay(20);
    }
}

// Teleoperated (Manual) Control
void opcontrol() {
    bool mobileGoalToggle = false;
    bool mobileGoalPressed = false;
    while (true) {
        // Getting joystick input from the controller
        int forward = -1 * master.get_analog(ANALOG_RIGHT_X);  // Forward/Backward
        int turn =  master.get_analog(ANALOG_LEFT_Y);    // Turning

        // Display values on LCD (optional)
        char buffer[100];
        sprintf(buffer, "Forward: %d | Turn: %d", forward, turn);
        pros::lcd::set_text(1, buffer);

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
            if (!mobileGoalPressed) {
                if(mobileGoalToggle) {
                    mobileGoalToggle = false;
                } else {
                    mobileGoalToggle = true;
                }
                mobileGoalPressed = true;
            }
        } else {
            mobileGoalPressed = false;
        }
        mobileGoal.set_value(mobileGoalToggle);

        // Set left and right motor power for drivetrain
        for (pros::Motor left : leftDrive) {
            left.move(forward + turn);
        }
        for (pros::Motor right : rightDrive) {
            right.move(forward - turn);
        }

        // Intake control (example: controlled by controller buttons)
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            intake.move(100);  // Intake in
        } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            intake.move(-100);  // Intake out
        } else {
            intake.move(0);  // Stop intake
        }

        pros::delay(20);  // Run this loop every 20 milliseconds
    }
}
