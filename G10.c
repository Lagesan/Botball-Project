#include <kipr/wombat.h>

const int LEFT_MOTOR = 3;
const int RIGHT_MOTOR = 0;

const int LINE_SENSOR = 0;
const int FRONT_BUMPER = 7;
const int REAR_BUMPER = 5;

const int LINE_THRESHOLD = 3200; 

const int FORWARD_SPEED = 90;
const int TURN_SPEED = 50;

const int TURN_180_MS = 1500;
const int REAR_BUMPER_THRESHOLD = 1000;

int main()
{
    printf("Ready for G10 Task (Analog Sensor Mode)\n");
    printf("Waiting for start signal...\n");

    msleep(2000);
    
    printf("Signal received, task starting!\n");

    printf("Phase 1: Moving to finish line...\n");
    while (digital(FRONT_BUMPER) == 0)
    {
        if (analog(LINE_SENSOR) > LINE_THRESHOLD)
        {
            motor(LEFT_MOTOR, FORWARD_SPEED);
            motor(RIGHT_MOTOR, FORWARD_SPEED - TURN_SPEED);
        }
        else
        {
            motor(LEFT_MOTOR, FORWARD_SPEED - TURN_SPEED);
            motor(RIGHT_MOTOR, FORWARD_SPEED);
        }
        msleep(10);
    }
    ao();
    printf("Reached the finish line!\n");

    printf("Phase 2: Preparing to return...\n");
    motor(LEFT_MOTOR, -80);
    motor(RIGHT_MOTOR, -80);
    msleep(500);
    ao();

    motor(LEFT_MOTOR, 100);
    motor(RIGHT_MOTOR, -100);
    msleep(TURN_180_MS);
    ao();
    printf("Turned around, returning to start.\n");

    printf("Phase 3: Returning to start area...\n");
    while (analog(REAR_BUMPER) < REAR_BUMPER_THRESHOLD)
    {
        if (analog(LINE_SENSOR) > LINE_THRESHOLD)
        {
            motor(LEFT_MOTOR, FORWARD_SPEED);
            motor(RIGHT_MOTOR, FORWARD_SPEED - TURN_SPEED);
        }
        else
        {
            motor(LEFT_MOTOR, FORWARD_SPEED - TURN_SPEED);
            motor(RIGHT_MOTOR, FORWARD_SPEED);
        }
        msleep(10);
    }
    ao();
    printf("Returned to the start area!\n");

    printf("G10 Task Complete!\n");
    return 0;
}