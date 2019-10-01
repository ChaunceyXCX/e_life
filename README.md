# e_life

基于ESP8266 与机智云做一个网关托管局域网内的设备,将局域网内的所有设备接入互联网

## 源码编译方式

   1. cd app/
   2. ./gen_misc.sh  
  
## Gagent库版本
  
  04020024
  
## 烧录固件
  
  esp_init_data_default.bin          0x3fc000
  blank.bin                          0x3fe000
  boot_v1.6.bin                      0x00000
  user1.4096.new.6.bin               0x01000
  
  选项：CrystalFreq=26M  SPI_SPEED=40MHz SPI_MODE=QIO FLASH_SIZE=32Mbit-C1, 其他默认，串口115200
  进入uart烧录模式后，点击start下载即可！
