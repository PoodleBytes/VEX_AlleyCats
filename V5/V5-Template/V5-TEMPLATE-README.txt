There are two files to help with V5 robotics:
	V5-TEMPLATE.vex
	V5-ECHO-ENCODER.vex

V5-TEMPLATE.vex has examples of most of the VEX V5 commands needed to program a competitive robot. This was done by 'functionaizing' and 'methodizing' common actions

V5-ECHO-ENCODER.vex is a handly utility which displays encoder values to the controller.  When stepping through the autonomous pseudo-code the encoder valuse can be recorded & then zeroed.  When done these values can be passes to the motor functions to create an autonomous routine.

PROGRAMMING
VEX V5 C++ is an object-oriented language for programming robots.  'Objects' are beyond the scope of this document but basically means something can be created and 'inheret' certain values from it's parent.  For instance ArmMotor(), LeftDriveMotor() all are 'children' of the Motor() object. As such they inheret all of Motor()'s properties which can be changed such as velocity, velocity units, rotation, rotation units, braking mode etc..

The object properties often accept values (parameters).  A typical example:

ArmMotor.rotateFor(distance, distance units, speed, speed units)
object   property    			(parameters)

SMART MOTORS
'Smart Motors' are the heart of V5.  A smart motor simply means the motors have electronics built-in to allow some really powerful functionality - mostly notably an integrated 'encoder'. An encoder simply counts the motors' rotations - often referred to 'ticks'.  So if an encoder has 60 counts per revolution then 180 'ticks' would be 1/2 a revolution, 720 = 2 rev etc.  By controlling a motor's rotations allows fairly very accurate & powerful programming.  

Another advantage of the smart motors is the ability to 'lock' the motor's position to the encoder's value.  This is great for holding an arm in the air, for instance.

The motors also have a braking setting - coast, brake or hold.  Coast allows the motor to spin, brake electronically stops the motor and hold locks the motro's position to the encoder.

Lastly the motors have three basic modes of operation: spin, rotateTo and rotateFor descibed below. It is crucial to understand that rotateTo and rotateFor are BLOCKING commands!! This means than unless explicity told otherwise, NOTHING will happen until the command is completed.  This can be really good or really, really bad in that is something prevents a blocking command from completing the robot will stop dead!!!!!!

============  TEMPLATE ==============
        VEX C++ TEMPLATE FOR METHODIZING MOVEMENTS
 
The file is for nine-motor clawbot except the claw (called grabber) is a pulley which tilts the claw:
	4 - drive wheels
	2 - arm
	1 - grabber (claw)
	2 - ball 'puncher'  
       
        FUNCTIONS PREFIX
            'a' denotes ABSOLUTE movement from zero (home) position = aName(double, int, bool)
            'r' denotes movement RELATIVE to it's current position = rName(double, int, bool)
            't' denotes task = tName(bool)
        
        ORDER OF ROTATION PARAMETERS:
            DEGREES (DOUBLE), SPEED (INTEGER), WAIT (BOOLEAN)

            WAIT IS A 'FLAG' (TRUE/FALSE, 1/0) TO "WAIT UNTIL COMPLETION"(1) OR NOT (0)
                1 = BLOCKING, 0 = NON-BLOCKING
            
        COMMON MOTOR.settings
            .setStopping()
            .resetRotation()
            .spin()
            .rotateTo() 
            .rotateFor()   
       CONTROLS
            L1 - LOWER CLAW     L2 - RAISE CLAW
                    UP - LOAD BALL
            TBD LIFT CAP        R1  PUNCH BALL


========== ECHO ENCODER =============
The first step in any programming is outlining the steps needed to accomplish your objective.  This is especially true with autonomous. For example:
//RED FLAG
1 - shoot flag
2 - back up to low flag
3 - drive to platform
4 - turn RIGHT toward platform
5 - drive onto platform

By loading ECHO-ENCODER the motor's encoders will be displayed n the controller. The driver completes step one, records the drive motor's value, zeros the encoder and performs the next step:
1 - shoot flag			puncher()
2 - back up to low flag		LR -1300  RR 1300
3 - drive to platform		LR 1320  RR -1310
4 - turn RIGHT toward platform  LR 525  RR 525
5 - drive onto platform		LR 1210  RR 1210

Plugging these falues in to the functions creates the autonomous routine:
    punchBall();		//shoot ball
    eDrive(50,50,-1300,1300);   //back up to flag
    eDrive(50,50,1310,-1310);   //drive to platform 
    eDrive(50,50,525,525);    //*turn RIGHT 90
    eDrive(75,75,1210,-1210);   //drive onto platform

* Many times the RED and BLUE autonomous are mirrored. Once the autonomous is good simply reversing the turns should work for the other color.
 
