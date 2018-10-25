#pragma config(Sensor, in1,    Arm_Angle,      sensorPotentiometer)
#pragma config(Sensor, I2C_1,  LeftEncoder,    sensorNone)
#pragma config(Sensor, I2C_2,  RightEncoder,   sensorNone)
#pragma config(Motor,  port2,           L_Front,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           R_Front,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           L_Arm,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           L_Rear,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           R_Rear,        tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           R_Arm,         tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//VEX COMPETITION INCLUDES
#pragma platform(VEX2)//VEX cortex platform
//#pragma competitionControl(Competition)	// Select Download method as "competition"
#include "Vex_Competition_Includes.c"	//do not modify

void pre_auton()	//You must return from this function or the autonomous and usercontrol tasks will not be started.
{
	bStopTasksBetweenModes = true;
}

/*				VARIABLES 	*/
//drive()
int L_POWER = 0;								//left drive power in drive()
int R_POWER = 0;								//right drive power in drive()
int MAX_POWER = 100;						//maximum power to motor - 127 is max
float DRIVE_SENSITIVITY = 0.8;		//coefficient = decrease joystick sensitivity
int DEADBAND = 12;							//Joystick seldom 0 when off so this is the value to ignore joystick 'noise' below
int ARM_POWER = 0;							//power to apply to arm if needed
float ARM_SENSITIVITY = 0.4;		//slow arm movements
int HOLD_ARM = 15;

//xxxCap() variables - flipArm OK 10-22 (250,50,150,50)
int FLIP_ARM_TIME = 400;		//time lift arm before drive foward
int FLIP_ARM_SPEED = 50;		//power to lift arm
int FLIP_DRIVE_TIME = 150;	//time before driving foward
int FLIP_DRIVE_SPEED = 50;	//drive power
int ARM_MAX = 2100;				//maximum height of arm (3500 max before risk damaging Arm_Angle potentiometer)
int ARM_CAP_HIGH = 2000;		// TBD - value of Arm_Angle potentioneter for placing high caps
int ARM_CAP_LOW = 1380;			//TBD - value of Arm_Angle potentioneter for placing high caps

// autonomous driving variables
int DRIVE_PAUSE = 200;
int BRAKE_TIME = 50;

/*			FUNCTIONS		*/
//	DRIVE BASED ON ENCODER 'TICKS'
void eDrive(int l, int r, int e){
	//reset encoders
	resetMotorEncoder(R_Front);
	resetMotorEncoder(L_Front);

	//drive & brake
	wait1Msec(DRIVE_PAUSE);		//brief pause
	while(abs(getMotorEncoder(R_Front)) < e)  {		//drive until encoder 'ticks' = e
		motor[R_Rear] = motor[R_Front]=r;
		motor[L_Rear] = motor[L_Front]=l;}
	wait1Msec(BRAKE_TIME);
	motor[R_Rear] = motor[R_Front]=0;		//shut motors off
	motor[L_Rear] = motor[L_Front]=0;
	wait1Msec(BRAKE_TIME);
	motor[R_Rear] = motor[R_Front]=r *-.5;	//briefly reversing motors acts like a brake
	motor[L_Rear] = motor[L_Front]=l * -.5;
	wait1Msec(BRAKE_TIME/2);					//shut motors off
	motor[R_Rear] = motor[R_Front]=0;
	motor[L_Rear] = motor[L_Front]=0;
}//end eDrive

//TIMED DRIVE
void tDrive(int l, int r, int t){
	wait1Msec(DRIVE_PAUSE);
	motor[R_Rear] = motor[R_Front]=r;
	motor[L_Rear] = motor[L_Front]=l;
	wait1Msec(t);
	motor[R_Rear] = motor[R_Front]=0;
	motor[L_Rear] = motor[L_Front]=0;
	wait1Msec(BRAKE_TIME);
	motor[R_Rear] = motor[R_Front]=r *-.5;
	motor[L_Rear] = motor[L_Front]=l * -.5;
	wait1Msec(BRAKE_TIME/2);
	motor[R_Rear] = motor[R_Front]=0;
	motor[L_Rear] = motor[L_Front]=0;
}//end tDrive

//DRIVE CONTROL
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

		motor[L_Front]= motor[L_Rear]=L_POWER;
		motor[R_Front]= motor[R_Rear]=R_POWER;
	}//end while
}//end drive

	//ARM HEIGHT
task arm(){
	while(true){
		if(vexRT[Ch3]> DEADBAND && SensorValue(Arm_Angle) < ARM_MAX){//limit height
			motor[L_Arm]=motor[R_Arm]=vexRT[Ch3] * ARM_SENSITIVITY;
		}
		else if(vexRT[Ch3] < (DEADBAND * -2)){
			motor[L_Arm]=motor[R_Arm]=vexRT[Ch3] * ARM_SENSITIVITY/4;
		}
		else{
			motor[L_Arm]=motor[R_Arm]=ARM_POWER;
		}

		//if arm is in air - keep from falling
		if(SensorValue(Arm_Angle) > ARM_CAP_LOW / 4){
			ARM_POWER = HOLD_ARM;
		}
		else {
			ARM_POWER = 0;
		}

	}//end while
}//end arm

//flip cap
void flipCap(void){
	if(SensorValue(Arm_Angle)< 300){
		//stop tasks that may interfere with actions
		stopTask(arm);
		stopTask(drive);

		//perform flip by..
		motor[L_Arm]=motor[R_Arm]=FLIP_ARM_SPEED;	//lift claw
		wait1Msec(FLIP_ARM_TIME);		//wait
		motor[L_Front]= motor[L_Rear]=FLIP_DRIVE_SPEED;	//drive forward to push cap
		motor[R_Front]= motor[R_Rear]=FLIP_DRIVE_SPEED;
		wait1Msec(FLIP_DRIVE_TIME);	//wait
		motor[L_Front]= motor[L_Rear]=0;	//stop
		motor[R_Front]= motor[R_Rear]=0;
		motor[L_Arm]=motor[R_Arm]=0;

		// lower claw TBD (added 10-23)
		motor[L_Arm]=motor[R_Arm]=FLIP_ARM_SPEED * -0.25 ;	//slowly lower claw
		wait1Msec((FLIP_ARM_TIME + FLIP_DRIVE_TIME));	//wait - probably too long
		motor[L_Arm]=motor[R_Arm]=0;

		//re-start tasks
		startTask(arm);
		startTask(drive);
	}//end if
}//end flipCap()

//position arm for placing cap on 36" post
task liftCap(){
	while(true){
		//lift cap for 36" post
		if(vexRT[Btn5U]==1){
			stopTask(arm);
			while(SensorValue(Arm_Angle) < ARM_CAP_HIGH){
				motor[L_Arm]=motor[R_Arm]=50;}	//lift claw
			motor[L_Arm]=motor[R_Arm]=0;
			startTask(arm);
		}// end btn 5U

		//lift cap for 24" post
		if(vexRT[Btn5D]==1){
			stopTask(arm);
			while(SensorValue(Arm_Angle) < ARM_CAP_LOW){
				motor[L_Arm]=motor[R_Arm]=50;}	//lift claw
			motor[L_Arm]=motor[R_Arm]=0;
			startTask(arm);
		}//end 5D

		/*		MICRO-ARM MOVEMENT */
		//up
		if(vexRT[Btn7U]==1){
			stopTask(arm);
			motor[L_Arm]=motor[R_Arm]=35;	//lift claw
			wait1Msec(200);
			motor[L_Arm]=motor[R_Arm]=0;
			startTask(arm);}//end 7U

		//down
		if(vexRT[Btn7D]==1){
			stopTask(arm);
			motor[L_Arm]=motor[R_Arm]=-8;	//lift claw
			wait1Msec(250);
			motor[L_Arm]=motor[R_Arm]=HOLD_ARM;
			startTask(arm);}	//end 7D

	}//end while
}//end liftCap

task autonomous(){
	tDrive(0,0,0); //timed driving distance: Left power, Right power, Time (ms)
	eDrive(0,0,0);//encoder driving distance: Left power, Right power, Time (ms)
}

task usercontrol(){
	//set-up tasks, sensors etc..
	resetMotorEncoder(L_Front);
	resetMotorEncoder(R_Front);
	startTask(drive);
	startTask(arm);
	startTask(liftCap);
	while (true){
		if(vexRT[Btn7L]==1){flipCap();}

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

		}//end while
}//end main
