#include "analogue.h"

void initAnalogue(){
	RCC->CFGR2 &= ~RCC_CFGR2_ADCPRE12; // Clear ADC12 prescaler bits
	RCC->CFGR2 |= RCC_CFGR2_ADCPRE12_DIV6; // Set ADC12 prescaler to 6
	RCC->AHBENR |= RCC_AHBPeriph_ADC12; // Enable clock for ADC12

	ADC1->CR = 0x00000000; // Clear CR register
	ADC1->CFGR &= 0xFDFFC007; // Clear ADC1 config register
	ADC1->SQR1 &= ~ADC_SQR1_L; // Clear regular sequence register 1

	ADC1->CR |= 0x10000000; // Enable internal ADC voltage regulator
	for (int i = 0 ; i < 1000 ; i++) {} // Wait for about 16 microseconds

	ADC1->CR |= 0x80000000; // Start ADC1 calibration
	while (!(ADC1->CR & 0x80000000)); // Wait for calibration to finish
	for (int i = 0 ; i < 100 ; i++) {} // Wait for a little while

	ADC1->CR |= 0x00000001; // Enable ADC1 (0x01 - Enable, 0x02 - Disable)
	while (!(ADC1->ISR & 0x00000001)); // Wait until ready
}

uint16_t readAnalogue(uint8_t channel){
	uint32_t x = 0;
	if (channel == 1){
		ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);
	} else if (channel == 2){
		ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_1Cycles5);
	} else return 0;
	ADC_StartConversion(ADC1); // Start ADC read
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0){}
	return ADC_GetConversionValue(ADC1); // Read the ADC value
}
