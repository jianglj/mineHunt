#include "tiledata.h"

#include <stdlib.h> //????????????
#include <QTime>
#include <QTimer>

void tilesPropAppend(QDeclarativeListProperty<TileData>* prop, TileData* value)
{
    Q_UNUSED(prop);  //用于避免编译报错
    Q_UNUSED(value);
    return;  //Append not supported        ?????????????
}

int tilesPropCount(QDeclarativeListProperty<TileData>* prop)
{
    return static_cast<QList<TileData*>*>(prop->data)->count();
}

TileData* tilesPropAt(QDeclarativeListProperty<TileData>* prop, int index)
{
    return static_cast<QList<TileData*>*>(prop->data)->at(index);
}

QDeclarativeListProperty<TileData> MinehuntGame::tiles()  //grid中的model ????????????
{
    return QDeclarativeListProperty<TileData>(this, &_tiles, &tilesPropAppend, &tilesPropCount, &tilesPropAt, 0);
}

MinehuntGame::MinehuntGame()
    :numCols(9),numRows(9), playing(true), won(false)
{
    setObjectName("mainObject");
    srand(QTime(0,0,0).secsTo(QTime::currentTime()));
    //初始化数组
    for(int ii=0; ii<numRows*numCols;++ii)
    {
        _tiles << new TileData;
    }
    reset();
}

void MinehuntGame::setBoard()
{
    foreach(TileData* t, _tiles)  //_tiles初始化，清除地雷，线索数字为-1
    {
        t->setHasMine(false);
        t->setHint(-1);
    } //t的生命周期结束

    //放雷
    int mines = nMines;                     //nMines在哪初始化？  reset函数中。
    remaining = numRows * numCols - mines;  //总格数-总雷数

    while(mines)
    {
        int col = int( (double(rand()) / double(RAND_MAX)) *numCols);
        int row = int( (double(rand()) / double(RAND_MAX)) *numRows);
        TileData* t = tile(row,col);         //获取该位置TileData类实例
        if( t && !t->hasMine() )             //如果t在board上，且还没有设置有雷
        {
            t->setHasMine(true);
            mines--;
        }
    }

    //设置线索数字
    for(int r=0; r<numRows; r++)
    {
        for(int c=0; c<numCols; c++)
        {
            TileData* t = tile(r,c);
            if(t && !t->hasMine())
            {
                int hint = getHint(r,c);
                t->setHint(hint);
            }
        }
    }

    setPlaying(true);  //版面设置完后，开始游戏
}

void MinehuntGame::reset()
{
    foreach(TileData* t, _tiles)
    {
        t->unflip();               //翻回
        t->setHasFlag(false);      //无旗
    }
    nMines = 12;
    nFlags = 0;
    emit numMinesChanged();
    emit numFlagsChanged();
    setPlaying(false);
    QTimer::singleShot(600,this,SLOT(setBoard())); //之后重设版面
}

int MinehuntGame::getHint(int row, int col)
{
    int hint = 0;
    for(int c=col-1; c<=col+1; c++)
        for(int r=row-1; r<=row+1; r++)
        {
            TileData* t= tile(r,c);
            if(t && t->hasMine())
                hint++;
        }
    return hint;
}

bool MinehuntGame::flip(int row, int col)
{
    if(!playing)              //没有在玩 返回false
        return false;

    TileData *t = tile(row, col);
    if(!t || t->hasFlag())    //不在board上 或者 有旗子 返回false
        return false;

    if(t->flipped())          //如果是翻开的，再点，显示周围  当flag数与hint相等，其他翻开
    {
        int flags = 0;
        for(int c=col-1; c<=col+1; c++) //for循环用来统计总flag数
            for(int r=row-1; r<=row+1; r++)
            {
                TileData *nearT = tile(r, c);
                if(!nearT || nearT == t) //九个中包含了自己，因此肯定不是旗子，跳过
                    continue;
                if(nearT->hasFlag())
                    flags++;
            }
        if(!t->hint() || t->hint()!=flags) //hint为0 或 hint不等于周围flag数
            return false;

        for(int c=col-1; c<=col+1; c++)   //hint==flags，翻开未翻开的
            for(int r=row-1; r<=row+1; r++)
            {
                TileData *nearT = tile(r, c);
                if(nearT && !nearT->flipped() && !nearT->hasFlag()) //确定在board上，未翻开、没有旗子
                    flip(r,c);
            }
        return true;
    }
    //还剩一种情况，未翻开。则翻开。 分为有雷和无雷。 若hint==0，翻开周围
    t->flip();  //这里的flip是TileData类的flip函数
    if(t->hint() == 0)
    {
        for(int c=col-1; c<=col+1; c++)
            for(int r=row-1; r<=row+1; r++)
            {
                TileData *t = tile(r,c);
                if(t && !t->flipped())//在board上且未翻开，以免重复
                    flip(r,c);
            }
    }

    if(t->hasMine()) //如果翻开炸弹，显示所有炸弹
    {
        for(int r=0; r<numRows; r++)
            for(int c=0; c<numCols; c++)
            {
                TileData *t = tile(r,c);
                if(t && t->hasMine())
                    t->flip();          //??????????????????????原句： flip(r,c);
            }
        won = false;
        hasWonChanged();
        setPlaying(false);
        return true;
    }

    //翻开后不是0也不是雷的
    remaining--;
    if(!remaining) //没有剩下未翻的
    {
        won = true;
        hasWonChanged();
        setPlaying(false);
        return true;  //那这句根本就不需要啊....- -#
    }
    return true; //翻开不是0也不是雷，返回true
}

bool MinehuntGame::flag(int row, int col) //插旗子 右键触发
{
    TileData* t = tile(row, col);
    if(!t || !playing || t->flipped()) //不在board 或 不在玩 或 已翻开  则返回false
        return false;

    t->setHasFlag(!t->hasFlag()); //插旗标志取反
    nFlags += (t->hasFlag()? 1:-1); //现在是旗，加1； 不是了，减1
    emit numFlagsChanged();
    return true;
}
