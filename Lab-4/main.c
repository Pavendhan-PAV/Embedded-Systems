#include <time.h>
#define SYSCTL_RCGCGPIO_R (*((volatile unsigned long *)0x400FE608))
#define GPIO_PORTC_DIR_R (*((volatile unsigned long *)0x40006400))
#define GPIO_PORTC_DEN_R (*((volatile unsigned long *)0x4000651C))
#define GPIO_PORTC_DATA_R (*((volatile unsigned long *)0x400063FC))

void delay(float sec)
{
  float milsec = 10 * sec;
  clock_t startTime = clock();
  while (clock() < (startTime + milsec))
    ;
}

int main()
{
  SYSCTL_RCGCGPIO_R = 0x04; // Clock Gating Enable
  GPIO_PORTC_DIR_R = 0xF0;  // Data Direction
  GPIO_PORTC_DEN_R = 0xF0;  // Digital Enable

  while (1)
  {
    GPIO_PORTC_DATA_R = 0x00;
    for(int i=0;i<15;i++)
    {
      //delay(20);
      GPIO_PORTC_DATA_R+=0x10; 
    }
    
    /*
    *((volatile unsigned int *)(0x40006040)) = 0x10; //delay(1); // C4
    *((volatile unsigned int *)(0x40006080)) = 0x20; //delay(1); // C5
    *((volatile unsigned int *)(0x40006100)) = 0x30; //delay(1); // C6
    *((volatile unsigned int *)(0x40006200)) = 0x40; //delay(1); // C7  
    */
  }

  return 0;
}