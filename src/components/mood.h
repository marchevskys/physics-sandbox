#ifndef MOOD_H
#define MOOD_H


class Mood
{
public:
    float anger = 0.0f;
    float maxLevel = 1.0f;
    bool lovePeople = false;
    Mood() {
        //anger = glm::linearRand(0.0f, 1.0f);
        //lovePeople = rand() % 2;
    }

    bool loveSociety(bool someoneIsNearby) {
        if (anger < 0)
            lovePeople = true;
        else if (anger > maxLevel)
            lovePeople = false;
        anger = someoneIsNearby ? (anger += 0.01f) : (anger -= 0.01f);
        anger = anger < -maxLevel ? -maxLevel : anger;
        anger = anger > maxLevel * 2 ? maxLevel * 2 : anger;
        return lovePeople;
    }
};

#endif // MOOD_H
