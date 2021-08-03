#define Rotate motorB
#define Grabber motorC

#define Touch S4
#define Color S3
#define Gyro S2
#define Ultra S1

int threshold = 45;
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
	delay(500);
	setMotorSpeed(Grabber, 0);
	delay(1000);
}

task main()
{
	while(true){
		while(true){
			//Swing the arm back right
			setMotorSpeed(Rotate, 20);
			//Wait till the touch buttons get pressed
			if (getTouchValue(Touch) == 1){
				resetGyro(Gyro);
				setMotorSpeed(Rotate, 0);
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
			degGyro = getGyroDegrees(Gyro);
			if (degGyro <= 5 && degGyro >=-45)
			{
				setMotorSpeed(Rotate, -10);
				if (getGyroDegrees(Gyro) <= -45 && getGyroDegrees(Gyro) >= -50)
				{
					setMotorSpeed(Rotate, 0);
					break;
				}
			}
			sleep(30)
		}

		grab();

	//displayCenteredBigTextLine(3, getColorAmbient(Color));


	}
}
