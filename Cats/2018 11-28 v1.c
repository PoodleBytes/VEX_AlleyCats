#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(Sensor, in1,    Arm_Angle,      sensorPotentiometer)
#pragma config(Sensor, in2,    Selector,       sensorPotentiometer)
#pragma config(Sensor, I2C_1,  LeftEncoder,    sensorNone)
#pragma config(Sensor, I2C_2,  RightEncoder,   sensorNone)
#pragma config(Motor,  port2,           L_Front,       tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port3,           R_Front,       tmotorVex393_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port4,           L_Arm,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           L_Rear,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           R_Rear,        tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           R_Arm,         tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//VEX COMPETITION INCLUDES
#pragma platform(VEX2)//VEX cortex platform
#pragma competitionControl(Competition)	// Select Download method as "competition"
#include "Vex_Competition_Includes.c"	//do not modify

void pre_auton()	//You must return from this function or the autonomous and usercontrol tasks will not be started.
{
	slaveMotor(R_Rear,R_Front);
	slaveMotor(L_Rear,L_Front);
	slaveMotor(R_Arm,L_Arm);
	bStopTasksBetweenModes = true;
}

/*				VARIABLES 	*/
//AUTOA VARIABLES - Estimated 600mS per foot at FCMS
int autoA_fwd = 1420;	//time driving to the cap 1800
int autoA_liftCap = 500;	//how high to lift cap 500
//int autoA_backup = 300;	//how long to backup 300
int autoA_turn = 1800;	//how long you turn
int autoA_back2platform	= 1300;	//how long to park on platform
//AUTOB VARIABLES
int autoB_liftArm = 500;	//how high to lift claw to hit low flag
int autoB_fwd = 1900;	//time driving to the flag (500Ms/ft)
int autoB_backup = 3000;	//how long to backup to center of platform
int autoB_turn = 1700;	//how long to turn 90 (1700 slightly too far)
int autoB_back2platform	= 2500;	// 2pt=TBD	4pt=2500 to middle platform w/out crossing line!!!

//drive()
int L_POWER = 0;								//left drive power in drive()
int R_POWER = 0;								//right drive power in drive()
int MAX_POWER = 120;						//maximum power to motor - 127 is max
float DRIVE_SENSITIVITY = 0.8;		//coefficient = decrease joystick sensitivity
int DEADBAND = 12;							//Joystick seldom 0 when off so this is the value to ignore joystick 'noise' below
int ARM_POWER = 0;							//power to apply to arm if needed
float ARM_SENSITIVITY = 0.4;		//slow arm movements
int HOLD_ARM = 18;

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

//TIMED DRIVE
void tDrive(int l, int r, int t){
	wait1Msec(DRIVE_PAUSE);
	motor[R_Front]=r;
	motor[L_Front]=l;
	wait1Msec(t);
	motor[R_Front]=0;
	motor[L_Front]=0;
	wait1Msec(BRAKE_TIME);
	motor[R_Front]=r *-.5;
	motor[L_Front]=l * -.5;
	wait1Msec(BRAKE_TIME/2);
	motor[R_Front]=0;
	motor[L_Front]=0;
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

		motor[L_Front]= L_POWER;
		motor[R_Front]=R_POWER;
	}//end while
}//end drive

//ARM HEIGHT
task arm(){
	while(true){
		if(vexRT[Ch3]> DEADBAND && SensorValue(Arm_Angle) < ARM_MAX){//limit height
			motor[L_Arm]=vexRT[Ch3] * ARM_SENSITIVITY;
		}
		else if(vexRT[Ch3] < (DEADBAND * -2) && SensorValue(Arm_Angle) < ARM_MAX){
			motor[L_Arm]=vexRT[Ch3] * ARM_SENSITIVITY/4;
		}
		else if(vexRT[Ch3] < (DEADBAND * -2) && SensorValue(Arm_Angle) > ARM_MAX){
			motor[L_Arm]=vexRT[Ch3] * ARM_SENSITIVITY;
		}
		else{
			motor[L_Arm]=ARM_POWER;
		}

		//if arm is in air - keep from falling
		if(SensorValue(Arm_Angle) > 250){
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
		motor[L_Arm]=FLIP_ARM_SPEED;	//lift claw
		wait1Msec(FLIP_ARM_TIME);		//wait
		motor[L_Front]= FLIP_DRIVE_SPEED;	//drive forward to push cap
		motor[R_Front]=FLIP_DRIVE_SPEED;
		wait1Msec(FLIP_DRIVE_TIME);	//wait
		motor[L_Front]= 0;	//stop
		motor[R_Front]=0;
		motor[L_Arm]=0;

		// lower claw TBD (added 10-23)
		motor[L_Arm]=FLIP_ARM_SPEED * -0.25 ;	//slowly lower claw
		wait1Msec((FLIP_ARM_TIME + FLIP_DRIVE_TIME));	//wait - probably too long
		motor[L_Arm]=0;

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
			while(SensorValue(Arm_Angle) < ARM_CAP_HIGH - 500){
				motor[L_Arm]=50;}	//lift claw
			while(SensorValue(Arm_Angle) < ARM_CAP_HIGH){
				motor[L_Arm]=40;}	//lift claw
			motor[L_Arm]=0;
			startTask(arm);
		}// end btn 5U

		//lift cap for 24" post
		if(vexRT[Btn5D]==1){
			stopTask(arm);
			while(SensorValue(Arm_Angle) < ARM_CAP_LOW){
				motor[L_Arm]=50;}	//lift claw
			motor[L_Arm]=0;
			startTask(arm);
		}//end 5D

		/*		MICRO-ARM MOVEMENT */
		//up
		if(vexRT[Btn7U]==1){
			stopTask(arm);
			motor[L_Arm]=35;	//lift claw
			wait1Msec(200);
			motor[L_Arm]=0;
			startTask(arm);}//end 7U

		//down
		if(vexRT[Btn7D]==1){
			stopTask(arm);
			motor[L_Arm]=-8;	//lift claw
			wait1Msec(250);
			motor[L_Arm]=HOLD_ARM;
			startTask(arm);}	//end 7D

	}//end while
}//end liftCap

	/*  autoA() - Autonomous from Position A  	-> cap, lift cap, reverse, turn 90, drop cap, back onto platform, lift claw	*/
void autoA(int direction){  //directi0n = 1 blue, -1 = red side
	// UNFOLD CLAW & POSITION SO IT DOESN'T DRAG ON MAT
	int Arm_Start = SensorValue(Arm_Angle);
	while(SensorValue(Arm_Angle)<400){
		motor[L_Arm]=75;	//lift  claw
	}
	while(SensorValue(Arm_Angle)>Arm_Start + 100){
		motor[L_Arm]=-35;	//lower  claw
	}
	while(SensorValue(Arm_Angle)<Arm_Start + 15){
		motor[L_Arm]=55;	//lift  claw
	}
	motor[L_Arm]=10;	//hold claw position

	//drive to cap on ball
	tDrive(78,70,autoA_fwd); //timed driving distance: Left power, Right power, Time (ms)

	//lift cap
	while(SensorValue(Arm_Angle) < autoA_liftCap){
		motor[L_Arm]=40;}	//lift claw
	motor[L_Arm]=15;

//	tDrive(-50,-50,autoA_backup);	//backup

	//turn
	tDrive(-65*direction,65*direction,autoA_turn); //timed driving distance: Left power, Right power, Time (ms)

	//lower cap
	while(SensorValue(Arm_Angle) > Arm_Start){
		motor[L_Arm]=-50;}	//lower claw
	motor[L_Arm]=0;

	// backup
	tDrive(-120,-120,autoA_back2platform);

	while(SensorValue(Arm_Angle) < 300){
		motor[L_Arm]=40;}	//lift claw
	motor[L_Arm]=10;// end of autoA  */
}//end autoA

/*  autoB() - Autonomous from Position B 	raise claw, -> flag, back to center of platform, turn 90, back onto platform 	*/
void autoB(int direction){  //directi0n = 1 blue, -1 = red side
	/*  Autonomous from Position A 	*/
	// POSITION CLAW
	int Arm_Start = SensorValue(Arm_Angle);

	while(SensorValue(Arm_Angle)<  autoB_liftArm){
		motor[L_Arm]=55;	//lift  claw
	}
	motor[L_Arm]=10;	//hold claw position

	//drive to flag
	tDrive(78,70,autoB_fwd); //timed driving distance: Left power, Right power, Time (ms)

	//backup to platform center
	tDrive(-50,-50,autoB_backup);

	//turn so rear towards platform
	tDrive(65*direction,-65*direction,autoB_turn); //timed driving distance: Left power, Right power, Time (ms)

	// backup onto platform
	tDrive(-120,-120,autoB_back2platform);

	//arm power - keep position (competition?) or shut off (testing)
	motor[L_Arm]=0;		//shut arm off for testing
	//motor[L_Arm]=10;	//keep arm position
}//end autoB

	/* 		autonomous()		*/
task autonomous(){
	int autoSelect = SensorValue[Selector];	//read auto potentiometer position  0 2000 blue
	//NEED REAL WORLD POT VALUES FOR autoA blue / red autoB blue / red
	//below assumes autoPot's range is 1000 to 3000 with 2000 being the selector is pointing straight-up (select NO autonomous)

	if(autoSelect <=450){		//autoB RED
		autoB(-1);}
	else if (autoSelect >450 && autoSelect <=1500){	//autoA RED
		autoA(-1);}
	else if (autoSelect >1500 && autoSelect <=2200){	//NO Autonomous
		return;}
	else if (autoSelect >2200 && autoSelect <=3000){	//autoA BLUE
		autoA(1);}
	else if(autoSelect >3000){						//autoB BLUE
		autoB(1);}
	else{															//catch all
		return;}//neutral position
} //end autonomous()

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
