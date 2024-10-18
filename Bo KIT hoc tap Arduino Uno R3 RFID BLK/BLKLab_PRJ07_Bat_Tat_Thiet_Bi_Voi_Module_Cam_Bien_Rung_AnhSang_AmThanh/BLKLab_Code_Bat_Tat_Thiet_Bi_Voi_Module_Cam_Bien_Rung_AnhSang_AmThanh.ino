/*                                                                                 *
 * ================================================================================*
 *                            BANLINHKIEN.COM                                      * 
 * ================================================================================*                           
 *                                                                                 *
 *          Hướng Dẫn Bật /Tắt thiết bị bằng module cảm biến rung,                 *
 *                  cảm biến ánh sáng, cảm biến âm thanh                           *
 * Đấu nối:                                                                        *
 *-D2: nối chân DO trên cảm biến rung                                              *
 *-D4: nối chân DO trên cảm biến ánh sáng                                          *
 *-D3: nối chân OUT trên cảm biến âm thanh                                         *    
 *-D12: nối chân IN trên Relay                                                     *
 *=================================================================================*/
#define RUNG 2
#define ANHSANG 3
#define AMTHANH 4

uint8_t currentSst3=1,lastSst3=1;
uint8_t currentSst2=1,lastSst2=1;
uint8_t currentSst1=1,lastSst1=1;
int sensorValueCBRung = 0; 
int sensorValueCBAS = 0;
int sensorValueCBAT = 0;
uint8_t state = 0;

void setup() {
  
  pinMode(12, OUTPUT);
  pinMode(RUNG, INPUT);
  pinMode(ANHSANG, INPUT);
  pinMode(AMTHANH, INPUT);
  Serial.begin(9600);
}

void loop() {
  handle_CB1();
  handle_CB2();
  handle_CB3();
}


int countR = 0;
uint8_t stateRung = 0;
void handle_CB1()
{
  static uint32_t t_debound_buton;
  if(millis() - t_debound_buton >= 10)
  {
    t_debound_buton= millis();
    sensorValueCBRung = digitalRead(3);
    switch (stateRung)
    {
      case 0:
        
        if(sensorValueCBRung == 0) 
        {
          stateRung =1;
          currentSst1 = 1;
        }
        break;
      case 1:
        if(sensorValueCBRung == 1)
          countR ++ ;
        if(sensorValueCBRung == 0) countR = 0; 
        if(countR > 25)
        {
          countR = 0 ;
          stateRung = 0;
          currentSst1 = 0;
        }
        break;
 
    }
    if(currentSst1==0)
    {
        if(lastSst1==1)
        {
          lastSst1=currentSst1;
          state = 1- state;
          
          if(state == 0)
            digitalWrite(12,1);
          else
            digitalWrite(12,0);
           delay(20);
        }
        
    }
    ////Nha Tay
    else
    {
        if(lastSst1==0)
        {
          lastSst1=currentSst1;
          
          
        }     
    }
  }
}


void handle_CB2()
{

    sensorValueCBAS = digitalRead(2);
    if(sensorValueCBAS == 1) digitalWrite(12,1);
    else  digitalWrite(12,0);
    
}


int countAT = 0;
uint8_t stateAT = 0;
void handle_CB3()
{
  static uint32_t t_debound_buton;
  if(millis() - t_debound_buton >= 10)
  {
    t_debound_buton= millis();
    sensorValueCBAT = digitalRead(4);
    switch (stateAT)
    {
      case 0:
        if(sensorValueCBAT == 0) 
        {
          stateAT =1;
          currentSst3 = 1;
        }
        break;
      case 1:
        if(sensorValueCBAT == 1)
          countAT ++ ;
        if(sensorValueCBAT == 0) countAT = 0; 
        if(countAT > 15)
        {
          countAT = 0 ;
          stateAT = 0;
          currentSst3 = 0;
        }
        break;
 
    }
    if(currentSst3==0)
    {
        if(lastSst3==1)
        {
          lastSst3=currentSst3;
        }
    }
    ////Nha Tay
    else
    {
        if(lastSst3==0)
        {
          lastSst3=currentSst3;
          state = 1- state;
          if(state == 0)
            digitalWrite(12,1);
          else
            digitalWrite(12,0);
          delay(20);
        }     
    }
  }
}
