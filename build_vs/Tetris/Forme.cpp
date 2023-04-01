#include <stdlib.h>
#include <algorithm>
#include "Forme.h"

using namespace std;

void Forme::SetForme(Tetrominoes Forme)
{
    static const int coordsTable[8][4][2] = {
        { { 0, 0 },   { 0, 0 },   { 0, 0 },   { 0, 0 } },// NoForme
        { { 0, -1 },  { 0, 0 },   { 0, 1 },   { 0, 2 } },//FormeI
        { { 0, 0 },   { 1, 0 },   { 0, 1 },   { 1, 1 } },//FormeO
        { { -1, 0 },  { 0, 0 },   { 1, 0 },   { 0, 1 } },//FormeT
        { { -1, -1 }, { 0, -1 },  { 0, 0 },   { 0, 1 } },//FormeL
        { { 1, -1 },  { 0, -1 },  { 0, 0 },   { 0, 1 } },//FormeJ
         { { 0, -1 },  { 0, 0 },   { -1, 0 },  { -1, 1 } },//FormeZ
         { { 0, -1 },  { 0, 0 },   { 1, 0 },   { 1, 1 } }//FormeS
  };

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; ++j)
            coords[i][j] = coordsTable[Forme][i][j];
    }
    pieceForme = Forme;
}


void Forme::SetRandomForme()
{
    int x = rand() % 7 + 1;
    SetForme(Tetrominoes(x));
}

int Forme::MinX() const
{
    int m = coords[0][0];
    for (int i = 0; i < 4; i++) {
        m = min(m, coords[i][0]);
    }
    return m;
}

int Forme::MaxX() const
{
    int m = coords[0][0];
    for (int i = 0; i < 4; i++) {
        m = max(m, coords[i][0]);
    }
    return m;
}

int Forme::MinY() const
{
    int m = coords[0][1];
    for (int i = 0; i < 4; i++) {
        m = min(m, coords[i][1]);
    }
    return m;
}

int Forme::MaxY() const
{
    int m = coords[0][1];
    for (int i = 0; i < 4; i++) {
        m = max(m, coords[i][1]);
    }
    return m;
}

Forme Forme::RotateLeft() const
{
    if (pieceForme == FormeO)
        return *this;

    Forme result;
    result.pieceForme = pieceForme;
    for (int i = 0; i < 4; ++i) {
        result.SetX(i, y(i));
        result.SetY(i, -x(i));
    }
    return result;
}

Forme Forme::RotateRight() const
{
    if (pieceForme == FormeO)
        return *this;

    Forme result;
    result.pieceForme = pieceForme;
    for (int i = 0; i < 4; ++i) {
        result.SetX(i, -y(i));
        result.SetY(i, x(i));
    }
    return result;
}