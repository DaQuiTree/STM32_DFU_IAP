# STM32_DFU_IAP
基于洋桃1号开发板STM32（F103C8T6）实现的USB Device Firmwire Update

## IAP 注意事项
1、C8T6 Flash page size is 1K

2、bootloader中设置的中断，再跳转APP后不应当放任不管，否则APP中若触发了中断会因为找不到ISR而工作异常。程序中跳转前禁用了EXTI0。

## DFU 注意事项
根据ds手册，F103若想作为USB Slave被host识别，需要给PA12(USB+)一个弱上拉（1.5千欧），洋桃一号开发板需用户自行增加。

## 代码引用
[八度空间 DFU](http://www.openedv.com/forum.php?mod=viewthread&tid=28532&highlight=DFU)

整体引用上述代码，修改点：

1：EXTI0 trigger type raising->falling

2：MAL_Erase()函数针对1K page size做了处理。更好的办法应该是理解DFU机制，再修改代码。
