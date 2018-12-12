#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    arm_angle,      sensorPotentiometer)
#pragma config(Sensor, in2,    cap_flip_angle, sensorPotentiometer)
#pragma config(Sensor, in3,    red_blue_sw,    sensorPotentiometer)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           launch_ball,   tmotorVex393TurboSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           drive_right,   tmotorVex393HighSpeed_MC29, openLoop, reversed, driveLeft, encoderPort, I2C_1)
#pragma config(Motor,  port3,           drive_left,    tmotorVex393HighSpeed_MC29, openLoop, driveRight, encoderPort, I2C_2)
#pragma config(Motor,  port4,           lift_left,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           lift_right,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           cap_flip_left, tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           cap_flip_right, tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           robot_lift,    tmotorVex393_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)


//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton()
{
 slaveMotor(lift_right, lift_left);
 slaveMotor(cap_flip_right, cap_flip_left);
  bStopTasksBetweenModes = true;

}
/*      VARIABLES     */
int L_POWER = 0;								//left drive power in drive()
int R_POWER = 0;								//right drive power in drive()
int MAX_POWER = 100;						//maximum power to motor - 127 is max
float DRIVE_SENSITIVITY = 0.8;		//coefficient = decrease joystick sensitivity
int DEADBAND = 5;							//Joystick seldom 0 when off so this is the value to ignore joystick noise
int ARM_POWER = 0;							//power to apply to arm if needed
float ARM_SENSITIVITY = 0.4;		//slow arm movements
int HOLD_ARM = 15;

//xxxCap() variables - flipArm OK 10-22 (250,50,150,50)
int FLIP_ARM_TIME = 400;		//time lift arm before drive foward
int FLIP_ARM_SPEED = 50;		//power to lift arm
int FLIP_DRIVE_TIME = 150;	//time before driving foward
int FLIP_DRIVE_SPEED = 50;	//drive power
int ARM_MAX = 1750;				//maximum height of arm (1600 max top
int ARM_CAP_HIGH = 1800;		// value of Arm_Angle potentioneter for placing high caps
int ARM_CAP_LOW = 2200;   // value of Arm_Angle potentioneter for placing high caps
int CAP_FLIP_DOWN = 2000; // lowered value of cap flip frame
int HOLD_CAP_FLIP = 15;
// autonomous driving variables
int DRIVE_PAUSE = 200;
int BRAKE_TIME = 50;

// DRIVE CONTROL
task drive(){
	while(true){
		// calculate arcade drive motor[ power
		if(abs((vexRT[Ch2] + vexRT[Ch1]))>DEADBAND){
			L_POWER =(vexRT[Ch2] + vexRT[Ch1])* DRIVE_SENSITIVITY;
			R_POWER =(vexRT[Ch2] - vexRT[Ch1]) * DRIVE_SENSITIVITY;
		}
		else {
			L_POWER = 0;
			R_POWER = 0;
		}

		// limit motor power to MAX_POWER
		if(abs(R_POWER) > MAX_POWER)
			R_POWER = sgn(R_POWER) * MAX_POWER;
		if(abs(L_POWER) > MAX_POWER )
			L_POWER = sgn(L_POWER) * MAX_POWER;

		motor[drive_left]= L_POWER;
		motor[drive_right]=R_POWER;
	}//end while
}//end drive

//ARM HEIGHT
task arm(){
	while(true){
		if(vexRT[Ch3]> DEADBAND && SensorValue(arm_angle) > ARM_MAX){//limit height
			motor[lift_left]=vexRT[Ch3] * ARM_SENSITIVITY;
		}
		else if(vexRT[Ch3] < (DEADBAND * -2) && SensorValue(arm_angle) > ARM_MAX){
			motor[lift_left]=vexRT[Ch3] * ARM_SENSITIVITY;
		}
		else if(vexRT[Ch3] < (DEADBAND * -2) && SensorValue(arm_angle) < ARM_MAX){
			motor[lift_left]=vexRT[Ch3] * ARM_SENSITIVITY;
		}
		else{
			motor[lift_left]=ARM_POWER;
		}

		//if arm is in air - keep from falling
		if(SensorValue(arm_angle) < 2000){
			ARM_POWER = HOLD_ARM;
		}
		else {
			ARM_POWER = 10;
		}

	}//end while
}//end arm


//TIMED DRIVE
void tDrive(int l, int r, int t){
	wait1Msec(DRIVE_PAUSE);
	motor[drive_right]=r;
	motor[drive_left]=l;
	wait1Msec(t);
	motor[drive_right]=0;
	motor[drive_left]=0;
	wait1Msec(BRAKE_TIME);
	motor[drive_right]=r *-.5;
	motor[drive_left]=l * -.5;
	wait1Msec(BRAKE_TIME/2);
	motor[drive_right]=0;
	motor[drive_left]=0;
}//end tDrive


//flip cap
void flipCap(void){
	if(SensorValue(arm_angle)> 4000){
		//stop tasks that may interfere with actions
		stopTask(arm);
		stopTask(drive);

		//perform flip by..
		motor[cap_flip_left]=FLIP_ARM_SPEED;	//lift claw
		wait1Msec(FLIP_ARM_TIME);		//wait
		motor[drive_left]= FLIP_DRIVE_SPEED;	//drive forward to push cap
		motor[drive_right]=FLIP_DRIVE_SPEED;
		wait1Msec(FLIP_DRIVE_TIME);	//wait
		motor[drive_left]= 0;	//stop
		motor[drive_right]=0;
		motor[cap_flip_left]=0;

		// lower claw TBD (added 10-23)
		motor[cap_flip_left]=FLIP_ARM_SPEED * -0.25 ;	//slowly lower claw
		wait1Msec((FLIP_ARM_TIME + FLIP_DRIVE_TIME));	//wait - probably too long
		motor[cap_flip_left]=0;

		//re-start tasks
		startTask(arm);
		startTask(drive);
	}//end if
}//end flipCap()

void autoA(int direction){
//direction potentiometer swith 1 = blue side  -1 = red side
  //autonomous pos A
	tDrive(60,60,1600);{
	motor[cap_flip_left]=-30;}//move cap off ball
	tDrive(-50,-50,300);//backup
	tDrive(-60*direction,60*direction,1800);//turn
	tDrive(50,50,250);
}


//position arm for placing cap on 36" post
task liftCap(){
	while(true){
		//lift cap for 36" post
		if(vexRT[Btn5U]==1){
			stopTask(arm);
			motor[cap_flip_left]=-15;
			while(SensorValue(arm_angle) > ARM_CAP_HIGH){
				motor[lift_left]=50;}	//lift arm up to high post
		  	motor[lift_left]=0;
			startTask(arm);
		}// end btn 5U

		//lift cap for 24" post
		if(vexRT[Btn5D]==1){
			stopTask(arm);
			motor[cap_flip_left]=-15;
		 while(SensorValue(arm_angle) > ARM_CAP_LOW){
				motor[lift_left]=70;}	//lift arm up to low post
				motor[cap_flip_left]=-10;
			motor[lift_left]= 0;
			startTask(arm);
		}//end 5D

		/*		CAP FLIP     */

		if(vexRT[Btn6U]==1){
			stopTask(arm);
			motor[cap_flip_left]=-90;	//lift frame
			wait1Msec(100);
			motor[cap_flip_left]= 0;
			startTask(arm);
			}	//end 6U

		//down sets to neutral scoop position
		if(vexRT[Btn6D]==1){
			stopTask(arm);
			motor[lift_left]=-30;
			while(SensorValue(cap_flip_angle) > CAP_FLIP_DOWN){
			motor[cap_flip_left]=50;}	//lower frame
			wait1Msec(100);
			motor[cap_flip_left]=0;
			startTask(arm);
			}	//end 7U

	}//end while
}//end liftCap


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous()
{	if(SensorValue[red_blue_sw] <2000){//blue
	autoA(1);}
  else{autoA(-1);}//red

  // ..........................................................................
  // Insert user code here.
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
task usercontrol(){
	//set-up tasks, sensors etc..
	resetMotorEncoder(drive_left);
	resetMotorEncoder(drive_right);
	startTask(drive);
	startTask(arm);
	startTask(liftCap);
	while (true){


		/*	MICRO-DRIVE */
		if(vexRT[Btn8U]==1){//straight
			stopTask(drive);
			tDrive(25,25,150);
			startTask(drive);}
		if(vexRT[Btn8D]==1){	//reverse
			stopTask(drive);
			tDrive(-25,-25,150);
			startTask(drive);}
		if(vexRT[Btn8L]==1){	//left
			stopTask(drive);
			tDrive(-30,30,150);
			startTask(drive);}
		if(vexRT[Btn8R]==1){	//right
			stopTask(drive);
			tDrive(30,-30,150);
			startTask(drive);}

			/* ROBOT-LIFT FOR PARKING  */

			if(vexRT[Btn7U]==1){// lift
				stopTask(arm);
				motor[robot_lift]=90;
				startTask(arm);}
			if(vexRT[Btn7D]==1){
				stopTask(arm);
				motor[robot_lift]=-90;
				startTask(arm);}
			else{
				motor[robot_lift]=-10;}




		}//end while
}//end main
