//***************************************************************************************
//�һ�΢�����������Դ���Ȩ���һ��Ŷ����У�δ���һ��Ŷ�ͬ�⣬�������������ϴ�����Դ�롣
//�뱾��������鼮<<���������DIY-����STM32΢������>>���ɱ�����������ʽ���棬���ݶԱ��װ�
//�������������Լ�Ӳ����ض�������ϸ�Ľ��⣬����Ȥ�����ѿ��ԴӸ�����깺��
//�뱾�������׵�Ӳ����http://fire-dragon.taobao.com
//������������˸���������ĸĽ�������ʱ�����Ǳ�����ϵ��
//QQ��16053729    �һ�QQȺ��234879071
//***************************************************************************************
#include "struct_all.h"

uint16_t Battery_Fly,Battery_Rc;//�����ѹ��ң�ص�ѹ��100��
uint16_t ADC_Value[M]; 

/******************************************************************************
����ԭ�ͣ�	static void ADC1_GPIO_Config(void)
��    �ܣ�	ADC��IO��ʼ��
*******************************************************************************/ 
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ��GPIO��ADC��DMA������ʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	
	/* ��GPIO����Ϊ����ģʽ */	
  	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_Init(GPIOA , &GPIO_InitStructure);
}

/******************************************************************************
����ԭ�ͣ�	static void ADC1_Mode_Config(void)
��    �ܣ�	ADC��DMA����
*******************************************************************************/ 
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	/* DMA ͨ��1 ���� */
	DMA_DeInit(DMA1_Channel1);//��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//DMA����ADC����ַ;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Value;//DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//�ڴ���Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = M;//DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ���Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���ݿ���Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//������ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMAͨ��xӵ�и����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��

	/* ʹ�� DMA ͨ��1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
//////////////////////////////////////////////////////////////////////////////////////////////
	ADC_DeInit(ADC1); //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	ADC_TempSensorVrefintCmd(ENABLE);//ʹ���ڲ����յ�ѹ��1.2V��
	/* ADC1 ���� */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ ÿ��ADC��������
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ʹ��ɨ��ģʽ  scanλ����
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//contλ���� ����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//EXTSEL ѡ����������ͨ����ת�����ⲿ�¼� ���ó�����������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//���ݶ��� ��������λ�����   �������ó��Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = M;//����ͨ�����г��� ��Щλ�����������ڹ���ͨ��ת�������е�ͨ����Ŀ 1��ת�� ָ���ɶ��ٸ�ͨ����ת��
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ���� ADC1 ʹ��8ת��ͨ�����ڹ��������ת��˳��--->1����ͨ����ʱ��Ϊ 71.5 ���� */ 	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_71Cycles5 );//AUX2 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_71Cycles5 );//AUX3 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_71Cycles5 );//AUX1 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5);//AUX4 (��ص�ѹ)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_71Cycles5 );//THROTTLE
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_71Cycles5 );//YAW
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_71Cycles5 );//PITCH
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_71Cycles5 );//ROLL
	ADC_RegularChannelConfig(ADC1, ADC_Channel_17,9, ADC_SampleTime_239Cycles5);//�ڲ����յ�ѹ��1.2V��	

	/* ʹ�� ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	/* ʹ�� ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/* ��λ ADC1 ��У׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/* �ȴ� ADC1 У׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* ��ʼ ADC1 У׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ� ADC1 У׼��� */
	while(ADC_GetCalibrationStatus(ADC1));
	/* ʹ��ָ����ADC1������ת���������� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/******************************************************************************
����ԭ�ͣ�	void ADC1_Init(void)
��    �ܣ�	ADC1��ʼ��
*******************************************************************************/ 
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}

/******************************************************************************
����ԭ�ͣ�	void Voltage_Printf(void)
��    �ܣ�	���AD��ͨ����ѹֵһ�ٱ�
*******************************************************************************/ 
void Voltage_Printf(void)
{
	uint8_t i;
	uint16_t Voltage;
	
	PrintString("\r\n");
	for(i=0;i<8;i++)
	{
		Voltage = (uint16_t)(2.0f*ADC_Value[i]/ADC_Value[8]*1.2f*100);
		PrintU8(i);
		PrintString("ͨ���� ");
		PrintU16(Voltage); 
		PrintString("\r\n");
	}
}