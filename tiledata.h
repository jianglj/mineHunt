#ifndef TILEDATA_H
#define TILEDATA_H

#include <QObject>
#include <qdeclarative.h>

class TileData : public QObject
{
    Q_OBJECT
public:
    //explicit TileData(QObject *parent = 0);
    TileData() : _hasFlag(false), _hasMine(false), _hint(-1), _flipped(false){}  //???????????

    Q_PROPERTY(bool hasFlag READ hasFlag WRITE setHasFlag NOTIFY hasFlagChanged)
    bool hasFlag() const { return _hasFlag; }

    Q_PROPERTY(bool hasMine READ hasMine WRITE setHasMine NOTIFY hasMineChanged)
    bool hasMine() const { return _hasMine; }

    Q_PROPERTY(int hint READ hint NOTIFY hintChanged)
    int hint() const { return _hint; }

    Q_PROPERTY(bool flipped READ flipped NOTIFY flippedChanged())
    bool flipped() const { return _flipped; }

    void setHasFlag(bool flag) { if(flag == _hasFlag) return; _hasFlag = flag; emit hasFlagChanged(); }
    void setHasMine(bool mine) { if(mine == _hasMine) return; _hasMine = mine; emit hasMineChanged(); }
    void setHint(int hint)     { if(hint == _hint) return; _hint = hint; emit hintChanged(); }
    void flip()                { if(_flipped) return; _flipped = true; emit flippedChanged(); }
    void unflip()              { if(!_flipped) return; _flipped = false; emit flippedChanged(); }

signals:
    void hasFlagChanged();
    void hasMineChanged();
    void hintChanged();
    void flippedChanged();

public slots:
private:
    bool _hasFlag;
    bool _hasMine;
    int _hint;
    bool _flipped;
};

class MinehuntGame : public QObject
{
    Q_OBJECT
public:
    MinehuntGame();

    Q_PROPERTY(QDeclarativeListProperty<TileData> tiles READ tiles CONSTANT) //???????????
    QDeclarativeListProperty<TileData> tiles();

    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)
    bool isPlaying() { return playing; }

    Q_PROPERTY(bool hasWon READ hasWon NOTIFY hasWonChanged)
    bool hasWon() { return won; }

    Q_PROPERTY(int numMines READ numMines NOTIFY numMinesChanged)
    int numMines() const{ return nMines; }

    Q_PROPERTY(int numFlags READ numFlags NOTIFY numFlagsChanged)
    int numFlags() const{ return nFlags; }

public slots:
    Q_INVOKABLE bool flip(int row, int col);   //使用Q_INVOKABLE来修饰成员函数，目的在于被修饰的成员函数能够被元对象系统所唤起
    Q_INVOKABLE bool flag(int row, int col);
    void setBoard();
    void reset();

signals:
    void isPlayingChanged();
    void hasWonChanged();
    void numMinesChanged();
    void numFlagsChanged();

private:
    bool onBoard(int r,int c) const{ return r>=0 && r<numRows && c>=0 && c<numCols; }
    TileData *tile(int row, int col) { return onBoard(row,col) ? _tiles[col+numRows*row] : 0 ; }  //返回值为指针
    int getHint(int row, int col);
    void setPlaying(bool b) {if(b == playing) return; playing = b; emit isPlayingChanged();}

    QList<TileData *> _tiles;
    int numCols; //总行列数，在哪初始化？？？？？？？？？？？
    int numRows;
    bool playing;
    bool won;
    int remaining;  //????????????????
    int nMines;
    int nFlags;
};

#endif // TILEDATA_H
