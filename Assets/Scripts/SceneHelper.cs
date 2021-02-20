using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SceneHelper : MonoBehaviour
{
    public AnimWorker animWorker;
    public InputField INP_RunTime;
    public InputField INP_JumpTime;
    public InputField INP_FadeTime;
    public InputField INP_ExitTime;
    void Start()
    {
        INP_RunTime.SetSavedDataFloat("Scene_RunTime", 10, x => {
            animWorker.duration = x;
        });

        INP_JumpTime.SetSavedDataFloat("Scene_JumpTime", 0.5f, x => {
            animWorker.durationJump = x;
        });

        INP_FadeTime.SetSavedDataFloat("Scene_FadeTime", 5, x => {
            animWorker.fadeTime = x;
        });

        INP_ExitTime.SetSavedDataFloat("Scene_ExitTime", 5, x => {
            animWorker.stayToBlackScreen = x;
        });

    }

}
