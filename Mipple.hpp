#pragma once
//#include "IMipple.hpp"
#include "IContext.hpp"

class RedMipple : public IMipple
{
public:
    RedMipple()
    : IMipple()
    {
        color=IMipple::Color::Red;
    }
    virtual void apply(IContext* c, int x, int y, int count)
    {
        vector<pair<int,int>> variants;
        for (int j = max(y-count, 0); j <= min(y+count,4); j++)
        for (int i = max(x-count, 0); i <= min(x+count,5); i++)
        {
            int dist = abs(x-i)+abs(y-j);
            if ( dist > count )
                continue;
            vector<IMipple*>& mipples = c->getMipples(i, j);
            if (c->getOwner(i,j)==PlayerColor::None &&
                (mipples.size() == 1 || mipples.size() == 2 && mipples[0]->color==mipples[1]->color))
                variants.push_back(make_pair(i,j));
        }
        if (variants.empty())
            return;
        size_t bestScoreInd=0, bestScore = 0;
        for (size_t i=0; i<variants.size(); i++)
        {
            auto& p = variants[i];
            int score = c->getScore(p.first, p.second);
            if (bestScore < score)
            {
                bestScore = score;
                bestScoreInd = i;
            }
        }
        auto& p = variants[bestScoreInd];
        int px = p.first, py = p.second;
        c->getMipples(px, py).clear();
        c->setOwner(px, py);
    }
};

class GreenMipple : public IMipple
{
public:
    GreenMipple()
    : IMipple()
    {
        color=IMipple::Color::Green;
    }
    // add Resource cards
    virtual void apply(IContext* c, int i, int j, int count)
    {
        int countResCards = int((count)/2.0f+0.51f);
        c->countResCards += countResCards;
        c->countFakir += (count - countResCards);
    }
};

class BlueMipple : public IMipple
{
public:
    BlueMipple()
    : IMipple()
    {
        color=IMipple::Color::Blue;
    }
    virtual void apply(IContext* c, int i, int j, int count) 
    {
        c->money += (count+c->countFakir)*c->getCountBlue(i, j);
    }
};

class WhiteMipple : public IMipple
{
public:
    WhiteMipple()
    : IMipple()
    {
        color=IMipple::Color::White;
    }
    virtual void apply(IContext* c, int i, int j, int count)
    {
        c->countWhite += count;
    }
};

class GoldMipple : public IMipple
{
public:
    GoldMipple()
    : IMipple()
    {
        color=IMipple::Color::Gold;
    }
    // TODO compare with another players and add extra points
    virtual void apply(IContext* c, int i, int j, int count)
    {
        c->countGold += count;
    }
};

IMipple* char2Mipple(char ch)
{
    switch (ch)
    {
    case 'b' : return new BlueMipple();
    case 'y' : return new GoldMipple();
    case 'g' : return new GreenMipple();
    case 'w' : return new WhiteMipple();
    case 'r' : return new RedMipple();
    }
    return nullptr;
}
