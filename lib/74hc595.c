#include "N76E003.h"
//#include "Common.h"
//#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "port.h"
//#include "glbvar.h"
#include "..\Include\74hc595.h"

void fn80517segledSenddt595(unsigned char vruc80517segleddata,signed char vrc80517segselectled ){ //hien thi len led 7 thanh
		unsigned char i,Q;
		switch (vrc80517segselectled){
			case 1:
				pnobl80517segledCtr_L1 = 1;
				pnobl80517segledCtr_L2 = 0;
				pnobl80517segledCtr_L3 = 0;
				pnobl80517segledCtr_L4 = 0;
				break;
			case 2:
				pnobl80517segledCtr_L1 = 0;
				pnobl80517segledCtr_L2 = 1;
				pnobl80517segledCtr_L3 = 0;
				pnobl80517segledCtr_L4 = 0;
			  break;
			case 3:
				pnobl80517segledCtr_L1 = 0;
				pnobl80517segledCtr_L2 = 0;
				pnobl80517segledCtr_L3 = 1;
				pnobl80517segledCtr_L4 = 0;
			  break;
			case 4:
				pnobl80517segledCtr_L1 = 0;
				pnobl80517segledCtr_L2 = 0;
				pnobl80517segledCtr_L3 = 0;
				pnobl80517segledCtr_L4 = 1;
			  break;
		}
		Q = vruc80517segleddata;
		for(i=0;i<8;i++){
			pnoGlb80517segmentSDI = Q & (0x80>>i);
			pnoGlb80517segmentCLK = 0;
			pnoGlb80517segmentCLK = 1;
			} 
    pnoGlb80517segmentSTR = 0; 
		pnoGlb80517segmentSTR = 1; //Chot hien thi du lieu
}