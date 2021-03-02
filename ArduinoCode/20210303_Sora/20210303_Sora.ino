int ledPin1 = 4;
int ledPin2 = 5;
int ledPin3 = 6;
int ledPin4 = 7;
int inPin1 = 8;

int flagKey = 0;
int playKey = 0;
int incomingByte = 0;

int checkSensor = 0; //檢查是否站在感應器前



// Game step
const int GameStep_Idle = 0;
const int GameStep_Running = 1;

int gameStep = 0;
bool isHandShake = false;
int unstock = 0;
const int unstockMax = 600;

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

    digitalWrite(ledPin1, HIGH); //A馬達這是關閉
    digitalWrite(ledPin2, HIGH); //A馬達這是關閉
    digitalWrite(ledPin3, HIGH); //B馬達這是關閉
    digitalWrite(ledPin4, HIGH); //B馬達這是關閉

    delay(1000);
    motobcak();

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
            // Start to Recieve Messages , maybe its has been queue in COM port
            if (Serial.available() > 0)
            {
                // read the incoming byte:
                incomingByte = Serial.read();

                // a is actor JUMP
                if (incomingByte == 'a')
                {
                    motogo();

                    // will takes some time ...
                }

                // b is actor FADEOUT
                if (incomingByte == 'b')
                {
                    motobcak_002();

                    // will takes some time ...

                    CallUnitySceneRefresh();
                    gameStep = GameStep_Idle;
                }
            }

            // 防呆滯
            unstock += 1;
            if(unstock % 100 == 0){
                Serial.print("unstock:");
                Serial.println(unstock);
            }
            if(unstock > unstockMax){
                CallUnitySceneRefresh();
                unstock = 0;
                gameStep = GameStep_Idle;
            }

            break;

        default:
            break;
    }

    delay(100);


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

void motobcak() // 馬達復位  初始時使用
{
    Serial.println("B Motor Down");
    digitalWrite(ledPin3, HIGH); //B馬達這是關閉
    digitalWrite(ledPin4, HIGH); //B馬達這是關閉
    Serial.println("A Motor Down");
    digitalWrite(ledPin1, HIGH); //A馬達這是關閉
    digitalWrite(ledPin2, HIGH); //A馬達這是關閉
    delay(30000);
    Serial.println("A Motor Up");
    digitalWrite(ledPin1, LOW); //A馬達這是開
    digitalWrite(ledPin2, LOW); //A馬達這是開
    delay(30000);
    Serial.println("A Motor Down");
    digitalWrite(ledPin1, HIGH); //A馬達這是關閉
    digitalWrite(ledPin2, HIGH); //A馬達這是關閉
    delay(30000);
}

void motobcak_002() // 當人已經衝出時的回位
{
    Serial.println("B Motor Down");
    digitalWrite(ledPin3, HIGH); //B馬達這是關閉
    digitalWrite(ledPin4, HIGH); //B馬達這是關閉
    Serial.println("A Motor Up");
    digitalWrite(ledPin1, LOW); //A馬達這是開
    digitalWrite(ledPin2, LOW); //A馬達這是開
    delay(30000);
    Serial.println("A Motor Down");
    digitalWrite(ledPin1, HIGH); //A馬達這是關閉
    digitalWrite(ledPin2, HIGH); //A馬達這是關閉
    delay(30000);
}

void motogo() // 人衝出
{
    Serial.println("B Motor Up");
    digitalWrite(ledPin3, LOW); //B馬達這是開
    digitalWrite(ledPin4, LOW); //B馬達這是開
    delay(5000);
}