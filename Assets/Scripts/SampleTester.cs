using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using DG.Tweening;

public class SampleTester : MonoBehaviour
{
    public Animator anim;
    public Transform body;
    public Transform targetPoint;
    public Transform targetPoint2;
    public float duration = 5;
    public float duration2 = 0.5f;

    public Transform cam;
    //Vector3 basePoint = 
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if(Input.GetKeyDown(KeyCode.R)){
            anim.SetTrigger("Run");
            body.DOMove(targetPoint.position, duration).SetEase(Ease.Linear).OnComplete(delegate {
                anim.SetTrigger("Jump");
                body.DOMove(targetPoint2.position, duration2);
                cam.DORotate(new Vector3(-33, 0, 0), duration2).OnComplete(delegate {
                    cam.DORotate(new Vector3(0, 0, 0), duration2*0.5f);
                });
            });
        }
        if(Input.GetKeyDown(KeyCode.J)){
            anim.SetTrigger("Jump");
        }
        if(Input.GetKeyDown(KeyCode.Space)){
            anim.SetTrigger("Recover");
            body.position = Vector3.zero;
        }


    }
}
