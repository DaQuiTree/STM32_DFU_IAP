/**
  ******************************************************************************
  * @�ļ���     �� main.c
  * @����       �� DaQuiTree
  * @�汾       �� V1.0.0
  * @����       �� 2020��11��3��
  * @ժҪ       �� ������ - Demo������̻���StrongerHuang
  ******************************************************************************/

/************************************************
�������� �� main
��    �� �� ���������
��    �� �� ��
�� �� ֵ �� int
��    �� �� �˶ȿռ� ���� ս��STM32
��    ֲ �� DaQuiTree ���� ����1�ſ�����
*************************************************/
#include "sys.h"
#include "led.h"
#include "key.h"

#include "hw_config.h"	//USB���ͷ�ļ�
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_pwr.h"
#include "dfu_mal.h"

typedef  void (*pFunction)(void);

uint8_t DeviceState;
uint8_t DeviceStatus[6];
pFunction Jump_To_Application;
uint32_t JumpAddress;

/**********************************************************
                           ������
**********************************************************/
int main(void)
{
	u8 i=0;

    RCC_Configuration();
	USART1_Init(115200);	//��ʼ������1
	LED_Init();	//��ʼ��LED�ӿ�

	DFU_Button_Config();	//��ʼ����תAPP���򰴼�
    
	//����Ƿ����DFUģʽ����������û�а�������ת��APP������ִ��
	if(DFU_Button_Read() == 1)
	{
		if(((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)	//���APP��ַ�Ƿ�Ϸ�
		{
            //�ر��ж�(USB����ж���ʱδ���)
            DFU_Button_DeConfig();
            
			//��ת��APP��ַ��ʼִ�У���ַ+4λ���Ǹ�λ�ж����
			JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
			Jump_To_Application = (pFunction) JumpAddress;

			//����APP�����ջָ��
			__set_MSP(*(__IO uint32_t*) ApplicationAddress);
			
			//��ת��APP������ִ��
			Jump_To_Application();
		}
        while(1);
	}else{  
        printf("Hello Software Compiled Time: %s, %s.\r\n",__DATE__, __TIME__);	//��ȡ�������ʱ��
        //�����Ѿ����£������APP����ģʽ
        DeviceState = STATE_dfuERROR;
        DeviceStatus[0] = STATUS_ERRFIRMWARE;
        DeviceStatus[4] = DeviceState;

        Set_System();
        Set_USBClock();
        USB_Init();
        
        while(1)
        {
            i++;
            delay_ms(10);
            if(i == 20)
            {
                LEDn1 = ~LEDn1;
                i = 0;
            }
        }
	}
    
} 



