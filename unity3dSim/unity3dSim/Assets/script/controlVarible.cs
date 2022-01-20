using UnityEngine;

T[] InitializeArray<T>(int length) where T : new()
{
    T[] array = new T[length];
    for (int i = 0; i < length; ++i)
    {
        array[i] = new T();
    }

    return array;
}

speedMultiplier = 1;

public class controlVariableUpdater : MonoBehaviour{
    planet[] planetList = new InitializeArray<planet>(planetNum);
}