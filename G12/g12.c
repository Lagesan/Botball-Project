#include <kipr/wombat.h>

const int LM = 3;
const int RM = 0;
const int LS = 0;
const int BS = 7;
const int US = 3; 
const int SG = 1;
const int SL = 0;

const int TH = 2150;
const int SPD = 45;
const int K = 18;

const int GO = 1000;
const int GC = 2000;
const int LD = 1000;
const int LU = 2000;

void drv(int l, int r) {
    if(l > 100) l = 100; if(l < -100) l = -100;
    if(r > 100) r = 100; if(r < -100) r = -100;
    motor(LM, l); motor(RM, r);
}

void follow() {
    int v = analog(LS);
    int adj = (v - TH) / K;
    drv(SPD + adj, SPD - adj);
}

int found() {
    if (digital(BS) == 1) return 1; 
    if (digital(US) == 0) return 1; 
    return 0;
}

int main() {
    enable_servos();
    
    set_servo_position(SL, LD); 
    set_servo_position(SG, GC); 
    msleep(800);

    while(found() == 0) {
        follow();
        msleep(10);
    }
    
    ao(); msleep(200);
    
    drv(-40, -40); 
    msleep(500); 
    ao(); msleep(200);
    
    set_servo_position(SG, GO); 
    msleep(600); 
    
    drv(40, 40); 
    msleep(600); 
    ao(); msleep(200);
    
    set_servo_position(SG, GC); msleep(600); 
    set_servo_position(SL, LU); msleep(600); 
    
    drv(-50, 50);
    msleep(1150); 
    
    int c = 0;
    while(c < 750) {
        follow();
        msleep(10);
        c++;
    }

    ao();
    set_servo_position(SL, LD); msleep(800);
    set_servo_position(SG, GO); msleep(800);
    set_servo_position(SL, LU); msleep(600);

    drv(-40, -40); msleep(1000);
    ao();
    
    disable_servos();
    return 0;
}