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

	// Set pin PC10 to input
	GPIOC->MODER &= ~(0x00000003 << (10 * 2)); // Clear mode register
	GPIOC->MODER |= (0x00000000 << (10 * 2)); // Set mode register (0x00 –
	//Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOC->PUPDR &= ~(0x00000003 << (10 * 2)); // Clear push/pull register
	GPIOC->PUPDR |= (0x00000002 << (10 * 2));

	// Set pin PC12 to input
	GPIOC->MODER &= ~(0x00000003 << (12 * 2)); // Clear mode register
	GPIOC->MODER |= (0x00000000 << (12 * 2)); // Set mode register (0x00 –
	//Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOC->PUPDR &= ~(0x00000003 << (12 * 2)); // Clear push/pull register
	GPIOC->PUPDR |= (0x00000002 << (12 * 2));


}

uint16_t readAnalogue(uint8_t channel){
	if (channel == 1){
		ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);
	} else if (channel == 2){
		ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_1Cycles5);
	} else return 0;
	ADC_StartConversion(ADC1); // Start ADC read
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0){}
	return ADC_GetConversionValue(ADC1); // Read the ADC value
}

uint16_t readJoystickAxis(char axis){
	if (axis == 'y'){
		ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_1Cycles5);
	} else if (axis == 'x'){
		ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_1Cycles5);
	} else return 0;
	ADC_StartConversion(ADC1); // Start ADC read
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0){}
	return ADC_GetConversionValue(ADC1); // Read the ADC value
}

uint8_t readJoystick(){
	uint8_t tmp = 0;
	uint16_t x = readJoystickAxis('x');
	uint16_t y = readJoystickAxis('y');
	if(x>2800) tmp |= (1 << 3); //right
	if(x<1200) tmp |= (1 << 2); //left
	if(y>2800) tmp |= (1 << 0); //up
	if(y<1200) tmp |= (1 << 1); //down
	tmp |= ((GPIOC->IDR & (0x0001 << 10)) >> (-5+10)) //Button1
		  |((GPIOC->IDR & (0x0001 << 12)) >> (-6+12)); //Button2
	return tmp;
}


