#include "tm4c123gh6pm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//COLORS
#define BLACK 0x00U
#define RED 0x02U
#define BLUE 0x04U
#define GREEN 0x08U
#define PURPLE 0x06U
#define YELLOW 0x0AU
#define CYAN 0x0CU
#define WHITE 0x0EU

//DELAY
void delayMs(int n)
{
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < 3180; ++j)
      ;
}

char readChar(int uart)
{
  char c;
  if (uart == 0)
  {
    while ((UART0_FR_R & (1 << 4)) != 0)
      ;
    c = UART0_DR_R;
  }
  else if (uart == 5)
  {
    while ((UART5_FR_R & (1 << 4)) != 0)
      ;
    c = UART5_DR_R;
  }
  else if (uart == 7)
  {
    while ((UART7_FR_R & (1 << 4)) != 0)
      ;
    c = UART7_DR_R;
  }
  return c;
}

void printChar(char c, int uart)
{
  if (uart == 0)
  {
    while ((UART0_FR_R & (1 << 5)) != 0)
      ;
    UART0_DR_R = c;
  }
  else if (uart == 5)
  {
    while ((UART5_FR_R & (1 << 5)) != 0)
      ;
    UART5_DR_R = c;
  }
  else if (uart == 7)
  {
    while ((UART7_FR_R & (1 << 5)) != 0)
      ;
    UART7_DR_R = c;
  }
}

void printString(char *string, int uart)
{
  while (*string)
  {
    printChar(*(string++), uart);
  }
}

char *readString(char delimiter, int uart)
{
  int stringSize = 0;
  char *string = (char *)calloc(10, sizeof(char));
  char c = readChar(uart);
  printChar(c, uart);

  while (c != delimiter)
  {

    *(string + stringSize) = c; // put the new character at the end of the array
    stringSize++;

    if ((stringSize % 10) == 0) // string length has reached multiple of 10
    {
      string = (char *)realloc(string, (stringSize + 10) * sizeof(char)); // adjust string size by increasing size by another 10
    }

    c = readChar(uart);
    printChar(c, uart); // display the character the user typed
  }

  if (stringSize == 0)
  {
    return "\0"; // null character
  }
  return string;
}

//------------------------------------------MAIN

int main()
{
  /** PORT F CONFIG **/
  SYSCTL_RCGCGPIO_R |= 0x20U;      // enable clock on PortF
  GPIO_PORTF_DIR_R = 0x0EU;        // make LED pins (PF1, PF2, and PF3) outputs
  GPIO_PORTF_DEN_R = 0x1FU;        // enable digital function on LED pins
  GPIO_PORTF_DATA_R &= ~(0x0EU);   // turn off leds
  GPIO_PORTF_AMSEL_R = 0x00;       // disable analog on PF
  GPIO_PORTF_AFSEL_R = 0x00;       // disable alt funct on PF7-0
  GPIO_PORTF_CR_R = 0xFFU;         // Allow Changes to GPIO Port F
  GPIO_PORTF_PUR_R = 0x11U;        // Enable Pull Up on PF4 & PF0
  GPIO_PORTF_LOCK_R = 0x4C4F434BU; // Unlock GPIO Port F
  GPIO_PORTF_PCTL_R = 0x00000000;  // PCTL GPIO on PF4-0

  /** UART CONFIG **/
  // Enable the UART module using the RCGCUART register.
  SYSCTL_RCGCUART_R |= (1 << 0); // UART0
  SYSCTL_RCGCUART_R |= 0x20;     // UART5
  SYSCTL_RCGCUART_R |= 0x80;     // UART7

  // Enable the clock to the appropriate GPIO module via the RCGCGPIO register (see page 340).
  // To find out which GPIO port to enable, refer to Table 23-5 on page 1351.
  SYSCTL_RCGCGPIO_R |= (1 << 0);

  // Set the GPIO AFSEL bits for the appropriate pins.
  GPIO_PORTA_AFSEL_R = (1 << 1) | (1 << 0);

  // Configure the GPIO current level and/or slew rate as specified for the mode selected (see
  // page 673 and page 681

  // Configure the PMCn fields in the GPIOPCTL register to assign the UART signals to the appropriate
  // pins (see page 688 and Table 23-5 on page 1351).
  GPIO_PORTA_PCTL_R = (1 << 0) | (1 << 4);

  GPIO_PORTA_DEN_R = (1 << 0) | (1 << 1);

  // Find  the Baud-Rate Divisor
  // BRD = 16,000,000 / (16 * 9600) = 104.1666
  // UARTFBRD[DIVFRAC] = integer(0.166667 * 64 + 0.5) = 11

  // With the BRD values in hand, the UART configuration is written to the module in the following order

  // Disable the UART by clearing the UARTEN bit in the UARTCTL register
  UART0_CTL_R &= ~(1 << 0);
  UART5_CTL_R &= ~(1 << 0);
  UART7_CTL_R &= ~(1 << 0);

  // Write the integer portion of the BRD to the UARTIBRD register
  UART0_IBRD_R = 104;
  UART5_IBRD_R = 104;
  UART7_IBRD_R = 104;
  // Write the fractional portion of the BRD to the UARTFBRD register.
  UART0_FBRD_R = 11;
  UART5_FBRD_R = 11;
  UART7_FBRD_R = 11;

  // Write the desired serial parameters to the UARTLCRH register (in this case, a value of 0x0000.0060)
  UART0_LCRH_R = (0x3 << 5) | (1 << 4); // 8-bit, no parity, 1-stop bit
  UART5_LCRH_R = (0x3 << 5) | (1 << 4); // 8-bit, no parity, 1-stop bit
  UART7_LCRH_R = (0x3 << 5) | (1 << 4); // 8-bit, no parity, 1-stop bit

  // Configure the UART clock source by writing to the UARTCC register
  UART0_CC_R = 0x0;
  UART5_CC_R = 0x0;
  UART7_CC_R = 0x0;

  // Enable the UART by setting the UARTEN bit in the UARTCTL register.
  UART0_CTL_R = (1 << 0) | (1 << 8) | (1 << 9);
  UART5_CTL_R = (1 << 0) | (1 << 8) | (1 << 9);
  UART7_CTL_R = (1 << 0) | (1 << 8) | (1 << 9);

  // UART5 TX5 and RX5 use PE4 and PE5. Configure them digital and enable alternate function
  SYSCTL_RCGCGPIO_R |= 0x10;
  GPIO_PORTE_DEN_R = 0x33;         // set PE4 and PE5 as digitasl
  GPIO_PORTE_AFSEL_R = 0x33;       // Use PE4,PE5 alternate function
  GPIO_PORTE_AMSEL_R = 0;          // Turn off analog function
  GPIO_PORTE_PCTL_R |= 0x00110011; // configure PE4 and PE5 for UART
  GPIO_PORTE_CR_R = 0x2F;          // allow changes to PE5,3-0
  GPIO_PORTE_DIR_R |= 0x08;        // PE3 input
  GPIO_PORTE_DIR_R &= ~0x07;       // PE2,PE1,PE0 output
  GPIO_PORTE_PUR_R = 0x08;         // enable pullup resistors on PE3

  // Question Functions
  char *questions[] = {"Are you there in your office?", "Are you happy?", "Are you hungry, want to have lunch?", "Are you thirsty, want to meet for a beverage?", "Shall I come to your office to talk?"};
  int question_colors[] = {RED, YELLOW, GREEN, BLUE, CYAN};
  char question_encoded[] = {(char)0x30, (char)0x31, (char)0x32, (char)0x33, (char)0x34};

  // Answer functions
  char *answers[] = {"Yes", "Maybe", "No"};
  int answer_colors[] = {WHITE, PURPLE, BLACK};
  char answer_encoded[] = {(char)0x35, (char)0x36, (char)0x37};

  // Variables
  int ques_counter = 0;
  int ans_counter = 0;
  int controller = 0; // decides which UART is active
  unsigned int leftState, rightState;
  GPIO_PORTF_DATA_R = question_colors[ques_counter];

  while (1)
  {
    leftState = GPIO_PORTF_DATA_R & 16;
    rightState = GPIO_PORTF_DATA_R & 1;

    if (leftState == 0)
    {
      // delay for a while to counter bouncing effect
      delayMs(300);

      // Question controller
      if (controller == 0)
      {
        ques_counter++;
        ques_counter %= (sizeof(question_colors) / sizeof(question_colors[0]));
        GPIO_PORTF_DATA_R = question_colors[ques_counter];
      }
      // Answer controller
      else
      {
        ans_counter++;
        ans_counter %= (sizeof(answer_colors) / sizeof(answer_colors[0]));
        GPIO_PORTF_DATA_R = answer_colors[ans_counter];
      }
    }
    else if (rightState == 0)
    {
      // delay for a while to counter bouncing effect
      delayMs(300);

      // Send question
      if (controller == 0)
      {
        // Send question as encoded
        printChar(question_encoded[ques_counter], 5);

        // Recieve question as encoded
        char val = readChar(7);
        int idx = val - '0';

        // Show output
        printString("Sent question by 5: ", 0);
        printString(questions[ques_counter], 0);
        printString("\n\r", 0);
        printString("Recieved question by 7: ", 0);
        printString(questions[idx], 0);
        printString("\n\r", 0);
      }
      // Send answer
      else if (controller == 1)
      {
        // Send answer as encoded
        printChar(answer_encoded[ans_counter], 7);

        // Recieve answer as encoded
        char val = readChar(5);
        int idx = val - '5';

        // Show output
        printString("Sent answer by 7: ", 0);
        printString(answers[ans_counter], 0);
        printString("\n\r", 0);
        printString("Recieved answer by 5: ", 0);
        printString(answers[idx], 0);
        printString("\n\r", 0);
      }
      // Change controller
      controller = 1 - controller;
      // Reset variables
      ques_counter = 0;
      ans_counter = 0;
      GPIO_PORTF_DATA_R = BLACK;
    }
  }

  return 0;
}
