#ifndef FORME_h
#define FORME_h
#define _CRT_SECURE_NO_WARNINGS

enum Tetrominoes {
    NoForme, FormeI, FormeO, FormeT, FormeL, FormeJ, FormeZ, FormeS
};

class Forme
{
public:
    Forme() { SetForme(NoForme); }
    void SetForme(Tetrominoes Forme);
    void SetRandomForme();

    Tetrominoes GetForme() const { return pieceForme; }
    int x(int index) const { return coords[index][0]; }
    int y(int index) const { return coords[index][1]; }

    int MinX() const;
    int MaxX() const;
    int MinY() const;
    int MaxY() const;

    Forme RotateLeft() const;
    Forme RotateRight() const;

private:
    void SetX(int index, int x) { coords[index][0] = x; }
    void SetY(int index, int y) { coords[index][1] = y; }
    Tetrominoes pieceForme;
    int coords[4][2];
};

#endif