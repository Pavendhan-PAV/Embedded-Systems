#define SYSCTL_RCGCGPIO_R (*((volatile unsigned long *)0x400FE608))
#define GPIO_PORTF_DATA_RD (*((volatile unsigned long *)0x40025040))
#define GPIO_PORTF_DATA_WR (*((volatile unsigned long *)0x40025038))
#define GPIO_PORTF_DIR_R (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_DEN_R (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_PUR_R (*((volatile unsigned long *)0x40025510))

#define SYSCTL_RCGC2_GPIOF 0x0020
#define GPIO_PORTF_PIN3_EN 0x0E
#define GPIO_PORTF_PIN4_EN 0x10
#define SYSTEM_CLOC_FREQUENCY 16000000

#define DELAY_DEBOUNCE SYSTEM_CLOC_FREQUENCY / 1000

void Delay(unsigned long counter)
{
	unsigned long i = 0;
	for (i = 0; i < counter; i++)
		;
}

int main()
{
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGC2_GPIOF;
	GPIO_PORTF_DEN_R |= GPIO_PORTF_PIN3_EN + GPIO_PORTF_PIN4_EN;
	GPIO_PORTF_DIR_R |= GPIO_PORTF_PIN3_EN;
	GPIO_PORTF_DIR_R &= (~GPIO_PORTF_PIN4_EN);
	GPIO_PORTF_PUR_R |= GPIO_PORTF_PIN4_EN;

	while (1)
	{
		if (GPIO_PORTF_DATA_RD == 0)
		{
			Delay(DELAY_DEBOUNCE);
			if (GPIO_PORTF_DATA_WR >= 0x0EU)
				GPIO_PORTF_DATA_WR = 0x00U;
			else
				GPIO_PORTF_DATA_WR += 0x02;

			GPIO_PORTF_DATA_RD = 1;
		}
		else
		{
			GPIO_PORTF_DATA_RD = 0;
		}
	}
}