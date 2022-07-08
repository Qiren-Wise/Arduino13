#include <Servo.h>

Servo base,fArm,rArm,claw;


int  basePos = 90; //初始角度为电机角度值
int  fArmPos = 90;
int  rArmPos = 90;
int  clawPos = 90;

int DSD =15; //默认电机延迟时间

bool mode;   //mode=1 指令模式，mode=0 手柄模式
int moveStep = 3; //每按下手柄按键，舵机移动量（仅限于手柄）

int armJoyCmd;
int playDice;

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




   void armDataCmd(char serialCmd)    //指令示例：b45底盘转到45度位置   O输出机械臂舵机状态信息   判断模式是否输错
   {
    if(serialCmd == 'w' || serialCmd == 's'||serialCmd == 'a'||serialCmd == 'd'
    ||serialCmd == '5'||serialCmd == '4'||serialCmd == '6'||serialCmd == '8')
       {
           Serial.println("+Warning: Robot in Instruction Mode..");
           delay(100);
           while (Serial.available()>0) char wrongCommand =Serial.read();  //清除串口缓存错误指令
           return;
        }
    //}

//void  armDataCmd(char serialCmd)  //b45
 // {
   // Serial.print("serialCmd = ");
    //Serial.print(serialCmd);
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
          case 'm':  //切换手柄模式
          mode = 0;
          Serial.println("Command: Switch to Joy-Stick Mode.");
          break;
          
          case 'o':
        reportStatus();
        break;

       case 'i':
        armIniPos();
        break;

        case 'p':
        playDice();
        break;
        
        default:  //未知指令反馈  
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
        
        int action0[3] = {'b',90,DSD};
        int action1[3] = {'r',90,DSD};
        int action2[3] = {'f',90,DSD};
        int action3[3] = {'c',90,DSD};
    

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
    
    svoCmd(action0[0],action0[1],action0[2]);
    servoCmd(action1[0],action1[1],action1[2]);
    servoCmd(action2[0],action2[1],action2[2]);
    servoCmd(action3[0],action3[1],action3[2]);
        
      

 void playDice()
   {
      Serial.println("Command: Let's play some dice!");

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

  void armJoyCmd(char serialCmd)  //手柄按键相应操作
  {

     if(serialCmd == 'b' || serialCmd == 'c'||serialCmd == 'f'||serialCmd == 'r')
       {
           Serial.println("+Warning: Robot in Joy-Stick Mode..");
           delay(100);
           while (Serial.available()>0) char wrongCommand =Serial.read();  //清除串口缓存错误指令
           return;
        }
  
     
     int baseJoyPos;
     int fArmJoyPos;
     int rArmJoyPos;
     int clawJoyPos;
       switch(serialCmd)
       { 
          case 'a':  //Base 向左
          Serial.println("Received Commmand: Base Turn Right");
          baseJoyPos = base.read()-moveStep;
          servoCmd('b',baseJoyPos,DSD);
          break;
          
          case 'd':  //Base 向右
          Serial.println("Received Commmand: Base Turn Left");
          baseJoyPos = base.read()+moveStep;
          servoCmd('b',baseJoyPos,DSD);
          break;

          case 's':  //fArm向下
          Serial.println("Received Commmand: Raer Arm Up");
          rArmJoyPos = rArm.read()+moveStep;
          servoCmd('b',rArmJoyPos,DSD);
          break;

          case 'w':  //fArm向上
          Serial.println("Received Commmand: Raer Arm Down");
          rArmJoyPos = rArm.read()-moveStep;
          servoCmd('b',rArmJoyPos,DSD);
          break;

          case '8':  //fArm向上
          Serial.println("Received Commmand: Front Arm Up");
          rArmJoyPos = rArm.read()+moveStep;
          servoCmd('f',rArmJoyPos,DSD);
          break;

          case '5':  //fArm向下
          Serial.println("Received Commmand: Front Arm Up");
          rArmJoyPos = rArm.read()-moveStep;
          servoCmd('f',rArmJoyPos,DSD);
          break;

          case '4':  //claw 关闭
          Serial.println("Received Commmand: Claw open Up");
          ClawJoyPos = Claw.read()+ moveStep;
          servoCmd('c',ClawJoyPos,DSD);
          break;

           case '6':  //claw 打开
          Serial.println("Received Commmand: Claw open Up");
          ClawJoyPos = Claw.read()- moveStep;
          servoCmd('c',ClawJoyPos,DSD);
          break;


          case 'm':  //切换成指令模式
          mode = 1;
          Serial.println("Commmand: Switch to Instruction MOde.");
          break;

         case '0':  //切换成指令模式
           reportStatus();
          break;

          case 'i':  //切换成指令模式
           armIniPos();
          break;

          default:
        }
   
     
    }

void loop() {
  // put your main code here, to run repeatedly:
if (Serial.available()>0)
  {
    char serialCmd = Serial.read();         //使用串口监视器输入电机指令控制机械臂电机
     if(mode==1){
      armDataCmd(serialCmd);               //b45,底盘舵机转45度  指令模式
    } else 
       {
           armJoyCmd(serialCmd);          //手柄模式
        }

       
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
