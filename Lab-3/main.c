#define BUTTON_PRESSED 0x00
#define switch2 0x40025040 //pf4 2^(4+2)
#define switch1 0x40025004 //pf0 2^(0+2)

int main()
{
  *((unsigned int *)0x400FE608) = 0x20U; //clock gating register - enable clock on port F
  *((unsigned int *)0x40025400) = 0x0EU; // direction ( 1 - corresponding pins are outputs PF1 PF2 PF3 outputs)
  *((unsigned int *)0x4002551C) = 0x0EU; // digital mode

  *((unsigned int *)0x40025520) = 0x4C4F434B; // first status in GPIOLOCK has to be unlocked to access GPIOCR register
   //A write of the value 0x4C4F.434B unlocks the GPIO Commit (GPIOCR) register for write access

  *((unsigned int *)0x40025524) = 0x011U; // now set GPIOCR register for pf4 and pf0 (sw1 is connected to pf0 ; sw2 to pf4 where pull up needs to be enabled) to allow write access
  *((unsigned int *)0x40025510) = 0x011U; // enable pull up resistor for pf4 and pf0

  int count = 0;
  // no light 00
  // Red      02
  // Green    08
  // Blue     04
  
  while (1)
  {
    if (count % 4 == 0)
    {
      *((unsigned int *)switch1) = BUTTON_PRESSED; // simulate pressing of both switches
      *((unsigned int *)switch2) = BUTTON_PRESSED;
    }

    else if (count % 4 == 1)
    {
      *((unsigned int *)switch2) = BUTTON_PRESSED; //press only switch 2
      *((unsigned int *)switch1) = ~BUTTON_PRESSED;
    }
    else if (count % 4 == 2)
    {
      *((unsigned int *)switch2) = ~BUTTON_PRESSED; //press only switch 1
      *((unsigned int *)switch1) = BUTTON_PRESSED;
    }
    else
    {
      *((unsigned int *)switch2) = ~BUTTON_PRESSED; //press none
      *((unsigned int *)switch1) = ~BUTTON_PRESSED;
    }

    if (*((unsigned int *)switch2) == BUTTON_PRESSED && *((unsigned int *)switch1) == BUTTON_PRESSED)
    {
      *((unsigned int *)0x400253FC) = 0x08; // both switches then green
    }
    else if (*((unsigned int *)switch2) == BUTTON_PRESSED)
    {
      *((unsigned int *)0x400253FC) = 0x02; //only switch2 - red
    }
    else if (*((unsigned int *)switch1) == BUTTON_PRESSED) //only switch 1 -  blue
    {
      *((unsigned int *)0x400253FC) = 0x04;
    }
    else
    {
      *((unsigned int *)0x400253FC) = 0x00; // nothing - off
    }
    count += 1;
  }

  return 0;
}
