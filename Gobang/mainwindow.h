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
    enum ChessStatus{Empty, Black, White};
    void initChess();//初始化棋盘
    void changeRole();
    void judge(int i,int j);
protected:
    void paintEvent(QPaintEvent *); //绘图事件
    void mousePressEvent(QMouseEvent *); //鼠标按下事件
    void mouseMoveEvent(QMouseEvent *); //鼠标移动事件

private slots:
    void on_toolButton_2_clicked();

    void on_toolButtonBegin_clicked();

private:
    Ui::MainWindow *ui;
    QPoint p; //坐标的差值
    //起点坐标，终点坐标
    QPoint start, end;
    //格子宽度，高度
    double gridW, gridH;
    //黑白子的位置
    int chess[19][19];
    ChessStatus role = Black;
    bool gameStatus = false;

    QTimer leftTimer;
    int timeNum;
};
#endif // MAINWINDOW_H
