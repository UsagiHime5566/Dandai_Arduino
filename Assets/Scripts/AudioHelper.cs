using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class AudioHelper : MonoBehaviour
{
    public AudioSource AS_BGM;
    public AudioSource AS_Scream;
    public Slider SLD_BGM;
    public Slider SLD_Scream;

    void Start()
    {
        SLD_BGM.SetSavedDataFloat("V_BGM", 1, x => {
            AS_BGM.volume = x;
        });

        SLD_Scream.SetSavedDataFloat("V_Scream", 1, x => {
            AS_Scream.volume = x;
        });
    }
}
