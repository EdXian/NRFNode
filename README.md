# NRFNode簡介

## 支援硬體
 * NRFNode - (PCA-10100/S140) 
 * Thingy52 (僅支援部分的Service)

## 目錄結構

- Hardware - 包含PCB電路GERBER圖檔、外殼圖檔及加工說明。
- Software -  包含應用程序，APP以及桌面開發程式。
- Firmware - 包含 nrf52的驅動程式以及測試程式
- Tools  - 生產工具



## BLE模組

MDBT50Q  nRF52833

## 韌體架構圖

韌體主要由5~6層，大致可以分為以下幾層:

- Application Layer - 開發者可以自行開發應用功能，例如防跌倒偵測。
- Service Layrer - 事先提供的特定服務層，開發者可以透過這層的API快速寫出產品原型
- Nordic SDK Layer - 廠商所提供的HAL，用以開發service Layer以及application Layer
- OS Layer - 即時嵌入式系統FreeRTOS
- CPU Achitecture- nrf52833 為 Cortex M4 內核，可以通用M4支援的CMSIS API。

![](images/Architecture.png)

硬體外觀

![](images/nrfnode.png)



輸出入介面:

目前輸出介面提供幾種通訓方式

1. BLE 4.2/5.1
2. USB CDC
3. UART
4. SPI
5. I2C
6. NFC
7. 3 Buttons
8. WS2812B 



## 電源:

可外接3.7v Li-Po電源或5vUSB電源供電

## 加工

打板- 使用中國廠商JLCPCB的打板服務

被動元件貼片- 可使用JLCPCB的元件直接在廠內打件完畢

模組焊接-由於MDBT50Q模組不能用一般焊槍焊接，建議使用烤箱、回流焊爐加工，或是使用加熱平台進行加工

STEP1:

使用錫膏180~200度塗在模組表面上，並使用焊槍(210度)將銲點黏在模組PAD表層上，期間可使用助銲劑幫助焊接，焊接完成後使用放大鏡檢查

STEP2:

使用清潔劑清洗，可用75%酒精，確定PCB表面以及模組銲點表面都是乾淨(沒有黑色的油漬)

STEP3: 將PCB表層上助銲劑並將模組放上，並放上150度的加熱平台預先加熱約30~40秒左右接著提升溫度到210 度(溫度拉升時間約20秒)，接著等待錫膏溶解並與銲點正確連接。

STEP4:當錫膏溶解後會使模組滑動，這時候可以使用小鑷子輕輕推動模組觀察模組是否移動後會自動歸位，並上下輕點模組，將裡面多餘的銲錫推出來，反覆2~3次即可，接著靜止等待約10秒

STEP5: 開始降溫，並確保零件沒有被移動到，最好降溫到表面溫度到100度以下再拿起來，此時銲點已經凝固，可以使用電風散散熱

3d 列印機

建議在列印平台工作區至少又10cm*10cm



## 版本更新:

V1

V2
