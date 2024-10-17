#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() 
{
	
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
float const PI = 3.1415928;

//When width was set to 1000, it turned 95.5 times
float const ROBOT_WIDTH = 1000/95.5;

float inchToTick(float inch, float gearRatio, float wheelDia){
	return inch*((50 * gearRatio)/( wheelDia * 3.14159));
}

void rotateAroundCenter(float degree, pros::Motor bl, pros::Motor fl, pros::Motor br, pros::Motor fr) {
	float tick = inchToTick(PI * ROBOT_WIDTH * degree / 360, 18, 4);
	while(bl.get_position() < tick) {
		bl.move_absolute(tick, 33);
		fl.move_absolute(tick, 33);

		fr.move_absolute(tick, 33);
		br.move_absolute(tick, 33);
	}
	bl.tare_position();
	br.tare_position();

	fr.tare_position();
	fl.tare_position();
}

void opcontrol()
 {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor leftWheel(5);
	pros::Motor rightWheel(6);
	pros::Motor backRight(16);
	pros::Motor backLeft(15);

	float tick = inchToTick(PI * ROBOT_WIDTH, 18, 4);
	rotateAroundCenter(360, backLeft, leftWheel, backRight, rightWheel);

	while(backLeft.get_position() < tick) {
		leftWheel.move_absolute(tick, 33);
		backLeft.move_absolute(tick, 33);

		rightWheel.move_absolute(tick, 33);
		backRight.move_absolute(tick, 33);
		pros::delay(10);

	}

	for (int i = 0; i < 4; i++) {
		float tick = (inchToTick(PI * ROBOT_WIDTH, 18, 4) / 4);

		pros::lcd::initialize();
		char pos[] = "";
		sprintf(pos,"%i",i);
		pros::lcd::set_text(1,"It entered the loop");

		backLeft.tare_position();
		backRight.tare_position();

		leftWheel.tare_position();
		leftWheel.tare_position();

		while(backLeft.get_position() < inchToTick(120,18,4)) {
			leftWheel.move_absolute(inchToTick(120, 18, 4), 50);
			backLeft.move_absolute(inchToTick(120, 18, 4), 50);

			rightWheel.move_absolute(inchToTick(-120, 18, 4), 50);
			backRight.move_absolute(inchToTick(-120, 18, 4), 50);
			pros::delay(10);
			pros::lcd::set_text(1, "It is moving in straight line");
		}

		leftWheel.tare_position();
		backLeft.tare_position();

		rightWheel.tare_position();
		backRight.tare_position();

		while(backLeft.get_position() < tick) {
			leftWheel.move_absolute(tick, 30);
			backLeft.move_absolute(tick, 30);

			rightWheel.move_absolute(tick, 30);
			backRight.move_absolute(tick, 30);
			pros::delay(10);
			pros::lcd::set_text(1, "Stuck during turn");
		}
		pros::lcd::set_text(1,"Stuck after turn");
	}

}


