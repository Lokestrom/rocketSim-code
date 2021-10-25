#include "rocketsimObj.cpp"

int main()
{
    std::cout << std::fixed;
    int i = 0;
    planet planet1(1, 100000, 1, {0, 0, 0}, {0, 0, 0});
    planet planet2(2, 200000, 2, {1, 0, 0}, {1, 0, 0});
    std::list<planet> listOfPlanets = {planet1, planet2};
    std::list<fixedPlanet> listOfFixedPlanets = {};
    asignelistOfplanets(listOfPlanets, listOfFixedPlanets);

    /*rocketStage rocketStage1(2, 1);
    while (i < 1)
    {
        rocketStage1.rocketUpdate(0, i);
        planetUpdate();
        i++;
    }*/
}