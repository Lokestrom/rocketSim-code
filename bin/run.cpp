#include "rocketSimObjects.cpp"

int main()
{
    setCoutMode();
    rocketStage rocketx(100, 50, 0, 2, 345, false, 6371000, 5.972E24, 0);
    rocketx.startUp(90, 0);
    rocketx.idle(1);
}