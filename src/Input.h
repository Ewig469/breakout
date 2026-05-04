#pragma once

class Input {
public:
    int  P1MoveDir() const;
    bool P1Action() const;
    bool P1Fire() const;
    int  P2MoveDir() const;
    bool P2Action() const;
    bool P2Fire() const;
    bool UpPressed() const;
    bool DownPressed() const;
};
