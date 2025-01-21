#pragma once
#include "ICell.hpp"
#include <vector>

class Field
{
public: 
    Field() 
    {
        for (int j=0; j<5; j++)
            for (int i=0; i<6; i++)
                cells[j][i] = nullptr;
    }
    ~Field() 
    {
        clear();
    }
    void clear() 
    {
        for (int j=0; j<5; j++)
            for (int i=0; i<6; i++)
                delete cells[j][i];
    }

    Field(const Field& field)
    {
        for (int j=0; j<5; j++)
            for (int i=0; i<6; i++)
                cells[j][i] = field.cells[j][i]->clone();
    }

    Field* clone() 
    {
        Field* newField = new Field();
        for (int j=0; j<5; j++)
            for (int i=0; i<6; i++)
            {
                //printf(" %ld", cells[j][i]->mipples.size() );
                newField->cells[j][i] = cells[j][i]->clone();
                //printf("field::clone %d,%d after\n", j, i ); fflush(stdout);
            }
        //printf("\n");
        return newField;
    }
    void addCell(int y, int x, ICell* cell)
    {
        cells[y][x]=cell; 
        //cellsV.push_back(cell);
    }

    ICell* cells[5][6];
    //std::vector<ICell*> cellsV;
};