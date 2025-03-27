#include <U8g2lib.h>
#define LED 5	// LED 接腳
#define ISR_PIN 33	// 中斷輸入腳

// 初始化 OLED 顯示器
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

uint8_t score = 0;	// 紀錄成績
volatile bool flashLight = false;	// 是否快閃 LED，預設為「否」
SemaphoreHandle_t xSem;// 2 進位旗號參照


// 中斷服務例程 (ISR) 
void IRAM_ATTR ISR() {
  flashLight = true;	// 設置快閃 LED 的標誌
  xSemaphoreGiveFromISR(xSem, NULL);	// 從 ISR 中釋放旗標
}

// OLED 顯示任務
void taskOLED(void * pvParam) {
  u8g2.begin();	// 初始化 OLED 顯示器
  u8g2.setFont(u8g2_font_inb63_mn);	// 設置字體大小
  String txt;	// OLED 顯示文字
  while (1) {  // 若數字不是雙位數，則前補 0 if (score < 10) {
    txt = "0" + String(score);
  } else {
    txt = String(score);
  }
 
  // 更新顯示內容
  u8g2.firstPage(); do {
    u8g2.drawStr(12, 63, txt.c_str());	// 在特定位置顯示文字
  } while ( u8g2.nextPage() );
  vTaskDelay(pdMS_TO_TICKS(100));	// 延遲 100 毫秒
  }
}

// 成績計數任務
void taskScore(void * pvParam) { 
  uint32_t preMs = 0;	// 紀錄觸發時間
  while (1) {
  // 等待中斷發生並取得旗標
    if (xSemaphoreTake(xSem, portMAX_DELAY) == pdPASS) {
    // 跟前次時間比對，若小於 1000 tick 則不執行
      if(xTaskGetTickCountFromISR() - preMs < 1000) continue;
      
  // 增加成績，若達到 100 則歸零
  if (++score == 100) score = 0;
    preMs = xTaskGetTickCountFromISR();	// 紀錄時間
    }
  }
}

// LED 閃爍任務
void taskBlink(void * pvParam) {
  uint8_t counter = 0;	// 閃爍 LED 的計數器
  pinMode(LED, OUTPUT);	// 設置 LED 腳位為輸出

  while (1) {
    digitalWrite(LED, !digitalRead(LED));	// 反轉 LED 狀態

    // 若不需要快閃，則每 500 毫秒閃爍一次
    if (!flashLight) {
 
      vTaskDelay(pdMS_TO_TICKS(500));
    } else {
    // 若需要快閃，每 100 毫秒閃爍一次，持續 10 次
      if(++counter == 10) { 
        counter = 0;
        flashLight = false;
      }
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}

// 初始化設定
void setup() {
  Serial.begin(115200);	// 初始化串列通訊
  pinMode(ISR_PIN, INPUT_PULLUP);	// 設置中斷腳位為上拉模式
  attachInterrupt(ISR_PIN, ISR, RISING);	// 附加中斷處理函式

  xSem = xSemaphoreCreateBinary();	// 建立 2 進位號誌物件
  if (xSem == NULL) {
    Serial.println("無法建立「2 進位號誌」物件～");
  }
  
  // 創建三個任務
  xTaskCreate(taskScore, "score task", 1000, NULL, 2, NULL);
  xTaskCreate(taskBlink, "blink task", 1000, NULL, 1, NULL);
  xTaskCreate(taskOLED,  "OLED task",  1500, NULL, 1, NULL);
}


void loop() { }
