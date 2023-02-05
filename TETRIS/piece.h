#pragma once

enum class FormePiece {
    Init, FormeI, FormeO, FormeT, FormeL, FormeJ,
    FormeZ, FormeS
};
class Piece
{
public:
    Piece() { SetForme(FormePiece::Init); }
    void SetForme(FormePiece forme);
    void SetFormeRandom(FormePiece forme);

    FormePiece GetForme() const { return forme; }
    int GetPosX() const { return Pos[0][0]; }
    int GetPosY() const { return Pos[0][1]; }
    int GetCarrePosX(int idx) const { return Pos[idx][0]; }
    int GetCarrePosY(int idx) const { return Pos[idx][1]; }

    int GetMaxX() const;
    int GetMinX() const;
    int GetMaxY() const;
    int GetMinY() const;

    Piece RotationDroite() const;
    Piece RotationGauche() const;



private:
    FormePiece forme;
    int Pos[4][2];

    void setPosCarreX(int idx, int x) { Pos[idx][0] = x; }
    void setPosCarreY(int idx, int y) { Pos[idx][1] = y; }



};

