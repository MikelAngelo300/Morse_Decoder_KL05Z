#include "klaw.h"

void Klaw_Init(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;		
	PORTA->PCR[S2] |= PORT_PCR_MUX(1);
	PORTA->PCR[S3] |= PORT_PCR_MUX(1);
	PORTA->PCR[S2] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTA->PCR[S3] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

}
void Klaw_S2_3_Int(void)
{
	PORTA -> PCR[S2] |= PORT_PCR_IRQC(0xb);
	PORTA -> PCR[S3] |= PORT_PCR_IRQC(0xb);		
	NVIC_SetPriority(PORTA_IRQn, 3); 
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTA_IRQn);
}
