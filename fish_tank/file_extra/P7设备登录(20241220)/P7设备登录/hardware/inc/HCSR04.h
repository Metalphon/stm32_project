#ifndef __HCSR04_H
#define __HCSR04_H

void HC_SR04_Init(void);
int16_t sonar_mm(void);
float sonar(void);
float HSCR_Value_Conversion(void);
uint64_t numlen(uint64_t num);

#endif
