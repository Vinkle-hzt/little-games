#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    enum ChessStatus{Empty, Black, White};//记录状态
    void initChessboard();//初始化棋盘
    void changeRole();//改变角色
    bool inChessboard(const int i,const int j)//判断是否在棋盘上
    {
        if(i >= 0 && i < GridSIZE && j >= 0 && j < GridSIZE)  return true;
        return false;
    }
    bool isFull();
    bool judgeWin(const int i,const int j);//判断胜利
    void win(const int i,const int j);//输出胜利
    void gamerChess(const int i,const int j);
    void AIChess();
protected:
    void paintEvent(QPaintEvent *); //绘图事件
    void mousePressEvent(QMouseEvent *); //鼠标按下事件
    void mouseMoveEvent(QMouseEvent *); //鼠标移动事件
private slots:
    void on_toolButtonExit_clicked();

    void on_toolButtonGamer_clicked();


    void on_toolButtonCompuer_clicked();

private:
    const static int GridSIZE = 19;
    Ui::MainWindow *ui;
    QPoint p; //坐标的差值
    QPoint start, end;//起点坐标，终点坐标
    double gridW, gridH;//格子宽度，高度
    ChessStatus chess[GridSIZE][GridSIZE];//黑白子的位置
    ChessStatus role = Black;//当前的游戏角色
    bool gameStatus = false;//游戏的状态
    ChessStatus gamerRole;//玩家游戏角色
    ChessStatus AIRole;//电脑游戏角色
    QTimer leftTimer;//计时器
    int timeNum;//剩余时间
    QPair<int,int> lastPosition;//记录上一位置
    QPair<int,int> wantPosition;
    bool playWithComputer = false;
    bool rb_first, rb_second;
};

#endif // MAINWINDOW_H
