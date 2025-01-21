#pragma once

#include <stdio.h>
#include <vector>
#include <map>
#include <set>
#include "Mipple.hpp"
#include "Cell.hpp"
#include "Context.hpp"
#include "PossibleTurns.hpp"

class TurnFinder
{
public:
    struct OneTurn
    {
        int x,y;
        //IMipple::Color color;
        IMipple* mipple;
        OneTurn() :x(-1), y(-1), mipple(nullptr) {}
        OneTurn(int _x, int _y, IMipple* _m) :x(_x), y(_y), mipple(_m) {}
        void print() {printf("%c%d %c\n", 'A'+x, y+1, mipple ? mipple->color2char(): '-');}
    };

    struct Turn
    {
        vector<OneTurn> turns;//, nextTurns;
        int score = 0;
        int nextTurnScore = 0;
        void print()
        {
            if (nextTurnScore)
                printf("=== will get %d     next: %d===\n", score, nextTurnScore);
            else
                printf("=== will get %d ===\n", score);
            for(auto& it: turns) 
                it.print();
        }

        void fill(int i, int j, Sequence& seq, vector<IMipple*>& mipples)
        {
            //Turn turn;
            OneTurn firstTurn(i, j, nullptr);
            turns.push_back(firstTurn);
            for (size_t k=0; k<seq.size(); k++)
            {
                OneTurn oneTurn;
                oneTurn.x = seq[k].dx+i;
                oneTurn.y = seq[k].dy+j;
                oneTurn.mipple = mipples[k];
                turns.push_back(oneTurn);
            }
        }
    };

    /*struct SeqTurns
    {
        vector<Turn> Turns;
    };*/

    enum TurnType 
    {
        Max,
        MaxMax,
        MaxMin,
        MaxMaxMin,
        MaxMaxMax
    };

    TurnFinder(Context* context, map<int,vector<Sequence>>* turns, int finalDeep) 
        : _c(context) 
        , _pt(turns)
        , _finalDeep(finalDeep)
        {}

    int run( int deep, vector<Turn>& seqTurns, TurnType turnType )
    {
        Field* field = _c->field;
        _countProcessed = 0;
        _countSkipped = 0;
        
        int bestScore = 1;
        if (turnType == TurnType::MaxMin || turnType == TurnType::MaxMin )
            bestScore = -20;
        int scoreBefore = _c->getScore();
        Turn bestTurn; 
        vector<Turn> bestSeqTurns;

        for (int j=0; j<5; j++)
        for (int i=0; i<6; i++)
        {
            if (deep == 0)
            {
                printf("%c%c..", i+'A', j+'1');
                if (i==5)
                    printf("\n");
                fflush(stdout);
            }
            // это пеербор по ячейкам(начало хода)
            ICell* cell = field->cells[j][i];
            size_t countMipples = cell->mipples.size();
            if (countMipples == 0)
            {
                _countSkipped++;
                continue;
            }
            vector<Sequence>& allTurns = (*_pt)[countMipples];

            // заполняем варианты рассановки миплов(последовательность)
            vector<vector<IMipple*>> turnVariants;
            if (turnType == MaxMin || turnType == MaxMaxMin)
                fillPossibleMipplesPositionsShort(cell->mipples, turnVariants);
            else
                fillPossibleMipplesPositionsFull(cell->mipples, turnVariants);
            //printf("(%d, %d) %ld %ld\n", i, j, cell->mipples.size(), turnVariants.size() );

            // перебор по возможным комбинациям хода
            for (auto it: allTurns)
            {
                if (!it.isOnField(i,j))
                {
                    _countSkipped++;
                    continue;
                }

                Point lp = it.back();
                int lastX = i+lp.dx;
                int lastY = j+lp.dy;
                ICell* lastCellGlobal = field->cells[lastY][lastX];
                bool isIgnore = false;
                for (auto itIgnr: _ignoreFields)
                {
                    if ( lastX == itIgnr.dx && lastY == itIgnr.dy )
                    {
                        _countSkipped++;
                        isIgnore = true;
                    }
                }
                // проверяем есть ли в последней ячейке кто нибудь
                if (i==1&&j==3&&lastX==1&&lastY==2)
                    int z =0; 
                
                if ((lastCellGlobal->mipples.empty() && !it.isCircle) || isIgnore)
                {
                    _countSkipped++;
                    continue;
                }

                for (auto jt : turnVariants)
                {
                    if ( !lastCellGlobal->isHasColor(jt.back()->color) && !it.isCircle)
                    {
                        _countSkipped++;
                        continue;
                    }
                    Context* c = _c->clone();// TODO clone only one time, in other case, just copy values to cell
                    // DO NOT allocate memory every time and remove it
                    _countProcessed++;

                    ICell* lastCell = c->field->cells[lastY][lastX];
                    c->field->cells[j][i]->removeAllMipples();
                    //printf("debug1\n"); fflush(stdout);
                    IMipple* lastMipple = jt.back();
                    //printf("debug2\n"); fflush(stdout);
                    for (size_t k=0; k<jt.size(); k++)
                    {
                        Point& p = it[k];
                        IMipple* m = jt[k];
                        //printf("debug2.1 %d %d\n", j+p.dy, i+p.dx); fflush(stdout);
                        ICell* cell = c->field->cells[j+p.dy][i+p.dx];
                        //printf("debug2.2 %p %p\n", cell, m); fflush(stdout);
                        cell->addMipple(m);
                        //printf("debug2.3\n"); fflush(stdout);
                    }
                    //printf("debug3\n"); fflush(stdout);
                    int countMiples = lastCell->getCount(lastMipple->color);
                    if (countMiples < 2 )
                    {
                        _countSkipped++;
                        delete c->field;
                        delete c;
                        continue;
                    }
                    lastCell->removeAllMipples(lastMipple->color);
                    //if ( lastX==5 && lastY==4 && deep == 0/*&& it.back().dx+i == 0 && it.back().dy+j == 2*/ && jt.back()->color == IMipple::Color::Green )
                    //    int debug = 0;
                    lastMipple->apply(c, lastX, lastY, countMiples);
                    lastCell->apply(c);
                    if (lastCell->mipples.empty() && lastCell->owner == PlayerColor::None)
                        lastCell->owner = c->player;
                    
                    int score = c->getScore()-scoreBefore;
                    //------fillTurn------------------------------
                    // temp code for check
                    vector<Turn> localTurns=seqTurns;
                    Turn turn;
                    turn.fill(i, j, it, jt);
                    turn.score = score;
                    localTurns.push_back( turn );
                    if (deep < _finalDeep)
                    {
                        TurnFinder tf(c, _pt, _finalDeep);
                        tf._ignoreFields = _ignoreFields;
                        score += tf.run(deep+1, localTurns, TurnType::Max);
                    }

                    int scoreToCompare = 0;
                    if (turnType == MaxMax || turnType == MaxMaxMax)
                        for (auto it: localTurns)
                            scoreToCompare += it.score;
                    else if (turnType == Max)
                        scoreToCompare = score;
                    else if (turnType == MaxMin) 
                        scoreToCompare = localTurns[0].score-localTurns[1].score;
                    else if (turnType == MaxMaxMin)
                        scoreToCompare = localTurns[0].score+localTurns[1].score-localTurns[2].score;
                    
                    //if (score > bestScore || ( score == bestScore && deep == 0 ))
                    if (scoreToCompare > bestScore )
                    {
                        bestScore = scoreToCompare;
                        bestSeqTurns = localTurns;
                    }
                    delete c->field;
                    delete c;
                }
            }
        }
        if (deep == 0)
        {
            printf("count processed: %ld    count skipped %ld\nbest turn:\n", _countProcessed, _countSkipped );
            //bestTurn.print();
            for (auto turns: bestSeqTurns)
                turns.print();
        }
        else
        {
            //printf("deep %d:\n", deep );
            //bestTurn.print();
        }
        seqTurns = bestSeqTurns;
        return bestScore;//bestTurn;
    }
protected:
    void fillPossibleMipplesPositionsFull(vector<IMipple*>& in, vector<vector<IMipple*>>& out)
    {
        out.clear();

        for ( auto it: in )
        {
            vector<IMipple*> newSeq;
            newSeq.push_back(it);
            out.push_back(newSeq);
        }
        add(in, 1, (int)in.size()-1, out);
    }
    void fillPossibleMipplesPositionsShort(vector<IMipple*>& in, vector<vector<IMipple*>>& out)
    {
        out.clear();
        set<IMipple::Color> seqColors;
        for (auto it: in)
            seqColors.insert(it->color);
        //for (int i=0; i<seqColors)
        for (auto it: seqColors)
        {
            vector<IMipple*> seq;
            //IMipple* endMipple = nullptr;
            size_t useInd = 0;
            for (size_t j=0; j<in.size(); j++)
            {
                if (it == in[j]->color)
                    useInd = j;
            }
            for (size_t j=0; j<in.size(); j++)
            {
                if (j != useInd)
                    seq.push_back(in[j]);
            }
            seq.push_back(in[useInd]);
            out.push_back(seq);
        }        
    }
    void add(vector<IMipple*>& in, int ind, int maxInd, vector<vector<IMipple*>>& out)//
    {
        vector<vector<IMipple*>> res;

        set<IMipple::Color> seqColors;
        // если она уже добавлена
        for ( auto it: in )
        {
            //seq.push_back(nullptr);
            if (seqColors.find(it->color) != seqColors.end())
                continue;
            seqColors.insert(it->color);
            for (auto seq: out)
            {
                bool isAlreadyInSeq = false;
                // check, do we have or not
                for (auto jt: seq)
                    if (it == jt)
                        isAlreadyInSeq=true;
                if (!isAlreadyInSeq)
                {
                    //if (seq.back()==nullptr)
                    //    seq.back()=it;
                    //else
                    //{
                        //vector<IMipple*> newSeq = seq;
                        //ewSeq.
                    //}
                    seq.push_back(it);
                    res.push_back(seq);
                }
                //if (!isAlreadyInSeq)
            }
        }
        out = res;
        if (ind < maxInd)
            add(in, ind+1, maxInd, out);
    }
public:
    Context* _c;
    int _finalDeep = 1;
    map<int,vector<Sequence>>* _pt;
    vector<Point> _ignoreFields;
    unsigned long _countProcessed = 0;
    unsigned long _countSkipped = 0;
};