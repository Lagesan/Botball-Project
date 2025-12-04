#include <kipr/wombat.h>

int main()
{
    int sensor_port = 0;
    int left_motor = 3;
    int right_motor = 0;
    
    int threshold = 2000;

    printf("Ready...\n");
    msleep(3000);
    
    while(1)
    {
        if(analog(sensor_port) > threshold)
        {
            motor(left_motor, 30);
            motor(right_motor, 80);
        }
        else
        {
            motor(left_motor, 100);
            motor(right_motor, 30);
        }
        msleep(5);
    }
    return 0;
}