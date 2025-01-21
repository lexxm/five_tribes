#pragma once
#include <vector>
//#include "Gin.hpp"
#include "ResCards.hpp"
#include "Field.hpp"

class Context : public IContext
{
public:
    int getScore() 
    {
        int score = money;
        int countPalaces = 0;
        int countPalms = 0;
        score += countResCards*5;//sCardScores[countResCards];
        int myScore = 0;
        for (int j=0; j<5; j++)
            for (int i=0; i<6; i++)
            {
                auto it = field->cells[j][i];
                if (it->owner == player)
                {
                    score += it->getFinalScore(this);
                    myScore += it->getFinalScore(this);
                }
            }

        score += countGold * koeffs.koeffForGold;
        score += countWhite * koeffs.koeffForWhite;
        score += extraScore;
        if ( countGold >= 3 )
            score += 0;

        /*if (score == 71)
        {
            printf("all: %d\nresource: %d\nowner: %d\ngold: %d\nwhite: %d\nextra: %d\n", 
                score, sCardScores[countResCards], 
                myScore, countGold, countWhite, extraScore );
        }*/
        return score;
    }
    int getCountBlue(int x, int y) override
    {
        int countBlue = 0;
        for (int j = max(y-1, 0); j <= min(y+1,4); j++)
        for (int i = max(x-1, 0); i <= min(x+1,5); i++)
        {
            if ( field->cells[j][i]->isBlue )
                countBlue++;
        }
        return countBlue;
    }
    //void getMipples(int x, int y, vector<IMipple*>& mipples) override
    vector<IMipple*>& getMipples(int x, int y) override
    {
        return field->cells[y][x]->mipples;
        //mipples = field->cells[y][x]->mipples;
    }
    void removeMipple(int x, int y, IMipple::Color color) override
    {
        auto& mipples = field->cells[y][x]->mipples;
        for (auto it = mipples.begin(); it != mipples.end(); )
        {
            if ((*it)->color == color)
            {
                mipples.erase(it);
                break;
            }
            else
                it++;
        }
    }
    void applyCell(int x, int y) override
    {
        field->cells[y][x]->apply(this);
    }
    int getScore(int x, int y) override
    {
        return field->cells[y][x]->score;
    }
    void setOwner(int x, int y) override
    {
        field->cells[y][x]->owner = player;
    }
    PlayerColor getOwner(int x, int y) override
    {
        return field->cells[y][x]->owner;
    }

    Context() : field(nullptr) {}
    ~Context() {};

    void setField( Field* f) { field = f; }
    Context* clone()
    {
        Context* newContext = new Context;
        //printf("field %p\n", field);
        newContext->field = field->clone();
        //printf("field after clone %p\n", field);

        newContext->koeffs = koeffs;
        newContext->extraScore = extraScore;
        newContext->money = money;
        newContext->countResCards = countResCards;

        newContext->countGold = countGold;
        newContext->countWhite = countWhite;
        newContext->player = player;
        //printf("context end clone\n");
        return newContext;
    }

    Field* field;
};