#pragma once

enum PlayerColor {
    None = 0, 
    Fekete, 
    Orange, 
    Blue, 
    Purple
};

char playerColor2char(PlayerColor c)
{
    switch (c)
    {
        case PlayerColor::None: return '-';
        case PlayerColor::Purple: return 'p';
        case PlayerColor::Fekete: return 'f';
        case PlayerColor::Blue: return 'b';
        case PlayerColor::Orange: return 'o';
    }
    return 0;
}

PlayerColor char2PlayerColor(char ch)
{
    switch (ch)
    {
        case '-' : return PlayerColor::None;
        case 'p' : return PlayerColor::Purple;
        case 'f' : return PlayerColor::Fekete;
        case 'b' : return PlayerColor::Blue;
        case 'o' : return PlayerColor::Orange;
    }
    return PlayerColor::None;
}

// TODO: make like a claas, add GoldMiples to it
/*
class Player
{
public:
    enum Color {
        None = 0, 
        Black, 
        Orange, 
        Blue, 
        Purple
    };
    Color color;
    Player(const Color& c ) : color(c) {}
    //Player
};*/