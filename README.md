# 가전제어 - LED

## 기능

- LED를 Tact Switch로 제어한다. Tact Switch가 눌리는 것만을 작동으로 인지하고, On -> Off / Off -> On으로 바꾸어준다.
- Tact Switch로 제어한 결과를(on - 1, off - 0) 각각 led1, led2, led3 라는 MQTT의 topic으로 Publish한다.
- MQTT의 subLed1, subLed2, subLed3 라는 topic을 Subscribe하여 LED를 각각 원격 제어할 수 있다.
