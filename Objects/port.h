#ifndef _PORT_H_
#define _PORT_H_
		sbit pnoGlb80517segmentCLK 	= P0^4; //Chan clock /shcp
		sbit pnoGlb80517segmentSDI	= P0^2;//Chan 14data /ds
		sbit pnoGlb80517segmentSTR 	= P0^3;//Chan 12 chot
		sbit pnobl80517segledCtr_L1 = P1^2;
		sbit pnobl80517segledCtr_L2 =	P1^3;
		sbit pnobl80517segledCtr_L3 =	P1^4;
		sbit pnobl80517segledCtr_L4 =	P1^6;
		sbit pnobl8051ledalert 			= P1^1;
		sbit pni8051ButtonDown   		= P0^0;
		sbit pni8051ButtonSeclect   = P1^0;
		sbit pni8051ButtonUp  			= P1^5; 
	
#endif