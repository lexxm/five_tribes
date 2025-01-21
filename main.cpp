#include <stdio.h>

#include "FindTurn.hpp"
/*
#include <vector>
#include <map>
#include "Mipple.hpp"
//#include "Gin.hpp"
#include "Cell.hpp"
#include "Context.hpp"
#include "PossibleTurns.hpp"*/
#include <string.h>

using namespace std;
char baseStr[]="%c%c %c%c%c %d %d %d %d ";
vector<IMipple*> IMipple::_createdMiples;
// coords type-blue-owner score count_palm-count_palace count_miples
//char baseSt2[]="%c%d %c%c%c %d %d "
void print( Field* field );
void load( Field* field, map<PlayerColor,Context*> &contexts, const char* str );
void clearField( Field* field );

ICell* getCellFromChar(char type, int score, bool isBlue)
{
    ICell* cell = nullptr;
    //printf("getCellFromChar enter\n"); fflush(stdout);
    switch (type)
    {
        case 'g': cell = new GinCell(score, isBlue); break;//printf("create GinCell\n"); 
        case 's': cell = new SmallMarketCell(score, isBlue); break;//printf("create SmallMarketCell\n"); 
        case 'b': cell = new BigMarketCell(score, isBlue); break;//printf("create BigMarketCell\n"); 
        case 'p': cell = new PalmCell(score, isBlue); break;//printf("create PalmCell\n"); 
        case 'd': cell = new PalaceCell(score, isBlue); break;//printf("create PalaceCell\n"); 
        default: printf("=== wrong type %c ====\n", type);
    }
    //printf("getCellFromChar exit\n"); fflush(stdout);
    return cell;
}

int main()
{
    map<int,vector<Sequence>> possibleTurns;
    for ( int k=1; k<=6; k++ )
    {
        vector<Sequence> result;
        Sequence current;
        addTurn( 1, k, current, result );
        printf( "%d %lu\n", k, result.size() );
        possibleTurns[k]=result;
    }

    char helpStr[] = "commands list:\n"
        " TODO, add MThr, +maxMin, +check memory leacks, git, check amount mipples, check stat, +print stat, use count of players, add market cards\n"
        " * exit\n"
        " * random_init\n"
        " * add %%c,%%c %%c - add mipple to cell\n"
        " * rm %%c,%%c %%c - rm mipple from cell\n"
        " * cell %%c,%%c %%c%%c %%d %%d - add cell\n"
        " * calc - %c %d %c%c calculate variant for player, count ignore fields, fields...\n"
        " * save %%s - save field\n"
        " * load %%s - load field\n" 
        " * palm %%c,%%c - add palm\n"
        " * palace %%c,%%c - add palace\n"
        " * print - print field\n"
        " * turn %%c%%c %%c [%%c%%c %%c] []...(addr_start, player)  (addr, mipple color) - make a turn\n"
        " * play %%c,%%c %%c (addr, player)- play field\n"
        " * money %%c %%d - add money\n"
        " * set %c,%c %c - (addr, player) set owner\n\n"; 
    printf("%s", helpStr);//fflush(stdout);

    Field* field = new Field;
    int countPlayers=4;
    map<PlayerColor,Context*> contexts;// = new Context*[countPlayers];
#define CREATE_COTEXT(color)            \
    contexts[color] = new Context;      \
    contexts[color]->setField(field);   \
    contexts[color]->player = color;

    CREATE_COTEXT(PlayerColor::Blue);
    CREATE_COTEXT(PlayerColor::Purple);
    CREATE_COTEXT(PlayerColor::Orange);
    CREATE_COTEXT(PlayerColor::Fekete);

    //char filename[64];
    //sprintf(filename, "test");
    //string filename("test");
    load(field, contexts, "594633775");
    //printf("load done2 \n"); fflush(stdout);

    while (true)
    {
        char str[128];
        printf("> ");
        scanf("%s", str);
        if (str[0]=='\n')
            continue;
        if (!strcmp(str,"exit") || !strcmp(str,"quit") || !strcmp(str,"q") )
            break;
        if (!strcmp(str,"help") )
            printf("%s", helpStr);
        else if (!strcmp(str,"random_init"))
        {
            int countMiples = 3;
            char mippleColor[] = "bwgyr";
            char cellType[] = "dpgsb";
            for (int j=0; j<5; j++)
            for (int i=0; i<6; i++)
            {
                bool isBlue = (rand()%3 == 0);
                char type = cellType[rand()%5];
                int score = rand()%4+6;
                //printf("(%d,%d) %c", i, j, type); fflush(stdout);
                ICell *cell = getCellFromChar( type, score, isBlue );
                for (int i=0; i<countMiples; i++)
                {
                    int c = rand()%5;
                    cell->addMipple(char2Mipple(mippleColor[c]));
                }
                field->addCell(j, i, cell);
            }
        }
        else if (!strcmp(str,"add"))
        {
            char addr[2];
            char ch;
            scanf(" %c%c %c", &addr[0], &addr[1], &ch);
            int x = addr[0]-'a';
            int y = addr[1]-'1';
            
            //printf("%d,%d %c\n", y,x,ch);
            ICell* cell=field->cells[y][x];
            cell->addMipple(char2Mipple(ch));
            printf("add done\n"); fflush(stdout);
        }
        else if (!strcmp(str,"rm"))
        {
            char addr[2];
            char ch;
            scanf(" %c%c %c", &addr[0], &addr[1], &ch);
            int x = addr[0]-'a';
            int y = addr[1]-'1';
            //printf("%d %d\n", x, y); fflush(stdout);
            
            ICell* cell=field->cells[y][x];
            //printf("%p %c\n", cell, ch); fflush(stdout);
            cell->removeMipple(IMipple::char2color(ch));
            printf("rm done\n"); fflush(stdout);
        }
        else if (!strcmp(str,"cell"))
        {
            char addr[2];
            char type, color;
            int score, countMiples;
            scanf(baseStr, &addr[0], &addr[1], //"%c%c %c%c %d %d"
                &type, &color, &score, 
                &countMiples);
            int x = addr[1]-'a';
            int y = addr[0]-'1';
            ICell *cell = getCellFromChar( type, score, color == 'b');
            for (int i=0; i<countMiples; i++)
            {
                char ch;
                scanf("%c", &ch);
                cell->addMipple(char2Mipple(ch));
            }
            //printf("(%d,%d) %c%c%c\n", x, y, m[0], m[1], m[2]);
            field->addCell(y, x, cell);
        }
        else if (!strcmp(str,"calc"))
        {
            char player;
            int countIgnore;
            char type[10];
            scanf(" %s %c %d", type, &player, &countIgnore);

            PlayerColor plColor = char2PlayerColor(player);
            Context* context = contexts[plColor];
            TurnFinder turnFinder(context, &possibleTurns, 1);

            char addr[2];
            for (int k=0; k<countIgnore; k++)
            {
                scanf(" %c%c", &addr[0], &addr[1]);
                int x = addr[0]-'a';
                int y = addr[1]-'1';
                turnFinder._ignoreFields.push_back( Point(x,y) );
            } 

            TurnFinder::TurnType turnType;
            if (!strcmp("maxmax", type))
            {
                turnType = TurnFinder::TurnType::MaxMax;
                turnFinder._finalDeep = 1;
            }
            else if (!strcmp("max", type))
            {
                turnType = TurnFinder::TurnType::Max;
                turnFinder._finalDeep = 0;
            }
            else if (!strcmp("maxmaxmax", type))
            {
                turnType = TurnFinder::TurnType::MaxMaxMax;
                turnFinder._finalDeep = 2;
            }
            else if (!strcmp("maxmin", type))
            {
                turnType = TurnFinder::TurnType::MaxMin;
                turnFinder._finalDeep = 1;
            }
            else if (!strcmp("maxmaxmin", type))
            {
                turnType = TurnFinder::TurnType::MaxMaxMin;
                turnFinder._finalDeep = 2;
            }
            vector<TurnFinder::Turn> seqTurns;
            turnFinder.run(0, seqTurns, turnType);
            printf("calc done\n");
        }
        else if (!strcmp(str,"turn"))
        {
            char addr[2], player;
            scanf(" %c%c %c", &addr[0], &addr[1], &player);
            int x = addr[0]-'a';
            int y = addr[1]-'1';
            
            ICell* cell=field->cells[y][x];
            vector<IMipple*> mipples = cell->mipples;
            cell->mipples.clear();

            char ch;
            do {
                scanf(" %c%c %c", &addr[0], &addr[1], &ch);
                x = addr[0]-'a';
                y = addr[1]-'1';
                cell=field->cells[y][x];
                IMipple::Color c = IMipple::char2color(ch);
                //std::find(mipples.begin(), mipples.end(), [](){return true;})
                //std:erase_if(mipples.begin(), mipples.end(),);// mipplesmassage.erase()
                bool isRemove = false;
                for (auto it = mipples.begin(); it != mipples.end(); )
                {
                    if ((*it)->color == c)
                    {
                        cell->addMipple(*it);
                        mipples.erase(it);
                        isRemove = true;
                        break;
                    }
                    else
                        it++;
                }
                if (isRemove)
                    printf("remove %c (%d,%d)\n", ch, x, y );
            } while (!mipples.empty());

            IMipple::Color color = IMipple::char2color(ch);
            int countMiples = 0;
            IMipple* finalMiple = nullptr;
            for (auto it: cell->mipples)
                if (it->color == color)
                {
                    countMiples++;
                    finalMiple = it;
                }

            //printf("final mipple: %p\n", finalMiple);
            PlayerColor plColor = char2PlayerColor(player);
            Context* context = contexts[plColor];
            if (countMiples >= 2)//TODO only one color, or last mipple was red
            {
                cell->removeAllMipples(color);

                //printf("score before: %d\n", context->getScore());
                int score = context->getScore();
                finalMiple->apply(context, x, y, countMiples);
                
                cell->apply(context);
                if (cell->mipples.empty())
                    cell->owner = plColor;
                //printf("score after: %d\n", context->getScore());
                printf("got %d\n", context->getScore() - score);
            }
            printf("turn done\n");
        }
        else if (!strcmp(str,"save"))
        {
            char str[64];
            scanf("%s", str);
            FILE* f = fopen(str, "wt");
            for (int j=0; j<5; j++)
            for (int i=0; i<6; i++)
            {
                ICell* c = field->cells[j][i];
                fprintf(f, baseStr,//"%c%d %c%c%s %d %ld ", 
                    i+'A', j+'1', 
                    c->type, 
                    c->isBlue ? 'b' : 'r',
                    playerColor2char(c->owner),
                    c->countPalms,
                    c->countPalaces,
                    c->score,
                    c->mipples.size() );
                for (auto it: c->mipples)
                    fprintf(f, "%c", it->color2char());
                fprintf(f, "\n");
            }
            for (auto it: contexts)
                it.second->save(f);
            fclose(f);
            printf("save done\n"); fflush(stdout);
        }
        else if (!strcmp(str,"load"))
        {
            char str[64];
            scanf("%s", str);
            load(field, contexts, str);
            printf("load done\n"); fflush(stdout);
        }
        else if (!strcmp(str,"print2"))
        {
            for (int j=0; j<5; j++)
            for (int i=0; i<6; i++)
            {
                //printf("%d %d ", j, i ); fflush(stdout);
                ICell *c = field->cells[j][i];
                //printf("%p\n", c); fflush(stdout);
                printf(baseStr, 
                    i+'A', j+1, 
                    c->type,
                    c->isBlue ? 'b' : 'r',
                    playerColor2char(c->owner),
                    c->countPalms,//TODO: change context
                    c->countPalaces,
                    c->score,
                    c->mipples.size() );
                for (auto it: c->mipples)
                    printf("%c", it->color2char());
                //printf(" %p", c);
                printf("\n");
            }
            fflush(stdout);
        }
        else if (!strcmp(str,"print") || !strcmp(str,"p"))
            print(field);
        else if (!strcmp(str,"palm"))
        {
            char addr[2];
            scanf(" %c%c", &addr[0], &addr[1]);
            int x = addr[0]-'a';
            int y = addr[1]-'1';
            
            ICell* cell=field->cells[y][x];
            cell->countPalms++;
            printf("add palm done\n"); fflush(stdout);
        }
        else if (!strcmp(str,"palace"))
        {
            char addr[2];
            scanf(" %c%c", &addr[0], &addr[1]);
            int x = addr[0]-'a';
            int y = addr[1]-'1';
            
            ICell* cell=field->cells[y][x];
            cell->countPalaces++;
            printf("add palace done\n"); fflush(stdout);
        }
        else if (!strcmp(str,"stat"))
        {
            //for (auto it: contexts)
            char owner;
            scanf(" %c", &owner);
            PlayerColor player = char2PlayerColor(owner);
            contexts[player]->print();
        }
        else if (!strcmp(str,"set"))
        {
            char addr[2];
            char owner;
            scanf(" %c%c %c", &addr[0], &addr[1], &owner);

            int x = addr[0]-'a';
            int y = addr[1]-'1';
            PlayerColor player = char2PlayerColor(owner);
            //contexts[player]->setOwner(x, y);
            ICell* cell=field->cells[y][x];
            cell->owner = char2PlayerColor(owner);
        }
        else if (!strcmp(str,"money"))
        {
            char owner;
            int money;
            scanf(" %c %d", &owner, &money);
            PlayerColor color = char2PlayerColor(owner);
            contexts[color]->money += money;
        }
        else if (!strcmp(str,"play"))
        {
            char addr[2];
            char owner;
            scanf(" %c%c %c", &addr[0], &addr[1], &owner);

            int x = addr[0]-'a';
            int y = addr[1]-'1';

            ICell* cell=field->cells[y][x];
            PlayerColor player = char2PlayerColor(owner);
            Context* context = contexts[player];
            cell->apply(context);
        }
        else if (!strcmp(str,"init") || !strcmp(str,"i"))
        {
            clearField(field);
            printf("input type of cells:\n");
            for (int j=0; j<5; j++)
            {
                char types[10];
                scanf("%s", types);
                for (int i=0; i<6; i++)
                    field->cells[j][i] = getCellFromChar( types[i], 0, false);
            }
            printf("input scores of cells:\n");
            for (int j=0; j<5; j++)
            {
                for (int i=0; i<6; i++)
                {
                    int score;
                    if (i < 5)
                        scanf("%d ", &score);
                    else
                        scanf("%d", &score);
                    field->cells[j][i]->score = score;
                }
                printf("enter line %d\n", j+1);
            }
            printf("input parameter \"is_blue\":\n");
            for (int j=0; j<5; j++)
            {
                char types[10];
                scanf("%s", types);
                for (int i=0; i<6; i++)
                    field->cells[j][i]->isBlue = (types[i]=='b');
            }

            printf("input mipples(count and colors) for example: [3 rwg])\n");
            for (int j=0; j<5; j++)
            {
                for (int i=0; i<6; i++)
                {
                    ICell* cell = field->cells[j][i];
                    printf("%c%c: ", i+'A', j+'1');
                    int count = 3;
                    char mipples[5];
                    //scanf("%d %s", &count, mipples);
                    scanf("%s", mipples);
                    for (int k=0; k<count; k++)
                        cell->addMipple(char2Mipple(mipples[k]));
                }
            }
            // TODO: check count miples
            printf("init done\n");
        }
    }
    for (auto it : contexts)
        delete it.second;
    delete field;
    IMipple::removeAllMiples();
    return 0;
}

void load( Field* field, map<PlayerColor,Context*> &contexts, const char* str )
{
    //printf("enter load\n"); fflush(stdout);
    //printf("enter load %s\n", str); fflush(stdout);
    clearField(field);
    FILE* f = fopen(str, "rt");

    char addr[2];
    char type, color, owner;
    int score, countMiples;
    int countPalms, countPalaces;

    for (int j=0; j<5; j++)
    for (int i=0; i<6; i++)
    {
        if (j==2 && i==4)
            int debug=0;
        //"%c%d %c%c%c %d %d %d %d ";
        fscanf(f, baseStr,
            &addr[0], &addr[1],
            &type, &color, &owner,
            &countPalms, &countPalaces,
            &score, &countMiples );

        int x = addr[0]-'A';
        int y = addr[1]-'1';
        //printf("%c %c  (%d, %d)\n", addr[0], addr[1], x, y);
        ICell *cell = getCellFromChar(type, score, color == 'b');
        cell->countPalms = countPalms;
        cell->countPalaces = countPalaces;
        if (owner != '-')
            cell->owner = char2PlayerColor(owner);
        //printf("cell created\n"); fflush(stdout);
        for (int i=0; i<countMiples; i++)
        {
            char ch;
            fscanf(f, "%c", &ch);
            cell->addMipple(char2Mipple(ch));
        }
        //printf("(%d,%d) count: %ld ", x, y, cell->mipples.size());
        /*printf( baseStr, 
            i+'A', j+1, 
            cell->type,
            cell->isBlue ? 'b' : 'r',
            playerColor2char(cell->owner),
            cell->countPalms,
            cell->countPalaces,
            cell->score,
            cell->mipples.size());
        printf("\n");*/
        field->addCell(y, x, cell);

        //printf("%d %d (%d, %d) ", j, i, y, x ); fflush(stdout);
        //ICell *c = field->cells[j][i];
        //printf("%p\n", c); fflush(stdout);

        char ch;
        if (countMiples)
            fscanf(f, "%c", &ch);
        int z = 0;
    }

    for (auto it: contexts)
        it.second->load(f);
    fclose(f);
    for (int j=0; j<5; j++)
    for (int i=0; i<6; i++)
    {
        if (field->cells[j][i] == nullptr )
            int debug = 0;
    }

    //printf("load done\n"); fflush(stdout);
}

void clearField( Field* field )
{
    for (int j=0; j<5; j++)
    for (int i=0; i<6; i++)
    {
        if (field->cells[j][i])
        {
            delete field->cells[j][i];
            field->cells[j][i] = nullptr;
        }
    }
    IMipple::removeAllMiples();
}

void print( Field* field )
{
    printf(" |");
    for (int i=0; i<6; i++)
        printf("   %c    |", 'A'+i);
    for (int j=0; j<5; j++)
    {
        printf("\n-|");
        for (int i=0; i<6; i++)
            printf("---------");
        printf("\n |");
        for (int i=0; i<6; i++)
        {
            ICell *c = field->cells[j][i];
            printf("%c    %2d |", c->type, c->score );
        }
        printf("\n%d|", j+1);
        for (int i=0; i<6; i++)
        {
            ICell *c = field->cells[j][i];
            printf(" %s  %c |", c->isBlue ? "-B-" : "   ", playerColor2char(c->owner));
        }
        printf("\n |");
        for (int i=0; i<6; i++)
        {
            ICell *c = field->cells[j][i];
            int count = 8;
            //printf("%u ", c->mipples.size()); fflush(stdout);
            if (c->mipples.size()>6) 
                count = 9;
            else
                printf(" ");
            for (auto m: c->mipples)
            {
                count--;
                printf("%c", m->color2char() );
            }
            if ( count > 0 )
            {
                for (int k=1; k<count; k++)
                    printf(" ");
                printf("|");
            }
        }
        printf("\n |");
        for (int i=0; i<6; i++)
        {
            ICell *c = field->cells[j][i];
            printf(" %d    %d |", c->countPalms, c->countPalaces);
        }
        
    }
    printf("\n-|");
    for (int i=0; i<6; i++)
        printf("---------");
    printf("\n");
    fflush(stdout);
}