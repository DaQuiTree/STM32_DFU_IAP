/**
  ******************************************************************************
  * @文件名     ： main.c
  * @作者       ： DaQuiTree
  * @版本       ： V1.0.0
  * @日期       ： 2020年11月3日
  * @摘要       ： 主函数 - Demo软件工程基于StrongerHuang
  ******************************************************************************/

/************************************************
函数名称 ： main
功    能 ： 主函数入口
参    数 ： 无
返 回 值 ： int
作    者 ： 八度空间 基于 战舰STM32
移    植 ： DaQuiTree 基于 洋桃1号开发板
*************************************************/
#include "sys.h"
#include "led.h"
#include "key.h"

#include "hw_config.h"	//USB相关头文件
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
                           主函数
**********************************************************/
int main(void)
{
	u8 i=0;

    RCC_Configuration();
	USART1_Init(115200);	//初始化串口1
	LED_Init();	//初始化LED接口

	DFU_Button_Config();	//初始化跳转APP程序按键
    
	//检测是否进入DFU模式按键，开机没有按下则跳转到APP程序中执行
	if(DFU_Button_Read() == 1)
	{
		if(((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)	//检测APP地址是否合法
		{
            //关闭中断(USB相关中断暂时未理会)
            DFU_Button_DeConfig();
            
			//跳转到APP地址开始执行，地址+4位置是复位中断入口
			JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
			Jump_To_Application = (pFunction) JumpAddress;

			//设置APP程序堆栈指针
			__set_MSP(*(__IO uint32_t*) ApplicationAddress);
			
			//跳转到APP程序中执行
			Jump_To_Application();
		}
        while(1);
	}else{  
        printf("Hello Software Compiled Time: %s, %s.\r\n",__DATE__, __TIME__);	//获取软件编译时间
        //按键已经按下，则进入APP升级模式
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



