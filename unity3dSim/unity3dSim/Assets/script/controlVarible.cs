using UnityEngine;
using System.Collections;
using UnityEngine.UI;

namespace unity3dRocketSim{

public class ControlVariables{

    public text speedMultiplierInput;

    float timer = 0, speedMultiplier = 1;
    void startControlVariables(){
        speedMultiplierInput = GetComponent<Text>();
    }
    
    
    void updateContorlVarables(){
        timer += Time.deltaTime;
    
        foreach (char c in Input.inputString)
        {
            if (c == '\b') // has backspace/delete been pressed?
            {
                if (speedMultiplierInput.text.Length != 0)
                {
                    speedMultiplierInput.text = speedMultiplierInput.text.Substring(0, speedMultiplierInput.text.Length - 1);
                }
            }
            else if ((c == '\n') || (c == '\r')) // enter/return
            {
                speedMultiplier = stof(speedMultiplierInput.text);
                timer = timer/speedMultiplier;
            }
            else
            {
                speedMultiplierInput.text += c;
            }
        }
    }
}
}