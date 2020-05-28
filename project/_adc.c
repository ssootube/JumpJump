
#include "_main.h"
#include "_adc.h"

#define ADC_VREF_TYPE 0x00      
#define ADC_AVCC_TYPE 0x40    
#define ADC_RES_TYPE  0x80
#define ADC_2_56_TYPE 0xC0   

//ADC initialize
void Adc_init(void)
{
	ADCSRA = 0x00;
	ADMUX  = 0x00; 	
	ACSR   = 0x80;
	ADCSRA = 0xC3;//변환시간: 6uS
}
unsigned int Read_Adc_Data(unsigned char adc_input)//ADC 읽기
{	unsigned int adc = 0;
	ADCSRA = 0xC3;//변환시간: 6uS

	ADMUX = adc_input | ADC_AVCC_TYPE ;
	ADCSRA |= 0x40;                          
	while((ADCSRA & 0x10) != 0x10);
	adc += ADCL + ( ADCH * 256 );

	ADCSRA = 0x00;
	return adc;
}
