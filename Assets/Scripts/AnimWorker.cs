using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using DG.Tweening;

public class AnimWorker : MonoBehaviour
{
    public Animator anim;
    public Transform body;
    public Transform targetPoint;
    public Transform targetPoint2;
    public float duration = 5;
    public float durationJump = 0.5f;

    public Transform cam;

    [Header("Arduino 串接")]
    public ArduinoWork arduinoWork;

    [Header("黑幕設定")]
    public Image BlackScreen;
    public float fadeTime = 5;
    public float stayToBlackScreen = 5;

    public System.Action OnAnimJump;
    public System.Action OnStageBlack;
    void Start()
    {
        arduinoWork.OnStageReady += ScreenToLight;
        arduinoWork.OnAnimStartRun += AnimRun;
    }

    
    void Update()
    {
        if(Input.GetKeyDown(KeyCode.R)){
            AnimRun();
        }
        if(Input.GetKeyDown(KeyCode.J)){
            anim.SetTrigger("Jump");
        }
        if(Input.GetKeyDown(KeyCode.Space)){
            anim.SetTrigger("Recover");
            body.position = Vector3.zero;
        }
    }

    void AnimRun(){
        anim.SetTrigger("Run");
        body.DOMove(targetPoint.position, duration).SetEase(Ease.Linear).OnComplete(delegate {
            anim.SetTrigger("Jump");
            OnAnimJump?.Invoke();

            body.DOMove(targetPoint2.position, durationJump).OnComplete(delegate {
                StartCoroutine(EndAnimation());
            });
            cam.DORotate(new Vector3(-33, 0, 0), durationJump).OnComplete(delegate {
                cam.DORotate(new Vector3(0, 0, 0), durationJump * 0.5f);
            });
        });
    }

    IEnumerator EndAnimation(){
        yield return new WaitForSeconds(stayToBlackScreen);

        OnStageBlack?.Invoke();
        ScreenToBlack();
    }

    void ScreenToBlack(){
        BlackScreen.DOFade(1, fadeTime);
    }

    void ScreenToLight(){
        anim.SetTrigger("Recover");
        body.position = Vector3.zero;
        BlackScreen.DOFade(0, fadeTime);
    }
}
