#include <Servo.h>

Servo base,fArm,rArm,claw;


int  basePos = 90; //初始角度为电机角度值
int  fArmPos = 90;
int  rArmPos = 90;
int  clawPos = 90;

int DSD =15; //默认电机延迟时间

const int baseMin = 0; //存储电机极限值，不变，只读状态
const int baseMax = 180; //角度可调

const int fArmMin = 35;
const int fArmMax = 120;

const int rArmMin = 45;
const int rArmMax = 180;

const int clawMin = 25;
const int clawMax = 100;

void setup() {
  // put your setup code here, to run once:
base.attach(11);
delay(200);
fArm.attach(10);
delay(200);
rArm.attach(9);
delay(200);
claw.attach(6);
delay(200);
Serial.begin(9600);
Serial.println("Welcome to Taichi-Maker Robot Arm Tutorial");
}

void loop() {
  // put your main code here, to run repeatedly:
if (Serial.available()>0)
  {
    char serialCmd = Serial.read();         //使用串口监视器输入电机指令控制机械臂电机
      armDataCmd(serialCmd);               //b45,底盘舵机转45度
    }
    base.write(basePos);
    delay(10);
     fArm.write(fArmPos);
    delay(10);
     rArm.write(rArmPos);
    delay(10);
     claw.write(clawPos);
    delay(10);
}

void  armDataCmd(char serialCmd)  //b45
  {
    Serial.print("serialCmd = ");
    Serial.print(serialCmd);
  //serialCmd:b
  //servoData:45

  if (serialCmd=='b' ||  serialCmd =='c' ||  serialCmd == 'f' ||  serialCmd == 'r')
   {
      int servoData = Serial.parseInt();
      servoCmd(serialCmd,servoData,DSD);
    } else
     {
      switch(serialCmd)
        {
          case 'o':
        reportStatus();
        break;

       case 'i':
        armIniPos();
        break;

        case 'p':
        playDice();
        break;
        
        default:
        Serial.print(" Unknown Command. ");
          }
      }
  
    }


void servoCmd(char servoName,int toPos,int servoDelay)
  {
      Servo servo2go;   //创建Servo对象

      
      //串口监视器输出接收指令
      Serial.println("");
      Serial.print("+Command : Servo ");
      Serial.print(servoName);
      Serial.print(" to ");
      Serial.print(toPos);
      Serial.print(" at servoDelay Value");
      Serial.print(servoDelay);
      Serial.println(".");
      Serial.println("");

      int fromPos;  //建立变量，存储电机起始角度值

      switch  (servoName)
       {
        case 'b':
           if (toPos>=baseMin && toPos <= baseMax)
           {
              servo2go  = base;
              fromPos = base.read();
              break;
              
            }  else
               {
                Serial.println("+Warning: Base Servo Value Out Of Limit! ");
                  return ;
                }

        case 'c':
           if (toPos>=clawMin && toPos <= clawMax)
           {
              servo2go  = claw;
              fromPos = claw.read();
              break;
              
            }  else
               {
                Serial.println("+Warning: claw Servo Value Out Of Limit! ");
                  return ;
                }

             case 'f':
           if (toPos>=fArmMin && toPos <= fArmMax)
           {
              servo2go  = fArm;
              fromPos = fArm.read();
              break;
              
            }  else
               {
                Serial.println("+Warning: fArm Servo Value Out Of Limit! ");
                  return ;
                }

            case 'r':
           if (toPos>=rArmMin && toPos <= rArmMax)
           {
              servo2go  = rArm;
              fromPos = rArm.read();
              break;
              
            }  else
               {
                Serial.println("+Warning: rArm Servo Value Out Of Limit! ");
                  return ;
                }
                
            }

      if (fromPos <= toPos)
        {
          for (int i= fromPos; i<=fromPos;i++)
            {
              servo2go.write(i);
              delay (servoDelay);
              }
        }  else 
             {
              for (int i=fromPos;i>=toPos;i--)
                 {
                  servo2go.write(i);
                  delay(servoDelay);
                  }
              }
            
    }

    void reportStatus() //舵机状态信息
    {
      Serial.println("");
      Serial.println("");
      Serial.println("+Robot-Arm Status Report +");
      Serial.print("Claw Posoition: ");Serial.println(claw.read());
      Serial.print("Base Posoition: ");Serial.println(base.read());
      Serial.print("fArm Posoition: ");Serial.println(fArm.read());
      Serial.print("rArm Posoition: ");Serial.println(rArm.read());
      Serial.println("++++++++++++++++++");
      Serial.println("");
    
      }

   void armIniPos()
     {
        Serial.println("+Command: Restore Initial Position");
        /*
        int action0[3] = {'b',90,DSD};
        int action1[3] = {'r',90,DSD};
        int action2[3] = {'f',90,DSD};
        int action3[3] = {'c',90,DSD};
    */

     int robotIniPosArray[4][3] = 
       {
          {'b', 90,DSD},
          {'r', 90,DSD},
          {'f', 90,DSD},
          {'c', 90,DSD},
        };

        for (int i = 0;i<4;i++)
          {
            servoCmd(robotIniPosArray[i][0],robotIniPosArray[i][1],robotIniPosArray[i][2]);
            }
    /*
    servoCmd(action0[0],action0[1],action0[2]);
    servoCmd(action1[0],action1[1],action1[2]);
    servoCmd(action2[0],action2[1],action2[2]);
    servoCmd(action3[0],action3[1],action3[2]);
*/
        
      }

 void playDice()
   {
      Serial.println(Command: Let's play some dice!);

      armIniPos();

      int diceMove1[21][3] = 
        {
           {'c', 30,DSD},
           {'b', 90,DSD},
           {'f', 43,DSD},
           {'r', 126,DSD},
           {'c', 40,DSD},
           {'b', 80,DSD},
           {'f', 43,DSD},
           {'r', 156,DSD},
           {'c', 50,DSD},
           {'b', 100,DSD},
           {'f', 143,DSD},
           {'r', 26,DSD},
          };
      for (int i= 0;i<21;i++)
         {
            servoCmd(diceMove1[i][0],diceMove1[i][1],diceMove1[i][2]);
            delay(200);
          }
          armIniPos();

          int diceMove2[21][3] = 
        {
           {'c', 130,DSD},
           {'b', 90,DSD},
           {'f', 143,DSD},
           {'r', 126,DSD},
           {'c', 140,DSD},
           {'b', 80,DSD},
           {'f', 143,DSD},
           {'r', 156,DSD},
           {'c', 150,DSD},
           {'b', 100,DSD},
           {'f', 143,DSD},
           {'r', 126,DSD},
          };
      for (int i= 0;i<21;i++)
         {
            servoCmd(diceMove2[i][0],diceMove2[i][1],diceMove2[i][2]);
            delay(200);
          }
          armIniPos();
    }



      
