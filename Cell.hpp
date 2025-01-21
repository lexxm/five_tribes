#pragma once
#include <vector>
#include <memory>
//#include "Mipple.hpp"
#include "Player.hpp"
#include "ICell.hpp"

class IMipple;
#define DEFUALT_VALUE_FOR_MARKET_CARD 5
// TODO: what about blue mipple?

class GinCell : public ICell
{
public: 
    GinCell(int sc, bool bl) : ICell(sc, bl, 'g') {}

    virtual ICell* clone()
    {
        GinCell* newCell = new GinCell(score, isBlue);
        this->fill(newCell);
        return newCell;
    }
    
    virtual void apply(IContext* c)
    {
        if (c->countWhite>=1 && c->countFakir>=1)
        {
            c->extraScore += 6;
            c->countWhite -= 1;
            c->countFakir -= 1;
        } else if (c->countWhite >= 2)
        {
            c->extraScore += 6;
            c->countWhite -= 2;
        }
    }
};

class SmallMarketCell : public ICell
{
public:
    SmallMarketCell(int sc, bool bl) : ICell(sc, bl, 's') {}

    virtual ICell* clone()
    {
        SmallMarketCell* newCell = new SmallMarketCell(score, isBlue);
        this->fill(newCell);
        return newCell;
    }
    virtual void apply(IContext* c)
    {
        if (c->money < 3)
            return;
        c->money -= 3;
        c->countResCards++;
    }
};

class BigMarketCell : public ICell
{
public:
    BigMarketCell(int sc, bool bl) : ICell(sc, bl, 'b') {}
    virtual ICell* clone()
    {
        BigMarketCell* newCell = new BigMarketCell(score, isBlue);
        this->fill(newCell);
        return newCell;
    }
    virtual void apply(IContext* c)
    {
        if (c->money < 6)
            return;
        c->money -= 6;
        c->countResCards+=2;
        c->countFakir++;
    }
};

class PalmCell : public ICell
{
public:
    PalmCell(int sc, bool bl) : ICell(sc, bl, 'p') {}
    virtual ICell* clone()
    {
        PalmCell* newCell = new PalmCell(score, isBlue);
        this->fill(newCell);
        return newCell;
    }
    virtual void apply(IContext* c)
    {
        countPalms++;
    }
};

class PalaceCell : public ICell
{
public:
    PalaceCell(int sc, bool bl) : ICell(sc, bl, 'd') {}
    virtual ICell* clone()
    {
        PalaceCell* newCell = new PalaceCell(score, isBlue);
        this->fill(newCell);
        return newCell;
    }
    virtual void apply(IContext* c)
    {
        countPalaces++;
    }
};