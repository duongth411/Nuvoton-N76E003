
#include "main.h"

void fn8051ADCBypass (void){ 
		unsigned char i;
		for (i=0;i<0x03;i++){
				
				clr_ADCF;
				set_ADCS;
				while(ADCF == 0);
		}
}
void fn8051I2CStart(void){
		SCL = 1; SCL = 0;
		SDA = 1; SCL = 1;
		fn8051SYSDelay(1);
		SDA = 0; fn8051SYSDelay(1);
		SCL = 0; fn8051SYSDelay(1);
}
void fn8051I2CStop(void){
		SCL = 1; SCL = 0;
		SDA = 0; fn8051SYSDelay(1);
		SCL = 1; fn8051SYSDelay(1);
		SDA = 1;
}
bit fn8051I2CWrite(unsigned char dat){
		unsigned char i;
		for (i=0;i<8;i++){
				SDA = (dat & 0x80) ? 1:0;
				SCL=1;SCL=0;
				dat<<=1;
			}
		SCL = 1; fn8051SYSDelay(1);
		SCL = 0;
		return dat;
}
unsigned char fn8051I2CRead(void){
		bit rd_bit;
		unsigned char i, dat;
		dat = 0x00;
		for(i=0;i<8;i++) 										/* For loop read data 1 byte */
		{
				fn8051SYSDelay(1);
				SCL = 1; fn8051SYSDelay(1); 				/* Set SCL */
				rd_bit = SDA; 										/* Keep for check acknowledge */
				dat = dat<<1;
				dat = dat | rd_bit; 							/* Keep bit data in dat */
				SCL = 0; 													/* Clear SCL */
		}
		return dat;
}
void fn8051DS1307Out_1Hz()
{
	fn8051I2CStart(); /* Start i2c bus */
	fn8051I2CWrite(0xD0); /* Connect to DS1307 */
	fn8051I2CWrite(0x07);   //RTC pointer location
	fn8051I2CWrite(0x10);
	fn8051I2CStop();
}
unsigned char fn8051DS1307Readdata(unsigned char addr)
{
		unsigned int temp,ret;
		fn8051I2CStart(); 		/* Start i2c bus */
		fn8051I2CWrite(0xD0); 	/* Connect to DS1307 */
		fn8051I2CWrite(addr); 	/* Request RAM address on DS1307 */
		fn8051I2CStart(); 		/* Start i2c bus */
		fn8051I2CWrite(0XD1); 	/* Connect to DS1307 for Read */
		ret = fn8051I2CRead(); 	/* Receive data */
		fn8051I2CStop();
		//**********************************************************
		temp = ret; 							/*BCD to HEX*/
		ret = (((ret/16)*10)+ (temp & 0x0f)); 	/*for Led 7seg*/
		//**********************************************************
		return ret;
}
void fn8051DS1307Writedata(unsigned char addr,unsigned char dat)
{
		unsigned int temp;
		//********************************************** 
		temp = dat ; 						/*HEX to BCD*/
		dat = (((dat/10)*16)|(temp %10)); 	/*for Led 7seg*/
		//**********************************************
		fn8051I2CStart()
	; 		/* Start i2c bus */
		fn8051I2CWrite(0XD0); 	/* Connect to DS1307 */
		fn8051I2CWrite(addr); 	/* Request RAM address on DS1307 */
		fn8051I2CWrite(dat); 	/* Connect to DS1307 for Read */
		fn8051I2CStop();
}
void fn8051DS1307ReadTime()
{
		vrcGlb8051DS1307Sec  = fn8051DS1307Readdata(SEC);
		vrcGlb8051DS1307Min  = fn8051DS1307Readdata(MIN);
		vrcGlb8051DS1307Hour = fn8051DS1307Readdata(HOUR);
}
void fn8051DS1307WriteTime()
{
		fn8051DS1307Writedata(SEC,vrcGlb8051DS1307Sec);
		fn8051DS1307Writedata(MIN,vrcGlb8051DS1307Min);
		fn8051DS1307Writedata(HOUR,vrcGlb8051DS1307Hour);
}
int main (){
		Set_All_GPIO_Quasi_Mode;
		fn8051ITRRegSettup();
		fn8051DS1307Out_1Hz();
		//fn8051ADCBypass();
		do{
				fn80517segledSenddt595(0x92,1,0);
				fn8051SYSDelay(5);
				fn80517segledSenddt595(0x06,2,0);
				fn8051SYSDelay(5);
				fn80517segledSenddt595(0xc7,3,0);
				fn8051SYSDelay(5);
				fn80517segledSenddt595(0x06,4,0);
				fn8051SYSDelay(5);
				}while(pni8051ButtonSeclect!=0);
				fn8051SYSModeselect();
		while(1){
				if(vrucGlb8051SYSChangemode == 0){
						while(vrucGlb8051SYSCurrentmode==1&&vrucGlb8051SYSChangemode==0){
								fn80517segledFloatdisplay(vrfGlb8051ADCVadcP05);
								if((pni8051ButtonSeclect)==0&(pni8051ButtonDown==0)) {
										vrucGlb8051SYSChangemode = 1;	
									}

					}
				while((vrucGlb8051SYSCurrentmode==2)&(vrucGlb8051SYSChangemode==0)){
								ET1 = 1;
								fn8051DS1307ReadTime();
								if((pni8051ButtonSeclect)==0&(pni8051ButtonDown==0)) {
										vrucGlb8051SYSChangemode = 1;
										ET1 = 0;
											}
								if((pni8051ButtonSeclect)==0&(pni8051ButtonUp==0)) {
										fn8051SYSDelay(500);
										if((pni8051ButtonSeclect)==0&(pni8051ButtonUp==0)){
												fn8051DS1307timesetup();
													}
											}
								if(pni8051ButtonSeclect ==0){
										fn8051SYSDelay(2000);
										if(pni8051ButtonSeclect ==0){
												fn8051APROMwriteTime();
													}
										}	
							}
				while(vrucGlb8051SYSCurrentmode==3&&vrucGlb8051SYSChangemode==0){
						fn8051APROMReadandDisplay();
								if((pni8051ButtonSeclect==0)&(pni8051ButtonDown==0)) {
										vrucGlb8051SYSChangemode = 1;	
									}
								if(pni8051ButtonSeclect ==0){
										fn8051SYSDelay(2000);
										if(pni8051ButtonSeclect ==0){
												fn8051APROMWriteADC();
													}
										}	
							}
				}
				else{
						while(vrucGlb8051SYSChangemode == 1){
									fn8051SYSModeselect();
									}
					}
			}
}	
void fn8051ITRRegSettup (){
		TMOD = 0x11;
		TH0 = 0xC1;
		TL0 = 0x80;
		TH1 = 0xE0;
		TL1 = 0xC0;
		set_T0M;
		ET0 = 1;
		EA  = 1;
		TR0 = 1;
		TF0 = 0; 
		TF1 = 0;
		TR1 = 1;
}
void  fn8051ITRVector1(void) interrupt 1{ //timer 1ms
		unsigned int vri8051LedblinkCount ;
		unsigned int vruc8051ADCAdctimer1 ;
	  vri8051LedblinkCount++;
		if(vri8051LedblinkCount >1000){ //timer 1 s
		pnobl8051ledalert = ~pnobl8051ledalert;
		vri8051LedblinkCount = 0;
		}
		if(vrucGlb8051SYSCurrentmode == 1){
				if(ADCS == 1) {vruc8051ADCAdctimer1 = 0;}
				else{
				vruc8051ADCAdctimer1++;
				if(vruc8051ADCAdctimer1 > 100) {
						fn8051ADCAdcmain();
						vruc8051ADCAdctimer1=0;
						}
			}
		}
		//TF0 = 0;
		TH0 = 0xC1;
		TL0 = 0x80;
}
void  fn8051ITRVector3(void) interrupt 3{ //timer 0,5ms
		unsigned char vruc8051DS1307indexcounter;
			signed char hour1,hour2,min3,min4;
		vruc8051DS1307indexcounter++;
		if(vruc8051DS1307indexcounter>1){
				vrucGlb8051DS1307Led_ctrlindex--;
				vruc8051DS1307indexcounter = 0;
				if(vrucGlb8051DS1307Led_ctrlindex<1){vrucGlb8051DS1307Led_ctrlindex = 4;}
			}
		if(vrcGlb8051DS1307Hour<10){
				hour1 = 0;
				hour2 = vrcGlb8051DS1307Hour;
						}
		else{
				hour1 = vrcGlb8051DS1307Hour/10;
				hour2 = vrcGlb8051DS1307Hour-(hour1*10);                        
						}
		if(vrcGlb8051DS1307Min<10){
				min4 = vrcGlb8051DS1307Min ;
				min3 = 0;
						}
		else{
				min3 = vrcGlb8051DS1307Min/10;
				min4 = vrcGlb8051DS1307Min-(min3*10);                     
						}	
		if (vrucGlb8051DS1307Led_ctrlindex ==1){
				fn80517segledSenddt595(vraGlb80517segledcharacter[hour1],vrucGlb8051DS1307Led_ctrlindex,0);
					}
		if (vrucGlb8051DS1307Led_ctrlindex ==2){
				fn80517segledSenddt595(vraGlb80517segledcharacter[hour2],vrucGlb8051DS1307Led_ctrlindex,1);
					}
		if (vrucGlb8051DS1307Led_ctrlindex ==3){
				fn80517segledSenddt595(vraGlb80517segledcharacter[min3],vrucGlb8051DS1307Led_ctrlindex,0);
					}
		if (vrucGlb8051DS1307Led_ctrlindex ==4){
				fn80517segledSenddt595(vraGlb80517segledcharacter[min4],vrucGlb8051DS1307Led_ctrlindex,1);
					}	
		TH1 = 0xF9;
		TL1 = 0xC0;
}
void fn8051SYSDelay(signed int time){//delay 1ms
	signed int t,c;
	for(c=0;c<time;c++){ 
		 for(t=0;t<1600;t++){         
					}
		}
}		
signed char fn8051ButtonBTread(){
	if (pni8051ButtonDown==0){
		fn8051SYSDelay(100);
		return -1;
		}
	if (pni8051ButtonUp==0){
		fn8051SYSDelay(100);
		return 1;
		}								
	else return 0;
}
void fn8051SYSModeselect(){
	fn8051SYSDelay(200);
	while(pni8051ButtonSeclect!=0){
		vrucGlb8051SYSCurrentmode += fn8051ButtonBTread();
		if (vrucGlb8051SYSCurrentmode>3) {vrucGlb8051SYSCurrentmode = 1;}
		if (vrucGlb8051SYSCurrentmode<1) {vrucGlb8051SYSCurrentmode = 3;}
		fn80517segledSenddt595(vraGlb80517segledcharacter[0],3,1);
		fn8051SYSDelay(5);
		fn80517segledSenddt595(vraGlb80517segledcharacter[vrucGlb8051SYSCurrentmode],4,0);
		fn8051SYSDelay(5);
		}
	vrucGlb8051SYSChangemode = 0;
}
void fn80517segledSenddt595(unsigned char vruc80517segleddata,signed char vrc80517segselectled, char dotpoint ){ //7segment led display by timer
		unsigned char i,Q;
		Q = vruc80517segleddata;
		for(i=0;i<8;i++){
				if((dotpoint==1)&(i==0)){pnoGlb80517segmentSDI=0;}
				else
				pnoGlb80517segmentSDI = Q & (0x80>>i);
				pnoGlb80517segmentCLK = 0;
				pnoGlb80517segmentCLK = 1;
			} 
    pnoGlb80517segmentSTR = 0; 
		pnoGlb80517segmentSTR = 1;
		if(vrc80517segselectled == 1 ){			
				pnobl80517segledCtr_L1 = 1;
				pnobl80517segledCtr_L2 = pnobl80517segledCtr_L3 = pnobl80517segledCtr_L4 = 0;
		}
		if(vrc80517segselectled == 2 ){
				pnobl80517segledCtr_L2 = 1;
				pnobl80517segledCtr_L1 = pnobl80517segledCtr_L3 = pnobl80517segledCtr_L4 = 0;
		}
		if(vrc80517segselectled == 3 ){
				pnobl80517segledCtr_L3 = 1;		
				pnobl80517segledCtr_L1 = pnobl80517segledCtr_L2 = pnobl80517segledCtr_L4 = 0;
		}
		if(vrc80517segselectled == 4 ) 	{
				pnobl80517segledCtr_L4 = 1;
				pnobl80517segledCtr_L1 = pnobl80517segledCtr_L3 = pnobl80517segledCtr_L2 = 0;
		}
}
//void fn8051DS1307timedisplay() {					
//}
void fn8051DS1307timesetup(){
		fn80517segledSenddt595(0xff,1,1);
		vrcGlb8051DS1307Sec  = 0;
		vrcGlb8051DS1307Hour = 12;
		vrcGlb8051DS1307Min  = 30;
		do{
			vrcGlb8051DS1307Min +=fn8051ButtonBTread();
			if(vrcGlb8051DS1307Min>59) { vrcGlb8051DS1307Min=0; }
			if(vrcGlb8051DS1307Min==0) { vrcGlb8051DS1307Min=59; }
			if(pni8051ButtonSeclect==0) {
					fn8051SYSDelay(500);
						}
					}while(pni8051ButtonSeclect!=0);
		do{
			vrcGlb8051DS1307Hour+=fn8051ButtonBTread();
			if(vrcGlb8051DS1307Hour==0) { vrcGlb8051DS1307Hour=23; }
			if(vrcGlb8051DS1307Hour>23){ vrcGlb8051DS1307Hour=0; }
			if(pni8051ButtonSeclect==0){ fn8051SYSDelay(500);  }
					}while(pni8051ButtonSeclect!=0);
			fn8051DS1307WriteTime();
}
void fn8051APROMwrite(unsigned char vruc80517APROMdata,signed char vruc80517APROMaddress ){ //APROM Write
		EA = 0;
		TA = 0xAA;
		TA = 0x55;
		CHPCON |= 0x01;
		TA = 0xAA;
		TA = 0x55;
		IAPUEN |= 0x02; //enable LDROM update
		TA = 0xAA;
		TA = 0x55;
		IAPTRG |= 0x01; //trig IAP GO
		IAPCN = vc8051LDROMByteProgram; 
		IAPAH = 0x00;
		IAPAL = vruc80517APROMaddress;
		IAPFD = vruc80517APROMdata;
		TA = 0xAA;
		TA = 0x55;
		IAPTRG |= 0x01; //trig IAP GO
		TA = 0xAA; //IAPUEN is TA protected
		TA = 0x55;
		IAPUEN &= ~0x02; //disable LDROM update
		TA = 0xAA; //CHPCON is TA protected
		TA = 0x55;
		CHPCON &= ~0x01;// disable IAP 
		EA = 1;
}
unsigned char fn8051APROMread(signed char vruc80517APROMaddress){
		unsigned char Q;
		EA = 0;
		TA = 0xAA;
		TA = 0x55;
		CHPCON |= 0x01;
		TA = 0xAA;
		TA = 0x55;
		IAPTRG |= 0x01;
		IAPCN = vc8051LDROMByteRead; 
		IAPAH = 0x00;
		IAPAL = vruc80517APROMaddress;
		Q = IAPFD;
		TA = 0xAA;
		TA = 0x55;
		IAPTRG |= 0x01;//write ‘1’ to IAPGO to trigger IAP process
		TA = 0xAA; //IAPUEN is TA protected
	  TA = 0x55;
		IAPUEN &= ~0x02; 
		TA = 0xAA; //CHPCON is TA protected
		TA = 0x55;
		CHPCON &= ~0x01; //IAPEN = 0, disable IAP mode
		EA = 1;
		return Q;
}
void fn8051APROMwriteTime(){
			signed char hour1,hour2,min3,min4; 
		if(vrcGlb8051DS1307Hour<10){
				hour1 = 0;
				hour2 = vrcGlb8051DS1307Hour;
						}
		else{
				hour1 = vrcGlb8051DS1307Hour/10;
				hour2 = vrcGlb8051DS1307Hour-(hour1*10);                        
						}
		if(vrcGlb8051DS1307Min<10){
				min4 = vrcGlb8051DS1307Min ;
				min3 = 0;
						}
		else{
				min3 = vrcGlb8051DS1307Min/10;
				min4 = vrcGlb8051DS1307Min-(min3*10);                     
						}
		EA = 0;
		TA = 0xAA; //TA protect
		TA = 0x55;
		CHPCON |= 0x01; //enable IAP
		TA = 0xAA;
		TA = 0x55;
		IAPUEN |= 0x02; //enable LDROM update
		IAPCN = vc8051LDROMPageErase;
		IAPAH = 0x00;
		IAPAL = 0x00;
		IAPFD = 0xff;
		fn8051APROMwrite(hour1,0x00);
		fn8051APROMwrite(hour2,0x01);
		fn8051APROMwrite(min3,0x02);
		fn8051APROMwrite(min4,0x03);
		EA =1 ;	
}
void fn8051APROMReadandDisplay(){
		char i,vrc8051APROMNguyen,vrc8051APROMTPhan1,vrc8051APROMTPhan2;
		unsigned char vrc8051APROMmode =0;
		vrc8051APROMmode+=fn8051ButtonBTread();
		if(vrc8051APROMmode>1) {vrc8051APROMmode = 0;}
		if(vrc8051APROMmode == 0){
			for(i=1;i<5;i++){
					fn80517segledSenddt595(vraGlb80517segledcharacter[fn8051APROMread(0x00+i-1)],i,0);
					fn8051SYSDelay(4);
					}
				}
		if(vrc8051APROMmode == 1){
			vrc8051APROMNguyen = fn8051APROMread(0x04);
			vrc8051APROMTPhan1 = fn8051APROMread(0x05);
			vrc8051APROMTPhan2 = fn8051APROMread(0x06);
			fn80517segledSenddt595(vraGlb80517segledcharacter[vrc8051APROMNguyen],i,1);
			fn8051SYSDelay(4);
			fn80517segledSenddt595(vraGlb80517segledcharacter[vrc8051APROMTPhan1],i,0);
			fn8051SYSDelay(4);
			fn80517segledSenddt595(vraGlb80517segledcharacter[vrc8051APROMTPhan2],i,0);
			fn8051SYSDelay(4);			
		}
		
}
void fn80517segledFloatdisplay(float vrf80517segledFin){
		float Q;
		unsigned char nguyen,tphan1,tphan2;
		Q = vrf80517segledFin;
		nguyen = (unsigned char)(Q/1000);
		tphan1 = (unsigned char)((Q - nguyen*1000)/100);
		tphan2 = (unsigned char)((Q - (nguyen*1000+tphan1*100))/10);
		fn80517segledSenddt595(vraGlb80517segledcharacter[nguyen],2,1);
		fn8051SYSDelay(3);
		fn80517segledSenddt595(vraGlb80517segledcharacter[tphan1],3,0);
		fn8051SYSDelay(3);
		fn80517segledSenddt595(vraGlb80517segledcharacter[tphan2],4,0);
		fn8051SYSDelay(3);
}
void fn8051ADCRead_bandgap(){ //IAP read
		unsigned char BandgapHigh,BandgapLow,BandgapMark;	
		float Bandgap_Value,Bandgap_Voltage_Temp;
		set_IAPEN;
		IAPCN = 0x04;
		IAPAL = 0x0d;
    IAPAH = 0x00;
    set_IAPGO;
		BandgapLow  = IAPFD;
		BandgapMark = BandgapLow&0xF0;
			
		if (BandgapMark==0x80)
		{
				BandgapLow = BandgapLow&0x0F;
				IAPAL = 0x0C;
				IAPAH = 0x00;
				set_IAPGO;
				BandgapHigh = IAPFD;
				Bandgap_Value = (BandgapHigh<<4)+BandgapLow;
				Bandgap_Voltage_Temp = Bandgap_Value*3/4;
		}
		if (BandgapMark==0x00)
		{
				BandgapLow = BandgapLow&0x0F;
				IAPAL = 0x0C;
				IAPAH = 0x00;
				set_IAPGO;
				BandgapHigh = IAPFD;
				Bandgap_Value = (BandgapHigh<<4)+BandgapLow;
				vrfGlb8051ADCBandgap_Voltage= Bandgap_Value*3/4;
		}
		if (BandgapMark==0x90)
		{
				IAPAL = 0x0E;
				IAPAH = 0x00;
				set_IAPGO;
				BandgapHigh = IAPFD;
				IAPAL = 0x0F;
				IAPAH = 0x00;
				set_IAPGO;
				BandgapLow = IAPFD;
				BandgapLow = BandgapLow&0x0F;
				Bandgap_Value = (BandgapHigh<<4)+BandgapLow;
				vrfGlb8051ADCBandgap_Voltage= Bandgap_Value*3/4;
		}
		clr_IAPEN;
	}
void fn8051ADCAdcmain(){
		float bandgapvalue,VDD,VAIN,ADCvalue;
		//fn8051ADCBypass();
		fn8051ADCRead_bandgap();
		ADCCON0 = 0x08;
		ADCCON1 = 0x01;
		clr_ADCF;
		set_ADCS;
		while(ADCF == 0){};
		bandgapvalue = (ADCRH<<4) + ADCRL;
		VDD = (0xFFF/bandgapvalue)*vrfGlb8051ADCBandgap_Voltage;
		Disable_ADC;
		ADCCON0 = 0x04;
		ADCCON1 = 0x01;
		//ADCCON2 = 0x00;
		//AINDIDS = 0xEF;
		clr_ADCF;
		set_ADCS;
		while(ADCF == 0){};
		ADCvalue = (ADCRH<<4) + ADCRL;
		VAIN = (float)ADCvalue/0x1000;
		vrfGlb8051ADCVadcP05 = (float)(VAIN*VDD);
		Disable_ADC;
}
void fn8051APROMWriteADC(){
		float Q;
		unsigned char nguyen,tphan1,tphan2;
		Q = vrfGlb8051ADCVadcP05;
		nguyen = (unsigned char)(Q/1000);
		tphan1 = (unsigned char)((Q - nguyen*1000)/100);
		tphan2 = (unsigned char)((Q - (nguyen*1000+tphan1*100))/10);
		fn8051APROMwrite(nguyen,0x04);
		fn8051APROMwrite(tphan1,0x05);
		fn8051APROMwrite(tphan2,0x06);
}
