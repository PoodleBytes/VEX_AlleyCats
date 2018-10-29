

// This code is for the VEX cortex competition code
#pragma platform(VEX2)
//#pragma competitionControl(Competition)	//don't think we need this but leave just in case
#include "Vex_Competition_Includes.c"

void pre_auton()
{

  bStopTasksBetweenModes = true;
  // All activities that occur before the competition starts
  // Example: slaveMotor(), clearing encoders, setting servo positions, ...

}

task autonomous()
{
   // Insert autonomous code here.
}


task usercontrol()	//replaces main() function in you code
{
  // User control code here, inside the loop

  while (true)
  {
    //infinte loop
  }
}
