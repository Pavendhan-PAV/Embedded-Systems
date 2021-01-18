
/*Write a register level program to calculate each
bit address of Port F Data register, where the red,
green, blue LEDs are connected. Also light each
led with different delay 1 sec, 2sec, 3 sec. */

#define Black 0x00000000
#define Red 0x00000002
#define Blue 0x00000004
#define Green 0x00000008

void delay_function(int n)
{
  int i, j;
  for (i = 0; i < n; i++)
    for(j = 0; j < 1000000; j++); // simulated clock 1Mhz ( loop takes 1 clock cycle most of the time, so 1000 clock cycles = 1ms delay)               
}


int main(void)
{
    // Clock gating register to enable the clock on GPIOF Port
    (*(unsigned int *) 0x400FE608) = 0x00000020;        // bits 31:6 =0 bit 5= 1 bits 4:0 = 0  in hex = 0x 00000020 
    
    // Direction for input and output
    (*(unsigned int*) 0x40025400) = 0x0000000E;         //bits 31:8 = 0, bits 7:4 also = 0, bits 3:1 = 1 , bit 0 = 0, port 0 is unused as it is a special GPIO PIN which needs to be unlocked to be used
    
    // Digital Enable to enable the digital functionality on all the three LEDs
    (*(unsigned int*)0x4002551C) = 0x0000000E;
    
    while(1)
    {

      (*(unsigned int*)0x40025038)  = Red;
            delay_function(1); // give a delay of 1 second
      (*(unsigned int*)0x40025038)  = Black;
          
    
      (*(unsigned int*)0x40025038)  = Blue;
            delay_function(2); // give a delay of 2 second
      (*(unsigned int*)0x40025038)  = Black;
          
    
      (*(unsigned int*)0x40025038)  = Green;   
            delay_function(3); // give a delay of 3 second
      (*(unsigned int*)0x40025038)  = Black;
        
    }  
    return 0;
}

