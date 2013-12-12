/*
 * This file contains sources of system controllers
 */

#include "controllers.h"

#if ATOM_DATA_RECEIVE == ENABLED

// aileron position controller using surfnav
void controllerAileron_surfnav() {

	float KP = AILERON_POSITION_KP_SURFNAV;
	
	float correctedPos = xPosSurf - tan((((float) delayedRollAngle)/1800)*3.141592)*groundDistance*1000;

	float error = correctedPos;

	aileronSpeedSetPoint = KP*error;

	if (aileronSpeedSetPoint > SURFNAV_CONTROLLER_SATURATION) {
		aileronSpeedSetPoint = SURFNAV_CONTROLLER_SATURATION;
	} else if (aileronSpeedSetPoint < -SURFNAV_CONTROLLER_SATURATION) {
		aileronSpeedSetPoint = -SURFNAV_CONTROLLER_SATURATION;
	}
}

// elevator position controller using surfnav
void controllerElevator_surfnav() {

	float KP = ELEVATOR_POSITION_KP_SURFNAV;

	float correctedPos = yPosSurf - tan((((float) delayedPitchAngle)/1800)*3.141592)*groundDistance*1000;

	float error = correctedPos;

	elevatorSpeedSetpoint = KP*error;

	if (elevatorSpeedSetpoint > SURFNAV_CONTROLLER_SATURATION) {
		elevatorSpeedSetpoint = SURFNAV_CONTROLLER_SATURATION;
	} else if (elevatorSpeedSetpoint < -SURFNAV_CONTROLLER_SATURATION) {
		elevatorSpeedSetpoint = -SURFNAV_CONTROLLER_SATURATION;
	}
}

#endif

#if PX4FLOW_DATA_RECEIVE == ENABLED

// aileron speed controller using px4flow data
void controllerAileronSpeed() {

	float KP = AILERON_SPEED_KP;
	float KD = AILERON_SPEED_KD;
	//~ float KI = AILERON_SPEED_KI;

	//~ float error = aileronSpeedSetPoint-aileronSpeed;
	float error = aileronSpeedSetPoint-aileronSpeed;

	// position control
#if GUMSTIX_DATA_RECEIVE == ENABLED

	float positionError = yPosGumstix;

	if (positionError > 2000) {
		positionError = 2000;
	} else if (positionError < -2000) {
		positionError = -2000;
	}

	// calculate position P
	float positionProportional = POSITION_KP_GUMSTIX*positionError;

	// calculate adaptive offset
	int8_t smer;
	if (positionError > 0) {
		smer = 1;
	} else {
		smer = -1;
	}

	// calculate position I
	gumstixAileronIntegral += POSITION_KI_GUMSTIX*smer;

#endif // GUMSTIX_DATA_RECEIVE == ENABLED

	// calculate P
	float proportional = KP*error;

	//~ // calculate angular
	//~ float angular = -rollAngle;

	// calculate D
	float derivative = KD*(error-aileronSpeedPreviousError);

	//~ // calculate I
	//~ float integrational = -KI*aileronSpeedIntegration;
//~ 
	//~ if (constant2 < 1) {
		//~ integrational = 0;
	//~ }

	aileronSpeedPreviousError = error;

#if GUMSTIX_DATA_RECEIVE == ENABLED

	if (validGumstix == 1 && (abs(aileronSpeed) < GUMSTIX_CONTROLLER_SATURATION)) {
		controllerAileronOutput = proportional + derivative + positionProportional + gumstixAileronIntegral;
	} else {
		controllerAileronOutput = proportional + derivative;
	}

#elif ATOM_DATA_RECEIVE == ENABLED

	controllerAileronOutput = proportional + derivative - rollAngle;

#else

	controllerAileronOutput = proportional + derivative;

#endif

	// controller saturation
	if (controllerAileronOutput > CONTROLLER_AILERON_SATURATION) {
		controllerAileronOutput = CONTROLLER_AILERON_SATURATION;
	} else if (controllerAileronOutput < -CONTROLLER_AILERON_SATURATION) {
		controllerAileronOutput = -CONTROLLER_AILERON_SATURATION;
	}
}

// elevator speed controller using px4flow data
void controllerElevatorSpeed() {

	float KP = ELEVATOR_SPEED_KP;
	float KD = ELEVATOR_SPEED_KD;
	//~ float KI = ELEVATOR_SPEED_KI;

	//~ float error = elevatorSpeedSetpoint+elevatorSpeed;
	float error = elevatorSpeedSetpoint + elevatorSpeed;

#if GUMSTIX_DATA_RECEIVE == ENABLED

	float positionError = xPosGumstix - elevatorSetPoint;

	if (positionError > 2000) {
		positionError = 2000;
	} else if (positionError < -2000) {
		positionError = -2000;
	}

	// calculate position P
	float positionProportional = POSITION_KP_GUMSTIX*positionError;

	// calculate adaptive offset
	int8_t smer;
	if (positionError > 0) {
		smer = 1;
	} else {
		smer = -1;
	}

	// calculate position I
	gumstixElevatorIntegral += POSITION_KI_GUMSTIX*smer;

#endif // GUMSTIX_DATA_RECEIVE == ENABLED

	// calculate P
	float proportional = KP*error;

	//~ // calculate angular
	//~ float angular = -pitchAngle;

	// calulate D
	float derivative = KD*(error-elevatorSpeedPreviousError);

	//~ // calculate I
	//~ float integrational = KI*elevatorSpeedIntegration;
//~ 
	//~ if (constant2 < 1) {
		//~ integrational = 0;
	//~ }

	elevatorSpeedPreviousError = error;

#if GUMSTIX_DATA_RECEIVE == ENABLED

	if (validGumstix == 1 && (abs(elevatorSpeed) < GUMSTIX_CONTROLLER_SATURATION)) {
		controllerElevatorOutput = proportional + derivative + positionProportional + gumstixElevatorIntegral;
	} else {
		controllerElevatorOutput = proportional + derivative;
	}

#elif ATOM_DATA_RECEIVE == ENABLED

	controllerElevatorOutput = proportional + derivative - pitchAngle;

#else

	controllerElevatorOutput = proportional + derivative;

#endif

	// controller saturation
	if (controllerElevatorOutput > CONTROLLER_ELEVATOR_SATURATION) {
		controllerElevatorOutput = CONTROLLER_ELEVATOR_SATURATION;
	} else if (controllerElevatorOutput < -CONTROLLER_ELEVATOR_SATURATION) {
		controllerElevatorOutput = -CONTROLLER_ELEVATOR_SATURATION;
	}
}


// altitude controller - estimator
void controllerThrottleEstimator() {

   //new cycle 
   estimator_cycle++;

    if(groundDistance != estimated_pos_prev) {//input data changed

        // extreme filter
        if(abs(groundDistance - estimated_pos_prev) <= 0.3) {//limitation cca 3m/s

           // compute new values 
           estimated_velocity = (groundDistance - estimated_pos_prev) / 7;
           estimated_position = groundDistance;
           estimated_pos_prev = groundDistance;
           estimator_cycle = 0;

        }

    } else {

        if (estimator_cycle >= 8) { //safety reset

             estimated_velocity = 0;
             estimated_position = groundDistance;
             estimated_pos_prev = groundDistance;
             estimator_cycle = 0;

        } else { //estimate position

            estimated_position += estimated_velocity;

        }
    }
}

// altitude controller
void controllerThrottle() {

	float KP = ALTITUDE_KP;
	float KD = ALTITUDE_KD;
	float KI = ALTITUDE_KI;	

	float error = (altitudeSetpoint - estimated_position); //in meters!

	// calculate proportional
	float proportional = KP * error;

	// calculate derivative
	float derivative = -1 * KD * estimated_velocity / 0.0142222;

	// calculate integrational
	throttleIntegration += KI * error * 0.0142222;
	if (throttleIntegration > CONTROLLER_THROTTLE_SATURATION) {
		throttleIntegration = CONTROLLER_THROTTLE_SATURATION;
	}
	if (throttleIntegration <  -CONTROLLER_THROTTLE_SATURATION) {
		throttleIntegration = -CONTROLLER_THROTTLE_SATURATION;
	}
	float integrational = throttleIntegration;
	
	//total output
	controllerThrottleOutput = proportional + derivative + integrational;
	if (controllerThrottleOutput > 2*CONTROLLER_THROTTLE_SATURATION) {
		controllerThrottleOutput = 2*CONTROLLER_THROTTLE_SATURATION;
	}
	if (controllerThrottleOutput < -2*CONTROLLER_THROTTLE_SATURATION) {
		controllerThrottleOutput = -2*CONTROLLER_THROTTLE_SATURATION;
	}

}


#endif // PX4FLOW_DATA_RECEIVE == ENABLED

#if GUMSTIX_DATA_RECEIVE == ENABLED

// aileron position controller using surfnav
void controllerAileron_gumstix() {

	float KP = 0.07;
	float KD = 10.08;
	float KI = 0.01;

	float error = yPosGumstix - aileronSetPoint;

	int8_t smer;
	if (error > 0) {
		smer = 1;
	} else {
		smer = -1;
	}

	// calculate offset
	gumstixAileronIntegral += KI*smer;

	// calculate proportional
	float proportional = KP*error;

	// calculate derivative
	float derivative = (error - yGumstixPreviousError)*KD;

	yGumstixPreviousError = error;

	controllerAileronOutput = proportional + derivative + gumstixAileronIntegral;

	if (controllerAileronOutput > CONTROLLER_AILERON_SATURATION) {
		controllerAileronOutput = CONTROLLER_AILERON_SATURATION;
	} else if (controllerAileronOutput < -CONTROLLER_AILERON_SATURATION) {
		controllerAileronOutput = -CONTROLLER_AILERON_SATURATION;
	}
}

// elevator position controller using surfnav
void controllerElevator_gumstix() {

	float KP = POSITION_KP_GUMSTIX;

	float error =  elevatorSetPoint - xPosGumstix;

	elevatorSpeedSetpoint = -KP*error;

	if (elevatorSpeedSetpoint > GUMSTIX_CONTROLLER_SATURATION) {
		elevatorSpeedSetpoint = GUMSTIX_CONTROLLER_SATURATION;
	} else if (elevatorSpeedSetpoint < -GUMSTIX_CONTROLLER_SATURATION) {
		elevatorSpeedSetpoint = -GUMSTIX_CONTROLLER_SATURATION;
	}
}

#endif // GUMSTIX_DATA_RECEIVE == ENABLED
