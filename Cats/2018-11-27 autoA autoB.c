/*				VARIABLES 	*/
//AUTOA VARIABLES - Estimated 600mS per foot at FCMS
int autoA_fwd = 1800;	//time driving to the cap 1800
int autoA_liftCap = 500;	//how high to lift cap 500
int autoA_backup = 300;	//how long to backup 300
int autoA_turn = 1800;	//how long you turn
int autoA_back2platform	= 1300;	//how long to park on platform
//AUTOB VARIABLES
int autoB_liftCap = 500;	//how high to lift claw to hit  low flag
int autoB_fwd = 1900;	//time driving to the flag (500Ms/ft)
int autoB_backup = 3050;	//how long to backup to center of platform
int autoB_turn = 1700;	//how long to turn 90 (1700 slightly too far)
int autoB_back2platform	= 2500;	// 2pt=TBD	4pt=2500 to middle platform w/out crossing line!!!


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
	/*  autoB() - Autonomous from Position B 	raise claw, -> flag, back to center of platform, turn 90, back onto platform 	*/
void autoB(int direction){  //directi0n = 1 blue, -1 = red side
	/*  Autonomous from Position A 	*/
	// POSITION CLAW 
	int Arm_Start = SensorValue(Arm_Angle);
	while(SensorValue(Arm_Angle)<400){
		motor[L_Arm]=75;	//lift  claw
	}
	while(SensorValue(Arm_Angle)>Arm_Start + 100){
		motor[L_Arm]=-35;	//lower  claw
	}
	while(SensorValue(Arm_Angle)<autoB_liftCap){
		motor[L_Arm]=55;	//lift  claw
	}
	motor[L_Arm]=10;	//hold claw position

	//drive to flag
	tDrive(78,70,autoB_fwd); //timed driving distance: Left power, Right power, Time (ms)

	//backup to platform center
	tDrive(-50,-50,autoB_backup);	

	//turn so rear towards platform
	tDrive(-65*direction,65*direction,autoB_turn); //timed driving distance: Left power, Right power, Time (ms)

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

	switch(autoSelect){
		case <=1000	:				//autoB BLUE
			autoA(1);
			break;
		case <1500 && >1000	:		//autoA BLUE
			autoB(1);
			break;
		case >=>2500 && <3000:		//autoA RED
			autoA(-1);
			break;
		case =>3000	:				//autoB RED
			autoB(-1);
			break;
		default:			// NO Autonomous - autoPot in center position
			break;
	}//end switch
}//end autonomous()
	