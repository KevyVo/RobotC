#define sensorEV3_Color S3
#define leftwheel motorB
#define rightwheel motorc

//Param
int nMotorSpeedSetting = 20;
float nPfactor = 0.2;
int grey = 35;
int lowest = 60; //white
int highest = 10;//black
float error;
bool turnDone = false;
bool noTape = false;
long degreesGyro;

// We are back!

const int targetDistance = 5;
int currentDistance = getUSDistance(S4);


// This will keep the robot inline with the black tape
void scanLine()
{
	motor[leftwheel] = -3;
	motor[rightwheel] = 3;

	time1[T1] = 0;
	while(time1[T1] < 500)
	{
		if (SensorValue[colour] > highest)
		{
			highest = SensorValue[colour];
		}
		if (SensorValue[colour] < lowest)
		{
			lowest = SensorValue[colour];
		}
	}
	grey = (highest - lowest) / 2;
	motor[leftwheel] = 0;
	motor[rightwheel] = 0;
}

// This will set the to far away it will keep moving the robot and return the if the robot has reach it destination or not
bool endLine(int currDistance, int targetDistance)
{
	// If the currentDistance is in the range of (20 - 0) then slow down to 10% speed cause it will have more of a control reading
	// and will slowing come to a stop
	if ((currDistance > targetDistance))
		{
			setMotorReversed(motorB, false);
			setMotorReversed(motorC, false);
			setMotorSpeed(motorB, 50);
			setMotorSpeed(motorC, 50);
			//Found that moveMotorTarget not as smooth as setmotorSpeed because it does not need a contain specfic range
			//moveMotorTarget(motorB, 360, 80);
			//moveMotorTarget(motorC, 360, 80);
			return false;
		}
	
	if (currDistance < targetDistance)
		{
			setMotorReversed(motorB, true);
			setMotorReversed(motorC, true);
			setMotorSpeed(motorB, 10);
			setMotorSpeed(motorC, 10);
			//Found that moveMotorTarget not as smooth as setmotorSpeed because it does not need a contain specfic range
			//waitUntilMotorStop(motorB);
			//waitUntilMotorStop(motorC);
			return false;
		}

	if (currDistance < 20 && currentDistance > 0)
	{
		setMotorSpeed(motorB, 10);
		setMotorSpeed(motorC, 10);
		// If the currentDistance equals the targetDistance then stop
		if (currDistance == targetDistance)
		{
			setMotorSpeed(motorB, 0);
			setMotorSpeed(motorC, 0);
			waitUntilMotorStop(motorB);
			waitUntilMotorStop(motorC);
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

// This function check what side of the robot is far and set the direction
int setFar(long degreesGyro){
	// Return the base deg back to zero
	resetGyro(S2);

	// Vars
	long left;
	long right;
	long degreesGyro2 = degreesGyro;

	degreesGyro2 = getGyroDegrees(S2);

	// now checks left for how many deg
	while (degreesGyro2 <= 0 && degreesGyro2 >=-90)
	{
		degreesGyro2 = getGyroDegrees(S2);
		setMotorSpeed(motorC, 10);
		setMotorSpeed(motorB, -10);

	}

	// Stops the motors
	setMotorSpeed(motorC, 0);
	setMotorSpeed(motorB, 0);

	// records the distance of left into the sensor
	left = getUSDistance(S4);

	degreesGyro2 = getGyroDegrees(S2);
	// now check right for how many deg
	while (degreesGyro2 >= -90 && degreesGyro2 <= 90)
	{
		degreesGyro2 = getGyroDegrees(S2);
		setMotorSpeed(motorC, -10);
		setMotorSpeed(motorB, 10);
	}

	// Stops the motors
	setMotorSpeed(motorC, 0);
	setMotorSpeed(motorB, 0);

	// records the distance of the right into the sensor
	right = getUSDistance(S4);



	//left side is farther away
	if (left > right)
	{
		degreesGyro2 = getGyroDegrees(S2);
		// now checks left for how many deg
		while (degreesGyro2 <= 90 && degreesGyro2 >=-90)
		{
			degreesGyro2 = getGyroDegrees(S2);
			setMotorSpeed(motorC, 10);
			setMotorSpeed(motorB, -10);
		}
		// Stops the motors
		setMotorSpeed(motorC, 0);
		setMotorSpeed(motorB, 0);
		return -90;
	}

	//right side is farthur away
	if (left < right)
	{
		return 90;
	}

	return 0;
}

// This function check what side of the robot is far and set the direction
int setClose(long degreesGyro){
	// Return the base deg back to zero
	resetGyro(S2);

	// Vars
	long left;
	long right;
	long degreesGyro2 = degreesGyro;

	// now checks left for how many deg  -10
	while (degreesGyro2 <= 0 && degreesGyro2 >=-90)
	{
		degreesGyro2 = getGyroDegrees(S2);
		setMotorSpeed(motorC, 10);
		setMotorSpeed(motorB, -10);

	}

	// Stops the motors
	setMotorSpeed(motorC, 0);
	setMotorSpeed(motorB, 0);

	// records the distance of left into the sensor
	left = getUSDistance(S4);


	degreesGyro2 = getGyroDegrees(S2); 
	// now check right for how many deg //-80 >
	while (degreesGyro2 >= -90 && degreesGyro2 <= 90)
	{
		degreesGyro2 = getGyroDegrees(S2);
		setMotorSpeed(motorC, -10);
		setMotorSpeed(motorB, 10);
	}

	// Stops the motors
	setMotorSpeed(motorC, 0);
	setMotorSpeed(motorB, 0);

	// records the distance of the right into the sensor
	right = getUSDistance(S4);



	//right side is closer
	if (left > right)
	{
		return 90;
	}

	//left side is closer
	if (left < right)
	{
		degreesGyro2 = getGyroDegrees(S2);
		// now checks left for how many deg
		while (degreesGyro2 <= 90 && degreesGyro2 >=-90)
		{
			degreesGyro2 = getGyroDegrees(S2);
			setMotorSpeed(motorC, 10);
			setMotorSpeed(motorB, -10);
		}
		// Stops the motors
		setMotorSpeed(motorC, 0);
		setMotorSpeed(motorB, 0);
		return -90;
	}
	// Both distance is equal
	return 0;
}

//This function finds the corner
bool startpos(int currentDistance, int targetDistance)
{

	// Let the robot go straight until it hit the end of the wall
	while (!endLine(currentDistance, targetDistance)) //!false = true
	{
		currentDistance = getUSDistance(S4);
		displayCenteredBigTextLine(4, "Dist: %3d cm", currentDistance);
	}

	// set to the closer corner
	setClose(getGyroDegrees(S2));

	currentDistance = getUSDistance(S4);
	// Let the robot go straight until it hit the end of the wall
	while (!endLine(currentDistance, targetDistance))
	{
		currentDistance = getUSDistance(S4);
		displayCenteredBigTextLine(4, "Dist: %3d cm", currentDistance);
	}

	// Set the start point
	setFar(getGyroDegrees(S2));

	// process competle
	if (getUSDistance(S4) > targetDistance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

task main()
{
	resetGyro(S2);

	startpos(currentDistance, targetDistance);

	//Main Loop
	while (true)
	{

		displayCenteredTextLine(3, "Sensor Value: %d", SensorValue[colour]);
		displayCenteredTextLine(5, "Gyro Value: %d", getGyroDegrees(S2));

		//Grab Ultrasonic reading and store in variable, also sensor reading was accurate so no need more adjustment
		currentDistance = getUSDistance(S4);
		//Display to LCD
		displayCenteredTextLine(8, "Dist: %3d cm", currentDistance);

		// If to far away from target, move closer
		if ((currentDistance > targetDistance))
		{
			setMotorReversed(motorB, false);
			setMotorReversed(motorC, false);
			setMotorSpeed(motorB, 20);
			setMotorSpeed(motorC, 20);
		}


		// If pass target then reverse direction, and reading should never go negative.
		//Just incase of reading error move back is place here

		if (currentDistance < targetDistance)
		{
			setMotorReversed(motorB, true);
			setMotorReversed(motorC, true);
			setMotorSpeed(motorB, 10);
			setMotorSpeed(motorC, 10);
		}

		// If the currentDistance is in the range of (20 - 0) then slow down to 10% speed cause it will have more of a control reading
		// and will slowing come to a stop

		if ((currentDistance < 20 && currentDistance > 0))
		{
			setMotorSpeed(motorB, 10);
			setMotorSpeed(motorC, 10);
			// If the currentDistance equals the targetDistance then stop
			if (currentDistance == targetDistance)
			{
				setMotorSpeed(motorB, 0);
				setMotorSpeed(motorC, 0);
			}
		}


		// This mean that there no more black tape
		if (noTape == true){
			// Stop the motors
			setMotorSpeed(leftwheel, 0);
			setMotorSpeed(rightwheel, 0);
			//end program
			break;
		}
		// Robot keep moving forward
		setMotorSpeed(leftwheel, 25);
		setMotorSpeed(rightwheel, 25);

		// If it finds the edge black tape
		if ((SensorValue[colour] <= 30 && SensorValue[colour] >= 20)){
			//stop the robot
			setMotorSpeed(leftwheel, 0);
			setMotorSpeed(rightwheel, 0);
			//Stop for 2 seconds
			delay(2000);
			while (true){
				displayCenteredTextLine(3, "Sensor Value: %d", SensorValue[colour]);
				displayCenteredTextLine(5, "Gyro Value: %d", getGyroDegrees(S2));

				if (turnDone == true){
					break;
				}

				//move forward
				setMotorSpeed(leftwheel, 10);
				setMotorSpeed(rightwheel, 10);
				delay(1200);

				//stop motors
				setMotorSpeed(leftwheel, 0);
				setMotorSpeed(rightwheel, 0);
				delay(1000);

				while(true){
					//Now turn left
					setMotorSpeed(leftwheel, -12);
					setMotorSpeed(rightwheel, 12);

					// Once the robot has turn left it face paralel to the black line------------
					if (SensorValue[colour] <= 2 && SensorValue[colour] >= 0){
						// break out of the inner loop
						setMotorSpeed(leftwheel, 0);
						setMotorSpeed(rightwheel, 0);
						turnDone = true;
						break;
					}
				}
			}
			displayCenteredTextLine(3, "Sensor Value: %d", SensorValue[colour]);
			displayCenteredTextLine(5, "Gyro Value: %d", getGyroDegrees(S2));
			// Capture the datapoints of the black line
			scanLine();
			displayCenteredTextLine(8, "scan done");
			while (true){
				displayCenteredTextLine(8, "Last while loop");

				// This mean we had reach the end of end of the tape
				if (SensorValue[colour] > 55) {
					noTape = true;
					break;
				}
				// For display to screen testing
				displayCenteredTextLine(3, "Sensor Value: %d", SensorValue[colour]);
				displayCenteredTextLine(5, "Gyro Value: %d", getGyroDegrees(S2));
				//Error for PID
				error = SensorValue[colour] - grey;
				//Keep the robot bouncing back and forth between the sides of the black tape
				motor[leftwheel] = nMotorSpeedSetting - round(error * nPfactor);
				motor[rightwheel] = nMotorSpeedSetting + round(error * nPfactor);
				wait1Msec(30);
			}
		}
	}
}
