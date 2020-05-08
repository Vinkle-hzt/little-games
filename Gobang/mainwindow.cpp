#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QCoreApplication>
#include <QMessageBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QTimer>

#define cout qDebug()

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qsrand(time(0));

    this->setWindowFlag(Qt::FramelessWindowHint);//去边框

    start = QPoint(14,14);
    end = QPoint(585,583);
    gridW = (end.x() - start.x())/18.0;
    gridH = (end.y() - start.y())/18.0;

    initChessboard();

    ui->radioButtonFirst->setChecked(1);

    //计时
    connect(&leftTimer, &QTimer::timeout,
            [=]()mutable
    {
        if(gameStatus)
        {
            --timeNum;
            ui->lcdnumberTime->display(timeNum);
            //时间为0改变角色
            if(timeNum == 0)
            {
                timeNum = 30;
                changeRole();
            }
        }
    }
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initChessboard()
{
    for (int i = 0; i < 19; ++i)
    {
        for(int j = 0; j < 19; ++j)
        {
            chess[i][j] = Empty;
        }
    }
    lastPosition = qMakePair(-1,-1);

    timeNum = 30;
    ui->lcdnumberTime->display(timeNum);

    if(leftTimer.isActive() == false)
    {
        this -> leftTimer.start(1000);
    }
}

void MainWindow::changeRole()
{
    if(gameStatus)
    {
        if(role == Black)
        {
            role = White;
            ui->labelBalck->hide();
            ui->labelWhite->show();
        }
        else
        {
            role = Black;
            ui->labelBalck->show();
            ui->labelWhite->hide();
        }
        timeNum = 30;

        //如果是人机模式,机器下棋
        if(playWithComputer && role == AIRole)
        {
            AIChess();
            timeNum = 30;
        }
    }
}

bool MainWindow::isFull()
{
    for(int i = 0; i <= 18; ++i)
    {
        for(int j = 0; j <= 18; ++j)
            if(chess[i][j]==Empty) return false;
    }
    return true;
}

bool MainWindow::judgeWin(const int& i, const int& j)
{
    int count = 1;

    //统计水平方向个数
    for(int k = 1; k <= 4; k++)
    {
        if(inChessboard(i-k,j))
        {
            if(chess[i-k][j]==role)
            {
                ++count;
            }
            else break;
        }
    }
    for(int k = 1; k <= 4; k++)
    {
        if(inChessboard(i+k,j))
        {
            if(chess[i+k][j]==role)
            {
                ++count;
            }
            else break;
        }
    }
    if(count >= 5) return true;
    count = 1;

    //统计竖直方向个数
    for(int k = 1; k <= 4; k++)
    {
        if(inChessboard(i,j-k))
        {
            if(chess[i][j-k]==role)
            {
                ++count;
            }
            else break;
        }
    }
    for(int k = 1; k <= 4; k++)
    {
        if(inChessboard(i,j+k))
        {
            if(chess[i][j+k]==role)
            {
                ++count;
            }
            else break;
        }
    }
    if(count >= 5) return true;
    count = 1;

    //统计左上右下斜方向个数
    for(int k = 1; k <= 4; k++)
    {
        if(inChessboard(i-k,j-k))
        {
            if(chess[i-k][j-k]==role)
            {
                ++count;
            }
            else break;
        }
    }
    for(int k = 1; k <= 4; k++)
    {
        if(inChessboard(i+k,j+k))
        {
            if(chess[i+k][j+k]==role)
            {
                ++count;
            }
            else break;
        }
    }
    if(count >= 5) return true;
    count = 1;

    //统计右上左下斜方向个数
    for(int k = 1; k <= 4; k++)
    {
        if(inChessboard(i-k,j+k))
        {
            if(chess[i-k][j+k]==role)
            {
                ++count;
            }
            else break;
        }
    }
    for(int k = 1; k <= 4; k++)
    {
        if(inChessboard(i+k,j-k))
        {
            if(chess[i+k][j-k]==role)
            {
                ++count;
            }
            else break;
        }
    }
    if(count >= 5) return true;
    return false;
}

void MainWindow::win(const int& i, const int& j)
{
    if(judgeWin(i,j))
    {
        gameStatus = false;
        if(role==Black)
            QMessageBox::information(this,tr("Over"),QStringLiteral("游戏结束，黑方胜利！"),QMessageBox::Ok);
        else
            QMessageBox::information(this,tr("Over"),QStringLiteral("游戏结束，白方胜利！"),QMessageBox::Ok);
    }
}

void MainWindow::gamerChess(const int& i,const int& j)
{
    if(gameStatus && inChessboard(i,j))
    {
        if(chess[i][j] == Empty)
        {
            chess[i][j] = role;
            lastPosition = qMakePair(i,j);
            win(i,j);
            changeRole();
            update();
        }
    }
}

void MainWindow::AIChess()
{
    //瞎写的，主要用来测试功能行不行
    while(true)
    {
        int n = qrand() % (19*19);
        int i = n / 19, j = n % 19;
        if(chess[i][j] == Empty)
        {
            chess[i][j] = AIRole;
            lastPosition = qMakePair(i,j);
            win(i,j);
            break;
        }
    }
    update();
    changeRole();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0,0,800,600,QPixmap(":/image/front.png"));
    p.drawPixmap(0,0,592,595, QPixmap(":/image/main.png"));

    //绘制棋盘
    for (int i = 0; i < 19; ++i)
    {
        for(int j = 0; j < 19; ++j)
        {
            if(chess[i][j] == Black)
            {
                p.drawPixmap(start.x()+ (i-0.5)*gridW, start.y()+ (j-0.5)*gridH, gridW-2, gridH-2, QPixmap(":/image/black.png"));
            }
            else if(chess[i][j] == White)
            {
                p.drawPixmap(start.x()+ (i-0.5)*gridW, start.y()+ (j-0.5)*gridH, gridW-2, gridH-2, QPixmap(":/image/white.png"));
            }
        }
    }

    //绘制上一位置提醒
    if(lastPosition!=qMakePair(-1,-1))
    {
        int i = lastPosition.first;
        int j = lastPosition.second;
        p.drawPixmap(start.x()+ (i-0.5)*gridW, start.y()+ (j-0.5)*gridH, gridW-2, gridH-2, QPixmap(":/image/last.png"));
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
    {
        this->p = e->globalPos() - this->frameGeometry().topLeft();
    }
    //存储鼠标按下的坐标
    int i = qRound((e->x() - start.x()) / gridW);
    int j = qRound((e->y() - start.y()) / gridH);

    if(!playWithComputer)
    {
        gamerChess(i,j);
    }
    else
    {
        if(role == gamerRole)
        {
            gamerChess(i,j);
        }
    }

    if(isFull())
    {
        gameStatus = false;
        QMessageBox::information(this,tr("Over"),QStringLiteral("棋盘已满，本局游戏和局"),QMessageBox::Ok);
    }

}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    //如果按住的位置不是棋盘位置，而且按住的是左键，需要移动窗口
    if(e->pos().x()>600)
    {
        if(e->buttons() & Qt::LeftButton)
        {
            move(e->globalPos()-this->p);
        }
    }
}

void MainWindow::on_toolButtonExit_clicked()
{
    this->close();
}

void MainWindow::on_toolButtonGamer_clicked()
{
    initChessboard();
    gameStatus = true;
    playWithComputer = false;
    role = Black;
    ui->labelBalck->show();
    ui->labelWhite->hide();
    update();
}

void MainWindow::on_toolButtonCompuer_clicked()
{
    initChessboard();
    gameStatus = true;
    playWithComputer = true;
    if(ui->radioButtonFirst->isChecked())
    {
        AIRole = Black;
        gamerRole = White;
        chess[9][9] = Black;
        lastPosition = qMakePair(9,9);
        role = gamerRole;
    }
    else
    {
        gamerRole = Black;
        AIRole = White;
        role = gamerRole;
    }
    update();
}
