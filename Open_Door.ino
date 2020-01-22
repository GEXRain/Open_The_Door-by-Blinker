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

#define BLINKER_PRINT Serial
#define BLINKER_MQTT
#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT

#include <Blinker.h>

char auth[] = "c443e6424aef";
char ssid[] = "girls";
char pswd[] = "233dzdnz";

// 新建组件对象
BlinkerButton Button1("btn-abc");
BlinkerNumber Number1("num-abc");

int counter = 0;

// 按下按键即会执行该函数
void button1_callback(const String &state)
{
  BLINKER_LOG("get button state: ", state);
  digitalWrite(12, !digitalRead(12));
  delay(2500);
  digitalWrite(12, LOW);
}
void miotPowerState(const String &state)
{
  BLINKER_LOG("need set power state: ", state);
  if (state == BLINKER_CMD_ON)
  {
    digitalWrite(12, HIGH);
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();
      delay(2500);
  digitalWrite(12, LOW);
  }
  else if (state == BLINKER_CMD_OFF)
  {
    digitalWrite(12, LOW);
    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();
  }
}

// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String &data)
{
  BLINKER_LOG("Blinker readString: ", data);
  counter++;
  Number1.print(counter);
}

void setup()
{
  // 初始化串口
  Serial.begin(115200);

#if defined(BLINKER_PRINT)
  BLINKER_DEBUG.stream(BLINKER_PRINT);
#endif

  // 初始化有LED的IO
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  // 初始化blinker
  Blinker.begin(auth, ssid, pswd);
  Blinker.attachData(dataRead);
  Button1.attach(button1_callback);
  //小爱同学务必在回调函数中反馈该控制状态
  BlinkerMIOT.attachPowerState(miotPowerState); //注册回调函数
}

void loop()
{
  Blinker.run();
}
