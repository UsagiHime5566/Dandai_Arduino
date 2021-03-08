int ledPin1 = 4;
int ledPin2 = 5;
int ledPin3 = 6;
int ledPin4 = 7;
int inPin1 = 8;

int ledPow1 = 9;
int ledPow2 = 10;

int flagKey = 0;
int playKey = 0;
int incomingByte = 0;

int checkSensor = 0; //檢查是否站在感應器前



// Game step
const int GameStep_Idle = 0;
const int GameStep_Running = 1;
const int GameStep_Fallen = 2;
const int GameStep_Resume_1 = 3;
const int GameStep_Resume_2 = 4;

int gameStep = 0;
bool isHandShake = false;
int unstock = 0;
const int unstockMax = 700;


// Delay logic
const unsigned long fallenTime = 3000;
const unsigned long resume1_time = 30000;
const unsigned long resume2_time = 30000;

unsigned long nextDelayAt = 0;

bool DetectedUser(){
    int val = digitalRead(inPin1);
    return val != 0;
}
void CallUnityActorRun(){
    Serial.println("y");
}
void CallUnitySceneRefresh(){
    Serial.println("n");
}

void setup()
{
    Serial.begin(9600);

    pinMode(ledPin1, OUTPUT); // sets the digital pin as output
    pinMode(ledPin2, OUTPUT);
    pinMode(ledPin3, OUTPUT);
    pinMode(ledPin4, OUTPUT);
    pinMode(inPin1, INPUT);

    pinMode(ledPow1, OUTPUT);
    pinMode(ledPow2, OUTPUT);

    digitalWrite(ledPin1, HIGH); //A馬達這是關閉
    digitalWrite(ledPin2, HIGH); //A馬達這是關閉
    digitalWrite(ledPin3, HIGH); //B馬達這是關閉
    digitalWrite(ledPin4, HIGH); //B馬達這是關閉


    
    digitalWrite(ledPow1, LOW); //A馬達斷電
    digitalWrite(ledPow2, LOW); //B馬達斷電

    delay(1000);
    
    motobcak();
    powerOff();
    

    //Wait for handshake
    while (!isHandShake)
    {
        Serial.println("need assign 'c'");

        if (Serial.available() > 0)
        {
            incomingByte = Serial.read();

            //sora.add 'c' , unity is ready
            if (incomingByte == 'c')
            {
                isHandShake = true;
            }
        }

        delay(1000);
    }

    gameStep = GameStep_Idle;
    CallUnitySceneRefresh();
}

void loop()
{
    switch(gameStep)
    {
        case GameStep_Idle:
            // check for sensor status
            if(DetectedUser()) {
                //檢查是否站在感應器前3秒
                delay(3000); //是否站在前面的秒數

                if(DetectedUser()){
                    CallUnityActorRun();
                    unstock = 0;
                    gameStep = GameStep_Running;
                } else {
                    Serial.println("not stay");
                }
            }

            break;
        
        case GameStep_Running:

            if(GetMessage('a')){
                motogo();
                //nextDelayAt = millis();
                delay(5000);
                powerOff();////////////關馬達電
                unstock = 0;
                gameStep = GameStep_Fallen;
            }

            break;

        case GameStep_Fallen:
            if(!WaitTime(nextDelayAt, fallenTime))
                break;

            if(GetMessage('b')){
                motoback_step1();
                //nextDelayAt = millis();
                delay(30000);
                powerOff();////////////關馬達電
                unstock = 0;
                gameStep = GameStep_Resume_1;
            }
            
            break;

        case GameStep_Resume_1:
            if(!WaitTime(nextDelayAt, resume1_time))
                break;

            motoback_step2();
            //nextDelayAt = millis();
            delay(30000);
            powerOff();////////////關馬達電
            unstock = 0;
            gameStep = GameStep_Resume_2;
            
            break;

        case GameStep_Resume_2:
            if(!WaitTime(nextDelayAt, resume2_time))
                break;

            CallUnitySceneRefresh();
            unstock = 0;
            gameStep = GameStep_Idle;
            break;

        default:
            break;
    }

    delay(100);

    if(gameStep != GameStep_Idle)
    {
        // 防呆滯
        unstock += 1;
        if(unstock % 200 == 0){
            Serial.print("unstock:");
            Serial.println(unstock);
        }
        if(unstock > unstockMax){
            unstock = 0;
            ClearAllInput();
            motoback_step1();

            //nextDelayAt = millis();
            delay(30000);
            powerOff();////////////關馬達電

            
            gameStep = GameStep_Resume_1;
        }
    }

    // if (playKey == 0)
    // {
    //     val = digitalRead(inPin1);
    // }

    // if (val == 0 && flagKey == 0 && playKey == 0)
    // {
    //     Serial.println("n");
    //     //Serial.write("n");
    //     flagKey = 1;
    // }

    // if (val == 1 && flagKey == 1 && playKey == 0)
    // {

    //     //檢查是否站在感應器前3秒
    //     ////////////////////////////////
    //     delay(3000); //是否站在前面的秒數

    //     checkSensor = digitalRead(inPin1);

    //     if (checkSensor == 0)
    //     {
    //         flagKey = 0;
    //     }
    //     if (checkSensor == 1)
    //     {
    //         Serial.println("y");
    //         //Serial.write("y");
    //         delay(100);
    //         playKey = 1;
    //     }

    //     ///////////////////////////////
    // }

    // if (playKey == 1)
    // {
    //     if (Serial.available() > 0)
    //     {
    //         // read the incoming byte:
    //         incomingByte = Serial.read();

    //         if (incomingByte == 97)
    //         {
    //             motogo();
    //         }

    //         if (incomingByte == 98)
    //         {
    //             motobcak_002();
    //             flagKey = 0;
    //             playKey = 0;
    //         }
    //     }
    // }

    // delay(100);
}

void powerOff()
{
  
  
   digitalWrite(ledPow1, LOW); //A馬達斷電
   digitalWrite(ledPow2, LOW); //B馬達斷電

  
  
  
  }

void motobcak() // 馬達復位  初始時使用
{
    Serial.println("B Motor Down");
    digitalWrite(ledPow2, LOW); //B馬達斷電
    digitalWrite(ledPin3, HIGH); //B馬達這是關閉
    digitalWrite(ledPin4, HIGH); //B馬達這是關閉
    digitalWrite(ledPow2, HIGH); //B馬達送電
    Serial.println("A Motor Down");
    digitalWrite(ledPow1, LOW); //A馬達斷電
    digitalWrite(ledPin1, HIGH); //A馬達這是關閉
    digitalWrite(ledPin2, HIGH); //A馬達這是關閉
    digitalWrite(ledPow1, HIGH); //A馬達送電
    delay(30000);
    Serial.println("A Motor Up");
    digitalWrite(ledPow1, LOW); //A馬達斷電
    digitalWrite(ledPin1, LOW); //A馬達這是開
    digitalWrite(ledPin2, LOW); //A馬達這是開
    digitalWrite(ledPow1, HIGH); //A馬達送電
    delay(30000);
    Serial.println("A Motor Down");
    digitalWrite(ledPow1, LOW); //A馬達斷電
    digitalWrite(ledPin1, HIGH); //A馬達這是關閉
    digitalWrite(ledPin2, HIGH); //A馬達這是關閉
    digitalWrite(ledPow1, HIGH); //A馬達送電
    delay(30000);
}



void motoback_step1(){
    Serial.println("B Motor Down");
    digitalWrite(ledPow2, LOW); //B馬達斷電
    digitalWrite(ledPin3, HIGH); //B馬達這是關閉
    digitalWrite(ledPin4, HIGH); //B馬達這是關閉
    digitalWrite(ledPow2, HIGH); //B馬達送電
    Serial.println("A Motor Up");
    digitalWrite(ledPow1, LOW); //A馬達斷電
    digitalWrite(ledPin1, LOW); //A馬達這是開
    digitalWrite(ledPin2, LOW); //A馬達這是開
    digitalWrite(ledPow1, HIGH); //A馬達送電
}

void motoback_step2() // 當人已經衝出時的回位
{
    Serial.println("A Motor Down");
    digitalWrite(ledPow1, LOW); //A馬達斷電
    digitalWrite(ledPin1, HIGH); //A馬達這是關閉
    digitalWrite(ledPin2, HIGH); //A馬達這是關閉
    digitalWrite(ledPow1, HIGH); //A馬達送電
}

void motogo() // 人衝出
{
    Serial.println("B Motor Up");
    digitalWrite(ledPow2, LOW); //B馬達斷電
    digitalWrite(ledPin3, LOW); //B馬達這是開
    digitalWrite(ledPin4, LOW); //B馬達這是開
    digitalWrite(ledPow2, HIGH); //B馬達送電
}

bool GetMessage(char c){
    if (Serial.available() > 0)
    {
        unsigned int recv = Serial.read();
        
        if(recv == c){
            return true;
        }
    }

    return false;
}

bool WaitTime(unsigned long w_base, unsigned long w_val){
    if(millis() > w_base + w_val){
        return true;
    }
    return false;
}

void ClearAllInput(){
    while(Serial.available() > 0){
        unsigned int r = Serial.read();
    }
}
