void autoA(int direction){  //directi0n = 1 blue, -1 = red side
/*  Autonomous from Position A 	*/

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
	tDrive(78,70,1820); //timed driving distance: Left power, Right power, Time (ms)
//	eDrive(60,60,1000);		//eDrive (encoder) is roughly 62% of tDrive (timed) at 60% power
												//but is it reliable???

	//lift cap
	while(SensorValue(Arm_Angle) < 500){
				motor[L_Arm]=40;}	//lift claw
	motor[L_Arm]=15;

	tDrive(-50,-50,300);	//backup

	//turn
	tDrive(-65*direction,65*direction,1800); //timed driving distance: Left power, Right power, Time (ms)


	//tDrive(50,50,250); //drive fwd

	//lower cap
	while(SensorValue(Arm_Angle) > Arm_Start){
				motor[L_Arm]=-50;}	//lower claw
	motor[L_Arm]=0;

	// backup
	tDrive(-120,-120,1300);
	while(SensorValue(Arm_Angle) < 300){
				motor[L_Arm]=40;}	//lift claw
	motor[L_Arm]=10;// end of autoA  */
}//end autoA