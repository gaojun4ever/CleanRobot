#include "stm32f4xx.h"
#include "serial.h"


int main()
{
  Serial_Init();
  for(;;)
  {
		printf("hellow world\n");
  }

}
