#ifndef _GLBVAR_H_
#define _GLBVAR_H_


#define vc8051LDROMPageErase   0x65
#define vc8051LDROMByteProgram 0x61
#define vc8051LDROMByteRead		 0x40

////////////////khai bao functions protype/////////////
//void fn8051ITRVector1();
//void fn8051ITRRegSettup ();
//void fn8051SYSDelay(signed int );
//void fn8051SYSModeselect();
//void fn8051DS1307timedisplay();
//void fn8051DS1307timesetup();
//void fn8051APROMwriteTime();
//void fn8051APROMReadandDisplay();
//void fn80517segledFloatdisplay(float);
//void fn8051ADCAdcmain();
//void fn8051APROMWriteADC();
//void fn8051APROMwrite(unsigned char,signed char );
//void fn80517segledSenddt595(unsigned char ,signed char ,char  );
//void fn8051PWMPwm0setup();
//signed char fn8051ButtonBTread();
//unsigned char fn8051APROMread(signed char vruc80517APROMaddress);
//void fn8051ADCRead_bandgap();

///////////////khai bao bien toan cuc//////////////
//const SEC   = 	0x00;
//const MIN   = 	0x01;
//const HOUR  = 	0x02;
//bit BIT_TMP;
//unsigned char vraGlb80517segledcharacter[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
//unsigned char  vrucGlb8051SYSChangemode =0,vrucGlb8051SYSCurrentmode = 0;
//unsigned char vrcGlb8051DS1307Sec,vrcGlb8051DS1307Hour,vrcGlb8051DS1307Min;
unsigned char vrucGlb8051DS1307Led_ctrlindex = 4;
//unsigned char vrucGlb80517LDROMdata;
float vrfGlb8051ADCVadcP05,vrfGlb8051ADCBandgap_Voltage;
unsigned int testnumber = 0;
#endif
