#define SYSCTL_RCGCGPIO_R (*((volatile unsigned long *)0x400FE608))
#define SYSCTL_RCGCADC_R (*((volatile unsigned long *)0x400FE638))

#define GPIO_PORTF_DIR_R (*((volatile unsigned long *)0x40025400))      // PORT F
#define GPIO_PORTF_DEN_R (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_DATA_R (*((volatile unsigned long *)0x400253FC))

#define GPIO_PORTE_AFSEL_R (*((volatile unsigned long *)0x40024420))    // PORT E
#define GPIO_PORTE_DIR_R (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AMSEL_R (*((volatile unsigned long *)0x40024528))

#define ADC0_ACTSS_R (*((volatile unsigned long *)0x40038000))          // ADC
#define ADC0_EMUX_R (*((volatile unsigned long *)0x40038014))
#define ADC0_SSMUX3_R (*((volatile unsigned long *)0x400380A0))
#define ADC0_SSCTL3_R (*((volatile unsigned long *)0x400380A4))
#define ADC0_PSSI_R (*((volatile unsigned long *)0x40038028))
#define ADC0_RIS_R (*((volatile unsigned long *)0x40038004))
#define ADC0_SSFIFO3_R (*((volatile unsigned long *)0x400380A8))
#define ADC0_ISC_R (*((volatile unsigned long *)0x4003800C))

int main()
{
  // Register settings
  SYSCTL_RCGCGPIO_R = (1 << 4) | (1 << 5); // clock gating portE
  SYSCTL_RCGCADC_R |= 1;                   // analog clock gating
  GPIO_PORTF_DIR_R = 0xFF;                 // portF direction register
  GPIO_PORTF_DEN_R = 0xFF;                 // portF digital function register
  GPIO_PORTE_AFSEL_R |= 8;                 // enable alternate function
  GPIO_PORTE_DIR_R &= ~8;                  // disable digital function
  GPIO_PORTE_AMSEL_R |= 8;                 // enable analog function
  ADC0_ACTSS_R &= ~8;                      // disable SS3 during configuration
  ADC0_EMUX_R &= ~0xF000;                  // software trigger conversion
  ADC0_SSMUX3_R = 0;                       // get input from channel 0
  ADC0_SSCTL3_R |= 6;                      // take one sample at a time, set flag at 1st sample
  ADC0_ACTSS_R |= 8;                       // enable ADC0 sequencer 3

  // ADC
  volatile int result;

  while (1)
  {
    ADC0_PSSI_R |= 8;          // start a conversion sequence 3
    if ((ADC0_RIS_R & 8) == 0) // wait for conversion complete
    {
      result = ADC0_SSFIFO3_R; // read conversion result
      if (result < 2000)
        GPIO_PORTF_DATA_R = 0x00;
      else if (2000 <= result && result < 3000)
        GPIO_PORTF_DATA_R = 0x02;
      else if (3000 <= result && result < 4000)
        GPIO_PORTF_DATA_R = 0x04;
      else if (result > 4000)
        GPIO_PORTF_DATA_R = 0x08;
    }
    ADC0_ISC_R = 8; // clear completion flag
  }

  return 0;
}
