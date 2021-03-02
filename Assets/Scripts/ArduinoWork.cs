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
    public string Msg_Send_HandShake;
    public string Msg_Recv_HandShake;
    public ArduinoInteractive arduinoInteractive;
    public AnimWorker animWorker;

    public Action OnStageReady;
    public Action OnAnimStartRun;
    
    void Start()
    {
        arduinoInteractive.OnRecieveData += RecievedSignals;
        animWorker.OnAnimJump += SendJumpMsg;
        animWorker.OnStageBlack += SendBack;
    }

    
    // 爆破音效
    void SendJumpMsg(){
        Debug.Log($"Send message to arduino: {Msg_Send_ToJump}");
        arduinoInteractive.SendData(Msg_Send_ToJump);
    }

    // 黑幕降臨, 提示裝置該歸位了
    void SendBack(){
        Debug.Log($"Send message to arduino: {Msg_Send_ToBack}");
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

        if(msg == Msg_Recv_HandShake){
            arduinoInteractive.SendData(Msg_Send_HandShake);
        }
    }
}
