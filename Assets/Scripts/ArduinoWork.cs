using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ArduinoWork : MonoBehaviour
{
    public string Msg_Recv_Ready;
    public string Msg_Recv_ToRun;
    public string Msg_Send_ToJump;
    public string Msg_Send_ToBack;
    public ArduinoInteractive arduinoInteractive;

    public Action OnStageReady;
    public Action OnAnimStartRun;
    
    void Start()
    {
        arduinoInteractive.OnRecieveData += RecievedSignals;
    }

    
    // 爆破音效
    void SendJumpMsg(){
        arduinoInteractive.SendData(Msg_Send_ToJump);
    }

    // 黑幕降臨, 提示裝置該歸位了
    void SendBack(){
        arduinoInteractive.SendData(Msg_Send_ToBack);
    }

    void RecievedSignals(string msg){
        // 開始你的表演
        if(msg == Msg_Recv_ToRun){
            OnAnimStartRun?.Invoke();
        }

        // 舞台準備就緒, 黑幕可以拉開了
        if(msg == Msg_Recv_Ready){
            OnStageReady?.Invoke();
        }
    }
}
