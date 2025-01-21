#pragma once
#include "IContext.hpp"
#include "Player.hpp"

class ICell
{
public:
    //ICell() : countPalms(0), countPalaces(0), score(0) {}
    ICell(int sc, bool bl, char t) 
    : countPalms(0)
    , isBlue(bl)
    , countPalaces(0)
    , score(sc)
    , type(t) {}

    ~ICell() {}
    ICell& operator=(const ICell& another )
    {
        countPalaces    = another.countPalaces;
        countPalms      = another.countPalms;
        score           = another.score;
        mipples         = another.mipples;
        isBlue          = another.isBlue;
        return *this;
    }
    ICell( const ICell& another )
    {
        countPalaces    = another.countPalaces;
        countPalms      = another.countPalms;
        score           = another.score;
        mipples         = another.mipples;
        isBlue          = another.isBlue;
    }
    /*virtual ~ICell()
    {

    }*/

    virtual ICell* clone() = 0;
    virtual void fill( ICell* cell )
    {
        cell->countPalaces    = countPalaces;
        cell->countPalms      = countPalms;
        cell->score           = score;
        cell->mipples         = mipples;
        cell->isBlue          = isBlue;
        cell->type            = type;
        cell->owner           = owner;
    }
    virtual void apply(IContext* c) = 0;

    void addMipple(IMipple* mipple)
    {
        mipples.push_back(mipple);
    }
    //void removeMipple(IMipple* mipple)
    //{
    //    removeMipple(mipple->color);
    //}
    void removeMipple(IMipple::Color color)
    {
        for (auto it = mipples.begin(); it != mipples.end();)
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
    void removeAllMipples()
    {
        mipples.clear();
    }
    void removeAllMipples(IMipple* mipple)
    {
        removeAllMipples(mipple->color);
    }
    void removeAllMipples(IMipple::Color color)
    {
        for (auto it = mipples.begin(); it != mipples.end();)
        {
            if ((*it)->color == color)
                mipples.erase(it);
            else 
                it++;
        }
    }

    int getCount(IMipple::Color color)
    {
        int count = 0;
        for (auto it: mipples)
        {
            if (it->color == color)
                count++;
        }
        return count;
    }

    bool isHasColor(IMipple::Color color)
    {
        for (auto it: mipples)
        {
            if (it->color == color)
                return true;
        }
        return false;
    }

    int getFinalScore(IContext* c)
    {
        int cellScore = score + 
            c->koeffs.koeffForPalace*countPalaces +
            c->koeffs.koeffForPalm*countPalms;
        return c->player == owner ? cellScore : 0;
    }

    int countPalms = 0;
    int countPalaces = 0;
    int score = 0;
    char type;
    bool isBlue = false;
    PlayerColor owner = PlayerColor::None;
    std::vector<IMipple*> mipples;
};