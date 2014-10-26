#ifndef PACKETS_H
#define PACKETS_H

typedef struct
{
	unsigned char COORDINATOR[8];
	unsigned char KC1[8];
	unsigned char K1[8];
	unsigned char BROADCAST[8];
	unsigned char UNKNOWN16[2];
} ADDRESST;

typedef struct
{
	unsigned char GROUND_DISTANCE;
	unsigned char ELEVATOR_SPEED;
	unsigned char AILERON_SPEED;
	unsigned char ELEVATOR_SPEED_ESTIMATED;
	unsigned char AILERON_SPEED_ESTIMATED;
	unsigned char ELEVATOR_POS_ESTIMATED;
	unsigned char AILERON_POS_ESTIMATED;		
} TELEMETRIEST;

//Telemetry request options
typedef struct
{
	unsigned char SENDING_OFF;
	unsigned char SENDING_ON;
	unsigned char SENDING_ONCE;
	unsigned char SENDING_STATUS;
} TELREQOPTT;

typedef struct
{
	unsigned char LAND_ON;
	unsigned char LAND_OFF;
} LANDINGT;

typedef struct
{
	unsigned char FOLLOW;
	unsigned char NOT_FOLLOW;
} TRAJECTORYT;

typedef struct
{
	unsigned char THROTTLE_SP;
	unsigned char ELEVATOR_SP;	
	unsigned char AILERON_SP;	
} SETPOINTST;

typedef struct
{
	unsigned char ABSOLUT;
	unsigned char RELATIV;
} POSITIONST;

typedef struct
{
	unsigned char OFF;
	unsigned char VELOCITY;
	unsigned char POSITION;	
} CONTROLLERST;

typedef struct
{
	unsigned char LANDING;		
	unsigned char SET_SETPOINTS;
	unsigned char CONTROLLERS;
	unsigned char TRAJECTORY;
	unsigned char TRAJECTORY_POINTS;
}COMMANDST;

extern unsigned char GET_STATUS;

extern ADDRESST ADDRESS;
extern TELEMETRIEST TELEMETRIES;
extern TELREQOPTT TELREQOPT;
extern LANDINGT LANDING;
extern TRAJECTORYT TRAJECTORY;
extern SETPOINTST SETPOINTS;
extern POSITIONST POSITIONS;
extern CONTROLLERST CONTROLLERS;
extern COMMANDST COMMANDS;

void constInit();
void packetHandler(unsigned char *inPacket);

#endif /*PACKETS_H*/