#include "add/lpc17xx.h"
#include "can.c"

CanMsg Txmessage;

int main(){
    Can_Init(LPC_CAN2,125000,Can_Mode_Operating);
    Can_Init(LPC_CAN1,125000,Can_Mode_Operating);
    
    Txmessage.id = 0x00001234;
    Txmessage.Format = 1;
    Txmessage.Frame = 0;
    Txmessage.length = 7;
    Txmessage.Data[0] = 'S';
    Txmessage.Data[1] = 'A';
    Txmessage.Data[2] = 'I';
    Txmessage.Data[3] = 'F';
    Txmessage.Data[4] = 'A';
    Txmessage.Data[5] = 'A';
    Txmessage.Data[6] = 'N';
    Txmessage.Data[7] = 0;

    LPC_CANAF->AFMR = (1<<1);
    while(1){
        Can_SendMessage(LPC_CAN2,&Txmessage);
        if(LPC_CAN2->MOD & (1 << 2)){
           LPC_CAN2->CMR |=(1<<4); 
        }
        
    }
    return 0;
}
