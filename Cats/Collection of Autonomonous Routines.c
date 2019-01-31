/*------------------------------------------------------------------------------------------------------*\
|*                                - ALLEY CAT AUTONOMOUS CODE -                  						*|
|*    Collection of autonomous routines 																*|
|*																										*|
|*		Start position autoA is furthest from flag, autoB nearest										*|
|*																										*|
\*----------------------------------------------------------------------------------------------------*/


void oneA(){/* 	autoA() - Autonomous from Position A  	-> cap, lift cap, reverse, turn 90, drop cap, back onto platform, lift claw	
	December 2018
	Standard motor gearing
*/

//AUTOA VARIABLES - Estimated 600mS per foot at FCMS
int autoA_fwd = 1550;	//time driving to the cap 1800
int autoA_liftCap = 500;	//how high to lift cap 500
int autoA_backup = 250;	//11-29 - MADE AUTOa BETTER how long to backup 300
int autoA_turn = 1800;	//how long you turn
int autoA_back2platform	= 1300;	//how long to park on platform

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

	tDrive(-50,-50,autoA_backup);	//backup

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
}//end one

void twoA(){/* 	autoA() - Autonomous from Position A  Palmetto win	 -> center, back onto platform, lift claw	
	December 2018
	Standard motor gearing
*/

//AUTOA VARIABLES
int autoA_fwd = 800;			//time driving to the flag (500Ms/ft)
int autoA_turn = 2050;			//how long to turn 90
int autoA_liftArm = 300;		//how high to lift claw to hit low flag
int autoA_back2platform	= 2200;	// back to platform 

void autoA(int direction){  
	//drive toward center
	tDrive(50,50,autoA_fwd);
	
	//turn right, toward platform
	tDrive(50*direction,-50*direction,autoA_turn);
	
	//while raising the arm to > 300
		while(SensorValue(Arm_Angle) < autoA_liftArm){		
		motor[L_Arm]=40;}	

	//hold arm up	
	motor[L_Arm]=10;
	
	//drive onto platform
	tDrive(120,120,autoA_back2platform);
}//end autoA
}//end twoA

void oneB(){/* 	autoB() - Autonomous from Position B 	raise claw, -> flag, back to center of platform, turn 90, back onto platform 	
	December 2018
	Standard motor gearing
*/

//AUTOB VARIABLES
int autoB_liftArm = 700;		//how high to lift claw to hit low flag
int autoB_fwd = 1850;			//time driving to the flag (500Ms/ft)
int autoB_backup = 3100;		//how long to backup to center of platform
int autoB_turn = 1800;			//how long to turn 90
int autoB_back2platform	= 2500;	// 11-30 2800 TOO  AFTER A WHILE OF TESTS, 2500 WORKED EARLIER TRY 2pt=TBD	4pt=2500 to middle platform w/out crossing line!!!

void autoB(int direction){  
	// POSITION CLAW
	while(SensorValue(Arm_Angle)<  autoB_liftArm){
		motor[L_Arm]=55;	//lift  claw
	}
	motor[L_Arm]=10;	//hold claw position
	
	//back-up to flag
	tDrive(-78,-70,autoB_fwd); //timed driving distance: Left power, Right power, Time (ms)

	//backup to platform center
	tDrive(50,50,autoB_backup);

	//turn so rear towards platform
	tDrive(55*direction,-60*direction,autoB_turn); //timed driving distance: Left power, Right power, Time (ms)

	// backup onto platform
	tDrive(-127,-127,autoB_back2platform);

	//lift arm to chang center of gravity?
	while(SensorValue(Arm_Angle) < ARM_CAP_HIGH + 150
			){
		motor[L_Arm]=90;}	//lift claw
	motor[L_Arm]=10;*/
	motor[L_Arm]=0;		//shut arm off for testing
	motor[L_Arm]=10;	//keep arm position
}//end autoB
}//end oneB