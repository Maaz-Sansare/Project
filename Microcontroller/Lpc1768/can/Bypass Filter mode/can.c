


typedef struct
{
	uint32_t id;	
    uint8_t  Format; /**< Standard Id Format 0 
                          Extended Id Format 1  **/
	uint32_t Frame;  /**< Data frame 0 
                          Remote frame 1 **/
	uint8_t Data[8];
    uint8_t length;
} CanMsg;

typedef enum {
    Can_Mode_Operating = 0,                  /**<  Normal operation mode  */
    Can_Mode_Reset,                          /**<  Reset mode  */
    Can_Mode_ListenOnly,                     /**<  Lisent only mode  */
    Can_Mode_Selftest,                       /**<  Selftest mode  */
    Can_Mode_Txpriority,                     /**<  Txpriority mode  */
    Can_Mode_Sleep,                          /**<  Sleep mode  */
    Can_Mode_Rxpriority,                     /**<  Rxpriority mode  */
    Can_Mode_Test 					         /**<  Test mode  */
} CanMode_t;

CanMsg Rxmessage;

uint32_t Can_ReceiveMsg(LPC_CAN_TypeDef *can, CanMsg *message)
{
	uint32_t data;
	if((can->SR &0x00000001)){
		message->Format   = (uint8_t)(((can->RFS) & 0x80000000)>>31);
		message->Frame    = (uint8_t)(((can->RFS) & 0x40000000)>>30);
		message->length   = (uint8_t)(((can->RFS) & 0x000F0000)>>16);
		message->id = can->RID;

		if (message->Frame == 0){
            //Data frame
			data = can->RDA;
			*((uint8_t *) &message->Data[0])= data & 0x000000FF;
			*((uint8_t *) &message->Data[1])= (data & 0x0000FF00)>>8;;
			*((uint8_t *) &message->Data[2])= (data & 0x00FF0000)>>16;
			*((uint8_t *) &message->Data[3])= (data & 0xFF000000)>>24;
			data = can->RDB;
			*((uint8_t *) &message->Data[4])= data & 0x000000FF;
			*((uint8_t *) &message->Data[5])= (data & 0x0000FF00)>>8;
			*((uint8_t *) &message->Data[6])= (data & 0x00FF0000)>>16;
			*((uint8_t *) &message->Data[7])= (data & 0xFF000000)>>24;

		    can->CMR = 0x04;
		}
		else{
			can->CMR = 0x04; 
			return 1;
		}
	}
	else{
		return 0;
	}
	return 1;
}

uint32_t Can_SendMessage(LPC_CAN_TypeDef *can , CanMsg *message )
{
	uint32_t data;
    if ( can->SR & (1 << 2)){
        can->TFI1 |= (message->length)<<16;
		if(message->Frame == 1){
			can->TFI1 |= (1<<30); //set bit RTR
		}
		else{
			can->TFI1 &= ~(1<<30);
		}
		if(message->Format == 1){ // Extended Id Format
			can->TFI1 |= (1<<31); //set bit FF
		}
		else{
			can->TFI1 &= ~(1<<31);
		}
    	can->TID1 = message->id;

		data = (message->Data[0]) | (message->Data[1]<<8) | (message->Data[2]<<16) | (message->Data[3] <<24);
    	can->TDA1 = data;
		data = (message->Data[4]) | (message->Data[5]<<8) | (message->Data[6]<<16) | (message->Data[7] <<24);
    	can->TDB1 = data;

    	can->CMR = 0x21;
    	return 1;
    }
    else if ( can->SR & (1 << 10) ){
        can->TFI2 |= (message->length)<<16;
		if(message->Frame == 1){
			can->TFI2 |= (1<<30); //set bit RTR
		}
		else{
			can->TFI2 &= ~(1<<30);
		}
		if(message->Format == 1){ // Extended Id Format
			can->TFI2 |= (1<<31); //set bit FF
		}
		else{
			can->TFI2 &= ~(1<<31);
		}
    	can->TID2 = message->id;
    	can->TDA2 = (message->Data[0]) | (message->Data[1]<<8) | (message->Data[2]<<16) | (message->Data[3] <<24);
    	can->TDB2 = (message->Data[4]) | (message->Data[5]<<8) | (message->Data[6]<<16) | (message->Data[7] <<24);
    	can->CMR = 0x41;
    	return 1;
    }
    else if ( can->SR & (1 << 18) ){	
        can->TFI3 |= (message->length)<<16;
		if(message->Frame == 1){
			can->TFI3 |= (1<<30); //set bit RTR
		}
		else{
			can->TFI3 &= ~(1<<30);
		}
		if(message->Format == 1){ // Extended Id Format
			can->TFI3 |= (1<<31); //set bit FF
		}
		else{
			can->TFI3 &= ~(1<<31);
		}
    	can->TID3 = message->id;
    	can->TDA3 = (message->Data[0]) | (message->Data[1]<<8) | (message->Data[2]<<16) | (message->Data[3] <<24);
    	can->TDB3 = (message->Data[4]) | (message->Data[5]<<8) | (message->Data[6]<<16) | (message->Data[7] <<24);
    	can->CMR = 0x81;
    	return 1;
    }
    return  0;
}

void filter_set(uint32_t mask){
    uint32_t address = 0;
    uint32_t i;
    uint32_t ID_high, ID_low;

    /* Set explicit standard Frame */ 
    LPC_CANAF->SFF_sa = address;
    for ( i = 0; i < 4; i += 2 )
    {
	    ID_low = (i << 29) | (0x100 << 16);
	    ID_high = ((i+1) << 13) | (0x100 << 0);
	    *((volatile uint32_t *)(LPC_CANAF_RAM_BASE + address)) = ID_low | ID_high;
	    address += 4; 
    }
}

void Can_Mode(LPC_CAN_TypeDef *can ,CanMode_t mode){
    switch(mode)
	{
	    case Can_Mode_Operating:
	    	can->MOD = 0x00;
	    	break;
	    case Can_Mode_Reset:
	    	can->MOD = 0x01;
	    	break;
	    case Can_Mode_ListenOnly:
	    	can->MOD |= 0x01;//Enter Reset mode
	    	can->MOD |= (1<<1);
	    	can->MOD &= ~0x01;//Release Reset mode
	    	break;
	    case Can_Mode_Selftest:
	    	can->MOD |= 0x01;//Enter Reset mode
	    	can->MOD |= (1<<2);
	    	can->MOD &= ~0x01;//Release Reset mode
	    	break;
	    case Can_Mode_Txpriority:
	    	can->MOD |= (1<<3);
	    	break;
	    case Can_Mode_Sleep:
	    	can->MOD |= (1<<4);
	    	break;
	    case Can_Mode_Rxpriority:
	    	can->MOD |= (1<<5);
	    	break;
	    case Can_Mode_Test:
	    	can->MOD |= (1<<7);
	    	break;
	}
}

void Can_Bitrate(LPC_CAN_TypeDef *can ,uint32_t baudrate){
    uint32_t br = 0;
	uint8_t NT, TSEG1, TSEG2, BRFail;
	uint32_t BRP;

    uint32_t fpclk = 104000000/4;
	br = fpclk / baudrate;

	BRFail = 1;
	for( NT=24; NT>0; NT=NT-2)
	{
		if ((br%NT) == 0)
		{
			BRP = br / NT - 1;
			NT--;
			TSEG2 = (NT/3) - 1;
			TSEG1 = NT -(NT/3) - 1;
			BRFail = 0;
			break;
		}
	}
	if(BRFail){
        while(1); 
    }

	can->MOD = 1;
	can->BTR  = (TSEG2<<20)|(TSEG1<<16)|(3<<14)|BRP;

	can->MOD = 0;
}

void Can_Init(LPC_CAN_TypeDef *can , uint32_t br ,CanMode_t mode){

    if(mode == Can_Mode_Selftest){
        if(can == LPC_CAN1){
            LPC_SC->PCONP |= (1<<13);
        }else{
            LPC_SC->PCONP |= (1<<14); 
        }
    }else{
        if(can == LPC_CAN1){
            LPC_SC->PCONP |= (1<<13);
            LPC_PINCON->PINSEL1 &= ~0x00000F00;  
            LPC_PINCON->PINSEL1 |= (0x3 << 10);	 // Port 0 Pin 21 RD1 
            LPC_PINCON->PINSEL1 |= (0x3 << 12);  // Port 0 Pin 22 RD2
        }else{
            LPC_SC->PCONP |= (1<<14);
            LPC_PINCON->PINSEL0 &= ~0x00003C00;  
            LPC_PINCON->PINSEL0 |= (0x2 << 8);	 // Port 0 Pin 4 RD2
            LPC_PINCON->PINSEL0 |= (0x2 << 10);  // Port 0 Pin 5 RD2
        }        
    }
    
    can->MOD = 1;    /* Reset CAN */
    can->IER = 0;    /* Disable Receive Interrupt */
    can->GSR = 0;    /* Reset error counter when CANxMOD is in reset	*/

    can->CMR = (1 << 1) | (1 << 2) | (1 << 3) ;
    uint32_t temp = can->ICR;
	(void)temp;
    can->MOD = 0x0;  /* CAN in normal operation mode */
 
    //Reset CANAF value
	LPC_CANAF->AFMR = 0x01;
	for (int i = 0; i < 512; i++) {
		LPC_CANAF_RAM->mask[i] = 0x00;
	}

	LPC_CANAF->SFF_sa = 0x00;
	LPC_CANAF->SFF_GRP_sa = 0x00;
	LPC_CANAF->EFF_sa = 0x00;
	LPC_CANAF->EFF_GRP_sa = 0x00;
	LPC_CANAF->ENDofTable = 0x00;

	LPC_CANAF->AFMR = 0x00;

    Can_Bitrate(can,br);
    Can_Mode(can,mode);
	NVIC_EnableIRQ(CAN_IRQn);
    can->IER = 0x01; /* Enable receive interrupts */
	can->IER |= (1<<1); /* Enable receive interrupts */
}

void CAN_IRQHandler(void)  
{	
	LPC_CAN_TypeDef * can;	
    if(LPC_CANCR->CANRxSR & (1 << 8)){
		can = LPC_CAN1;
    }else{
		can = LPC_CAN2;
	}
    if ( can->SR & (1 << 0)){
    	Can_ReceiveMsg(can,&Rxmessage);
    }
    return;
}