// Define some constants
#define Rotate motorB
#define Grabber motorC
#define Touch S4
#define Color S3
#define Gyro S2
#define Ultra S1

// This set the maxuimum range of the robot, the farest the robot can turn is -90. The reason the number is negative is that the bound is going to the left side.
int maxTurn = -80;
long degGyro;

// This function will perform the a subroutine to grab the object infront of it and display the action to the LCD.
void grab()
{
	setMotorReversed(Grabber, true);
	setMotorSpeed(Grabber, 20);
	delay(500);
	setMotorSpeed(Grabber, 0);
	displayCenteredBigTextLine(3, "Pick up");
	delay(500);
}

// This function will perform the a subroutine to release the claw and drop the item it was holding. Once release the function will display the claw is in a release state and display it on the LCD.
void drop()
{
	setMotorReversed(Grabber, false);
	setMotorSpeed(Grabber, 8);
	delay(1000);
	setMotorSpeed(Grabber, 0);
	displayCenteredBigTextLine(3, "Release");
	delay(1000);
}

// Check if there is a object to grab within the reach of the claw, returns a boolean value.
bool withinReach(float distance)
{
	if (distance >= 0 && distance <= 5)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// This functions with help to set the correct delay time to the specfic range. 
// While testing we found between theses three ranges, they all differ on delay time to get the claw to be correctly position in front of the object.
bool detected(long deg)
{
	if (deg <= -20 && deg>= -40)
	{
		delay(200);
		setMotorSpeed(Rotate, 0);
		delay(500);
		grab();
	}

	else if (deg <= -40 && deg >= -60)
	{
		delay(400);
		setMotorSpeed(Rotate, 0);
		delay(500);
		grab();
	}

	else
	{
		delay(500);
		setMotorSpeed(Rotate, 0);
		delay(500);
		grab();
	}
	return true;
}

// This function control the color modes. If the red is present in front of the color sensor it will terminate the program. 
// If yellow is present it will pause the program and await either a green color to resume the program or terminate if red is shown.
// It will also display on the LCD what current mode they are in.
void setMode()
{
	//Red = 5 (Stop and exit program)
	//White = 6 (Pause)
  	//Green = 3 (Resume)
	if (getColorName(Color) == 5)
	{
		displayCenteredBigTextLine(6, "Red: Stop");
		stopAllTasks();
	}
	if (getColorName(Color) == 6)
	{
		displayCenteredBigTextLine(6, "White: Pause");
		while (getColorName(Color) != 3)
		{
			if (getColorName(Color) == 5)
			{
				displayCenteredBigTextLine(6, "Red: Stop");
				stopAllTasks();
			}
			displayCenteredBigTextLine(6, "Pause loop");
		}
		displayCenteredBigTextLine(6, "Green: Go");
	}
}

// The main loop of the program
task main()
{
	while(true)
	{
			// This loop will bring back the robot are to the starting position and find the starting bound by reseting the gyro.
			while(true){
				setMode();
				//Swing the arm back right
				setMotorSpeed(Rotate, 30);
				//Wait till the touch buttons get pressed
				if (getTouchValue(Touch) == 1){
					setMotorSpeed(Rotate, 0);
					resetGyro(Gyro);
					SetMode();
					drop();
					break;
				}
			}

            // This loop will search for a object to pick up, while moving to the left. 
			while(true)
			{
				setMode();
				degGyro = getGyroDegrees(Gyro);

                // Check if there a object with in reach
				if (withinReach(getUSDistance(Ultra)))
				{
					setMode();

                    // This will ignore the loading zone area. Even if there is something in the loading zone, all it would do it infinitly pick up the object and drop it.
                    // This the reason why we can safetly ignore the loading zone range.
					if (degGyro <= 6 && degGyro >= -20)
					{
						displayCenteredBigTextLine(3, "Dock is Full");
					}
					else
					{
						setMode();
						detected(degGyro);
						break;
					}
				}

                // Turns to the left
				if (degGyro >= maxTurn)
				{
					setMotorSpeed(Rotate, -10);
				}
                
                // When the most left bounds is reach, breakout.
				if (degGyro <= maxTurn)
				{
					setMotorSpeed(Rotate, 0);
					break;
				}
			}
	}
}
