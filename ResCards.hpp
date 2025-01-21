#pragma once
const int sCardScores[10] = {0,1,3,7,13,21,30,40,50,60};

class ResCard
{
public:
    enum CardType
    {
        Fakir = 0,
        Brilliant,
        FrenchFries,
        Textile,
        Wheat,
        Fish,
        Spieces
    };

    CardType CardId;
};