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

volatile int down=0;

void RIT_IRQHandler (void)
{					
	static int J_up=0;
	static int J_down=0;
	static int position=0;	
	static int value =0;
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		/* Joytick UP pressed */
		J_up++;
		switch(J_up){
			case 1:
			  value++;
				LED_Out(value);
				break;
		
			default:
				if(J_up %8 == 0){
					value++;
				  LED_Out(value);
				}
			
			
				break;
		}
	}
	else{
			J_up=0;
	}
	
  if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
		/* Joytick DOWN pressed */
		J_down++;
		switch(J_down){
			case 1:
			  value--;
				LED_Out(value);
				break;
			
			default:
				if(J_down %8 == 0){
					value--;
				  LED_Out(value);
				}
			
			
			
				break;
		}
	}
	else{
			J_down=0;
	}
	
	/* button management */
	if(down>=1){ 
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
			switch(down){				
				case 2:				/* pay attention here: please see slides to understand value 2 */
				if( position == 7){
					LED_On(0);
					LED_Off(7);
					position = 0;
				}
				else{
					LED_Off(position);
					LED_On(++position);
				}
					break;
				default:
					break;
			}
			down++;
		}
		else {	/* button released */
			down=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
/*	else{
			if(down==1)
				down++;
	} */
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
