#pragma once
#include <vector>
#include "Player.hpp"
//#include "IMipple.hpp"
//#include "Field.hpp"

using namespace std;

class IContext;
class IMipple
{
public:
    IMipple() {_createdMiples.push_back(this);}
    enum Color {
        Red = 0, White, Gold, Green, Blue, Invalid
    };
    Color color;
    virtual void apply(IContext* c, int x, int y, int count) = 0;
    bool operator==(const IMipple& another)
    {
        return color == another.color;
    }

    char color2char()
    {
        switch (color) 
        {
            case Red: return 'r';
            case White: return 'w';
            case Gold: return 'y';
            case Green: return 'g';
            case Blue: return 'b';
        }
        return 0;
    }
    
    static Color char2color(char ch)
    {
        switch (ch)
        {
        case 'b' : return Color::Blue;
        case 'v' :
        case 'y' : return Color::Gold;
        case 'm' :
        case 'g' : return Color::Green;
        case 'e' :
        case 'w' : return Color::White;
        case 'a' :
        case 'r' : return Color::Red;
        }
        return Color::Invalid;
    }

    static vector<IMipple*> _createdMiples;
    static void removeAllMiples()
    {
        for (auto it: _createdMiples)
            delete it;
        _createdMiples.clear();
    }
};


struct GameKoeefs
{
    int koeffForWhite = 2;
    int koeffForGold = 1;
    int koeffForPalace = 5;
    int koeffForPalm = 3;
    int koeffForBlue = 1;

    void save(FILE* f)
    {
        fprintf(f, "koeffForWhite: %d\n",     koeffForWhite);
        fprintf(f, "koeffForGold: %d\n",      koeffForGold);
        fprintf(f, "koeffForPalace: %d\n",    koeffForPalace);
        fprintf(f, "koeffForPalm: %d\n",      koeffForPalm);
        fprintf(f, "koeffForBlue: %d\n",      koeffForBlue);
    }
    void load(FILE* f)
    {
        fscanf(f, "koeffForWhite: %d",     &koeffForWhite);
        fscanf(f, "koeffForGold: %d",      &koeffForGold);
        fscanf(f, "koeffForPalace: %d",    &koeffForPalace);
        fscanf(f, "koeffForPalm: %d",      &koeffForPalm);
        fscanf(f, "koeffForBlue: %d",      &koeffForBlue);
    }
    void print()
    {
        printf("koeffForWhite: %d\n",     koeffForWhite);
        printf("koeffForGold: %d\n",      koeffForGold);
        printf("koeffForPalace: %d\n",    koeffForPalace);
        printf("koeffForPalm: %d\n",      koeffForPalm);
        printf("koeffForBlue: %d\n",      koeffForBlue);
    }
};

class IContext
{
public:
    GameKoeefs koeffs;
    int extraScore = 0;
    int money = 50;
    int countResCards = 0;
    int countFakir = 0;

    int countGold = 0;
    int countWhite = 0;
    PlayerColor player = PlayerColor::None;

    virtual int getCountBlue(int x, int y) = 0;
    //virtual void getMipples(int x, int y, vector<IMipple*>& mipples) = 0;
    virtual vector<IMipple*>& getMipples(int x, int y ) = 0;
    virtual void removeMipple(int x, int y, IMipple::Color color) = 0;
    virtual void applyCell(int x, int y) = 0;
    virtual int getScore(int x, int y) = 0;
    virtual void setOwner(int x, int y) = 0;
    virtual PlayerColor getOwner(int x, int y) = 0;
    void save(FILE* f)
    {
        koeffs.save(f);
        fprintf(f, "extraScore: %d\n",    extraScore);
        fprintf(f, "money: %d\n",         money);
        fprintf(f, "countFakir: %d\n",    countFakir);
        fprintf(f, "countResCards: %d\n", countResCards);
        fprintf(f, "countGold: %d\n",     countGold);
        fprintf(f, "countWhite: %d\n",    countWhite);
    }
    void load(FILE* f)
    {
        koeffs.save(f);
        fscanf(f, "extraScore: %d",    &extraScore);
        fscanf(f, "money: %d",         &money);
        fscanf(f, "countFakir: %d",    &countFakir);
        fscanf(f, "countResCards: %d", &countResCards);
        fscanf(f, "countGold: %d",     &countGold);
        fscanf(f, "countWhite: %d",    &countWhite);
    }
    void print()
    {
        koeffs.print();
        printf("extraScore: %d\n",    extraScore);
        printf("money: %d\n",         money);
        printf("countFakir: %d\n",    countFakir);
        printf("countResCards: %d\n", countResCards);
        printf("countGold: %d\n",     countGold);
        printf("countWhite: %d\n",    countWhite);
    }
};