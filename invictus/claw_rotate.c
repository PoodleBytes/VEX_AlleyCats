/VARIABLES
int CLAW_FLIP_TIME	= 500;	//time to flip claw 180 degrees
//int HOLD_CLAW_FLIP = 8;	//power to hold claw_flip - NOT USED but may if claw doesn't stay
int CLAW_ROTATE = 0;	//track position of claw

//FUNCTION
void flipClaw(void){
	if(CLAW_ROTATE == 0){  //starting position of claw
		motor[claw_lift]=50;
		wait1Msec(CLAW_FLIP_TIME);
		motor[claw_lift]=0;
		CLAW_ROTATE = 1;
	}
	else{           //claw is rotated, turn back to starting position
		motor[claw_lift]=-50;
		wait1Msec(CLAW_FLIP_TIME);
		motor[claw_lift]=0;
		CLAW_ROTATE = 0;
	}

//BUTTON  - in usercontrol() task
   	if(vexRT(Btn5U)==1 ||vexRT(Btn5D)==1){	//rotate claw 
   		flipClaw();
   	}