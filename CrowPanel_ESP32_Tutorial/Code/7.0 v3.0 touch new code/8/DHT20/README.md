#DHT20

The DHT20 temperature & humidity sensor is the newly upgraded version of the DHT11. The DHT20 features a brand-new ASIC dedicated chip, an improved MEMS semiconductor capacitive humidity sensor element and a standard on-chip temperature element. All sensors are factory calibrated and characterized by low power, high precision & stability, fast response, and strong anti-interference. Moreover, the DHT20 performance parameters of accuracy, power supply voltage, measurement range and response time have been enhanced greatly over the previous generation,making it more cost-effective. Besides that, the DHT20 temperature & humidity sensor employs I2C digital output protocol, which is very suitable for HVAC,automobiles, data loggers, weather stations, home appliances, and other related temperature and humidity detection and controlled areas. <br>

<img src="https://dfimg.dfrobot.com/nobody/wiki/73694035631036647cd8872b1d083f6c.png" width="450" hegiht="" align=right/>


## Product Link([https://www.dfrobot.com/product-2391.html](https://www.dfrobot.com/product-2391.html))
    SKU：SEN0497
## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary


Provide an Arduino library to get Humidity and Temperature by reading data from dht20.

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods
```C++

  /*!
   * @brief Construct the function
   * @param pWire IC bus pointer object and construction device, can both pass or not pass parameters, Wire in default.
   * @param address Chip IIC address, 0x38 in default.
   */
  DFRobot_DHT20(TwoWire *pWire = &Wire, uint8_t address = 0x38);

  /**
   * @brief init function
   * @return Return 0 if initialization succeeds, otherwise return non-zero and error code.
   */
  int begin(void);
    
  /**
   * @brief Get ambient temperature, unit: °C
   * @return ambient temperature, the measurement range is (-40°C ~ 80°C)
   */
  float getTemperature();
    
  /**
   * @brief Get relative humidity, unit: %RH. 
   * @return Relative humidity, the measurement range is (1-100%)
   */
  float getHumidity();
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino Uno        |      √       |              |             | 
Mega2560        |      √       |              |             | 
Leonardo        |      √       |              |             | 
ESP32        |      √       |              |             | 
ESP8266        |      √       |              |             | 
M0        |      √       |              |             | 
Micro:bit        |      √       |              |             | 

## History

- Date 2021-6-25
- Version V1.0.0


## Credits

Written by fengli(li.feng@dfrobot.com), 2021.7.31 (Welcome to our [website](https://www.dfrobot.com/))





