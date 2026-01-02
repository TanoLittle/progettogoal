/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "../led/led.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
#define N 10
extern int traduzione_morse(char*, char*);
char vett_in[]= {0,0,0,0,2,1,1,1,2,0,1,0,0,2,0,1,3,1,1,2,0,0,1,2,1,0,2,1,0,0,2,1,1,1,3,0,1,1,1,1,2,0,0,1,1,1,4};
char vett_out[100];
char vett_in2 [100];	
	
volatile int down_0 = 0;
volatile int down_1 = 0;
volatile int down_2 = 0;

volatile int Acquisition = 1;

void RIT_IRQHandler (void)
{			
/* Static as its value persists between calls to the function. It is not reinitialized each time the function is executed.*/
	static int J_down = 0;
	static int J_Up = 0;
	static int pointer = 0;
	int RES;
	static int J_left = 0;
	static int J_right = 0;
	
	
if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
		/* Joytick Select pressed p1.25*/
		/* Joytick Down pressed p1.26 --> using J_DOWN due to emulator issues*/
		
		J_down++;
		switch(J_down){
			case 1:
				vett_in2[pointer++]= (char)1;
			
				if (pointer == 100)
				{
					pointer =0;
				}
				LED_Out(15);
				enable_timer(1);
			
				break;
			default:
				break;
		}
	}
	else{
			J_down=0;
	}

	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
		/* Joytick Select pressed p1.25*/
		/* Joytick Down pressed p1.27 --> using J_left due to emulator issues*/
		
		J_left++;
		switch(J_left){
			case 1:
				vett_in2[pointer++]= (char)3;
			
				if (pointer == 100)
				{
					pointer =0;
				}
				break;
			default:
				break;
		}
	}
	else{
			J_left=0;
	}

	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
		/* Joytick Select pressed p1.25*/
		/* Joytick Down pressed p1.28 --> using J_right */
		
		J_right++;
		switch(J_right){
			case 1:
				vett_in2[pointer++]= (char)2;
			
				if (pointer == 100)
				{
					pointer =0;
				}
			
				break;
			default:
				break;
		}
	}
	else{
			J_right=0;
	}

	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0 ){	
		/* Joytick Up pressed p1.29 --> using J_Up */

		J_Up++;
		switch(J_Up){
			case 1:
			// Data acquisition
				vett_in2[pointer++]= (char)0;
			
				if (pointer == 100)
				{
					pointer =0;
				}
				LED_Out(1);
				enable_timer(1);
				
			
				break;
				
			default:
				break;
		}
	}
	else{
			J_Up=0;
	}
/*************************INT0***************************/
if(down_0 !=0){
	down_0++;
	if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
		switch(down_0){
			case 2:
				break;
			default:
				break;
		}
	}
	else {	/* button released */
		down_0=0;			
		NVIC_EnableIRQ(EINT0_IRQn);							 /* disable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
	}
} // end INT0

/*************************KEY1***************************/
if(down_1 !=0){
	down_1++;
	if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){
		switch(down_1){
			case 2:				/* pay attention here: please see slides to understand value 2 */
				vett_in2[pointer++]= (char)4;
			
				if (pointer == 100)
				{
					pointer =0;
				}
				
				RES = traduzione_morse(vett_in,vett_out);
				LED_Out(RES);
				reset_timer(0);
				enable_timer(0);
				/*Restart the system operation*/
				pointer =0;
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_1=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}


/*************************KEY2***************************/
if(down_2 !=0){
	down_2++;
	if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){
		switch(down_2){
			case 2:
				
				break;
			default:
				break;
		}
	}
	else {	/* button released */
		down_2=0;		
		NVIC_EnableIRQ(EINT2_IRQn);							 /* disable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
	}
} // end KEY2
	
	reset_RIT();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
