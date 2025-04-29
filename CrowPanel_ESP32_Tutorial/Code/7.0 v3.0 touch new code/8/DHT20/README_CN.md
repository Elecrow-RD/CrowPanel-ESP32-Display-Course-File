#DHT20

DHT20 配有一个全新设计的 ASIC专用芯片、一个经过改进的MEMS半导体电容式湿度传感元件 <br>
和一个标准的片上温度传感元件，其性能已经大大提升甚至超出了前一代传感器的可靠性水平 <br>
新一代温湿度传感器，经过改进使其在恶劣环境下的性能更稳定。每一个传感器都经过严格的 <br>
校准和测试。由于对传感器做了改良和微型化改进，因此它的性价比更高。<br>

<img src="https://dfimg.dfrobot.com/nobody/wiki/73694035631036647cd8872b1d083f6c.png" width="450" hegiht="" align=right/>

## 产品链接([https://www.dfrobot.com.cn/goods-3282.html](https://www.dfrobot.com.cn/goods-3282.html))
    SKU：SEN0497
## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性)
* [历史](#历史)
* [创作者](#创作者)

## 概述


提供一个Arduino库，通过从dht20读取数据来获取湿度和温度。

## 库安装

使用此库前，请首先下载库文件，将其粘贴到\Arduino\libraries目录中，然后打开examples文件夹并在该文件夹中运行演示。

## 方法

```C++

  /*!
   * @brief 构造函数
   * @param IIC对象的指针，可以传参或不传默认为Wire
   * @param 产品的IIC地址，默认为0x38
   */
  DFRobot_DHT20(TwoWire *pWire = &Wire, uint8_t address = 0x38);

  /**
   * @brief 初始化函数
   * @return 初始化成功返回0，否则返回非零的错误码
   */
  int begin(void);
    
  /**
   * @brief 获取环境温度,单位为摄氏度(°C)
   * @return 环境温度,量程为(-40°C ~ 80°C)
   */
  float getTemperature();
    
  /**
   * @brief 获取相对湿度,单位为%RH. 
   * @return 相对湿度, 量程为(1-100%)
   */
  float getHumidity();
```

## 兼容性

主板                | 通过    | 未通过 | 未测试 | 备注 
------------------ | :----------: | :----------: | :---------: | -----
Arduino Uno        |      √       |              |             |
Mega2560        |      √       |              |             |
Leonardo        |      √       |              |             |
ESP32        |      √       |              |             |
ESP8266        |      √       |              |             |
M0        |      √       |              |             |

## 历史

- 日期 2021-6-25
- 版本 V1.0.0

## 创作者

Written by fengli(li.feng@dfrobot.com), 2021.7.31 (Welcome to our [website](https://www.dfrobot.com/))





