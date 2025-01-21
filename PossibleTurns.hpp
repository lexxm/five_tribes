#pragma once
#include <vector>
#include <stdio.h>

using namespace std;

struct Point
{
    int dx, dy;
    Point() : dx(0), dy(0) {}
    Point( int _dx, int _dy ) : dx(_dx), dy(_dy) {}
    bool operator==( const Point& p) { return p.dx==dx && p.dy==dy; }
    Point& operator+=( const Point& p) { dx+=p.dx; dy += p.dy; return *this; }
};



class Sequence: public std::vector<Point>
{
public:
    bool isCircle = false;
    /*Sequence& operator=(const Sequence& another)
    {
        return *this;
    }*/
    bool isOnField(int x, int y)
    {
        for (auto& p: *this)
            if (x+p.dx<0 || x+p.dx>=6 || y+p.dy<0 || y+p.dy>=5)
                return false;
        return true;
    }
    void printSequence()
    {
        int k=0;
        for ( auto it: *this )
        {
            if ( k > 0 )
                printf("->");
            printf( "[%d,%d]", it.dx, it.dy );
            k++;
        }
        printf("\n");
    }

    void add2File()
    {
        char filename[64];
        sprintf(filename, "turns_%ld.txt", size());
        if (FILE* f = fopen(filename, "at"))
        {
            for (auto it: *this)
                fprintf(f, "%d,%d;", it.dx, it.dy);
            fprintf(f, "\n");
            fclose(f);
        }
    }

    void convert(Sequence& seqOut)
    {
        Point p(0,0);
        bool isFind = false;
        for (auto it: *this)
        {
            p+=it;
            for (auto jt: seqOut)
            {
                if (p==jt)
                    isFind = true;
            }
            seqOut.push_back(p);
        }
        seqOut.isCircle = isFind;
    }
};


void addTurn(int currK, int endK, Sequence& current, vector<Sequence>& result)
{
    if ( currK > endK )
    {
        Sequence convertedSeq;
        current.convert(convertedSeq);
        Point last = convertedSeq.back();
        //printSequence(current);
        if ( (endK==4 || endK==6 ) && last == Point(0,0) )
        {
            //rintf("circle\n");
            return;
        }
        result.push_back(convertedSeq);
        //convertedSeq.add2File();
        return;
    }
    for (int dx = -1; dx <= 1; dx +=2)
    {
        Point prev = current.empty() ? Point(0,0) : current.back();
        Point curr(dx,0);
        if ( prev.dx*curr.dx == -1 || prev.dy*curr.dy == -1)
            continue;
        Sequence copySeq = current;
        copySeq.push_back(curr);
        addTurn( currK+1, endK, copySeq, result );
    }
    for (int dy = -1; dy <= 1; dy +=2)
    {
        Point prev = current.empty() ? Point(0,0) : current.back();
        Point curr(0,dy);
        if ( prev.dx*curr.dx == -1 || prev.dy*curr.dy == -1)
            continue;
        Sequence copySeq = current;
        copySeq.push_back(curr);
        addTurn( currK+1, endK, copySeq, result );
    }
}