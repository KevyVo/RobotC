#define Rotate motorB
#define Grabber motorC

#define Touch S4
#define Color S3
#define Gyro S2
#define Ultra S1

int threshold = 50;
int maxTurn = -80;
long degGyro;

void grab()
{
	setMotorReversed(Grabber, true);
	setMotorSpeed(Grabber, 18);
	delay(500);
	setMotorSpeed(Grabber, 0);
	delay(500);
}

void drop()
{
	setMotorReversed(Grabber, false);
	setMotorSpeed(Grabber, 8);
	delay(1000);
	setMotorSpeed(Grabber, 0);
	delay(1000);
}

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

task main()
{
	while(true){
		while(true){
			//Swing the arm back right
			setMotorSpeed(Rotate, 30);
			//Wait till the touch buttons get pressed
			if (getTouchValue(Touch) == 1){
				setMotorSpeed(Rotate, 0);
				resetGyro(Gyro);
				drop();
				break;
			}
		}
		// If the sensor is cover then the robot will terminate the program
		if (getColorAmbient(Color)>threshold){
			break;
		}

		while(true)
		{
			//&& (degGyro <= -30 && degGyro <= maxTurn)
			// -35 <= -30 | -35 >= -80
		  //if (withinReach(getUSDistance(Ultra)) && (degGyro <= -20 && degGyro >= maxTurn))
			degGyro = getGyroDegrees(Gyro);

			if (withinReach(getUSDistance(Ultra)))
			{
				if (degGyro <= 6 && degGyro >= -20)
				{
					displayCenteredBigTextLine(3, "There is object in the loading dock");
				}
				else
				{
					detected(degGyro);
					break;
				}
			}

			if (degGyro >= maxTurn)
			{
				setMotorSpeed(Rotate, -10);
			}

			if (degGyro <= maxTurn)
			{
				setMotorSpeed(Rotate, 0);
				break;
			}
			//sleep(30);
		}

		//grab();

	//displayCenteredBigTextLine(3, getColorAmbient(Color));


	}
}
