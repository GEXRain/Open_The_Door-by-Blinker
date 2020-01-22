/* *****************************************************************
 * 
 * Blinker 库下载地址:
 * https://github.com/blinker-iot/blinker-library/archive/master.zip
 * 
 * Blinker 是一个运行在 IOS 和 Android 上用于控制嵌入式硬件的应用程序。
 * 你可以通过拖放控制组件，轻松地为你的项目建立图形化控制界面。
 * 
 * 文档: https://doc.blinker.app/
 *       https://github.com/blinker-iot/blinker-doc/wiki
 * 
 * *****************************************************************/

#define BLINKER_MQTT
#define BLINKER_PRINT Serial
#define BLINKER_MIOT_SENSOR

#include <Blinker.h>
#include <DHT.h>

char auth[] = "c443e6424aef";
char ssid[] = "girls";
char pswd[] = "233dzdnz";

// 新建组件对象
BlinkerButton Button1("btn-abc");
BlinkerNumber Number1("num-abc");
BlinkerButton Button2("btn-new");
BlinkerNumber HUMI("humi");
BlinkerNumber TEMP("temp");

#define DHTPIN D7
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

float humi_read = 0, temp_read = 0;
int counter = 0;
//心跳包
void heartbeat()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    BLINKER_LOG("Failed to read from DHT sensor!");
  }
  else
  {
    BLINKER_LOG("Humidity: ", h, " %");
    BLINKER_LOG("Temperature: ", t, " *C");
    humi_read = h;
    temp_read = t;
  }
  HUMI.print(humi_read);
  TEMP.print(temp_read);
}
// 按下按键即会执行该函数
void button1_callback(const String &state)
{
  BLINKER_LOG("get button state: ", state);
  digitalWrite(12, !digitalRead(12));
  Blinker.delay(2500);
  digitalWrite(12, LOW);
}
void button2_callback(const String &state)
{

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    BLINKER_LOG("Failed to read from DHT sensor!");
  }
  else
  {
    BLINKER_LOG("Humidity: ", h, " %");
    BLINKER_LOG("Temperature: ", t, " *C");
    humi_read = h;
    temp_read = t;
  }
  HUMI.print(humi_read);
  TEMP.print(temp_read);
}
void miotPowerState(const String &state)
{
  BLINKER_LOG("need set power state: ", state);
  if (state == BLINKER_CMD_ON)
  {
    digitalWrite(12, HIGH);
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();
    Blinker.delay(2500);
  digitalWrite(12, LOW);
  }
  else if (state == BLINKER_CMD_OFF)
  {
    digitalWrite(12, LOW);
    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();
  }
}
void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case 0 :
            BlinkerMIOT.temp(dht.readTemperature());
            BlinkerMIOT.humi((int)dht.readHumidity());
            BlinkerMIOT.print();
            Number1.print(queryCode);
            break;
    }
}
void setup()
{
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);
  BLINKER_DEBUG.debugAll();
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);

  Blinker.begin(auth, ssid, pswd);
  Button1.attach(button1_callback);
  Button2.attach(button2_callback);
  Blinker.attachHeartbeat(heartbeat);
  dht.begin();
  //小爱同学务必在回调函数中反馈该控制状态
  BlinkerMIOT.attachPowerState(miotPowerState); //注册回调函数
  BlinkerMIOT.attachQuery(miotQuery);
}

void loop()
{
  Blinker.run();
}
