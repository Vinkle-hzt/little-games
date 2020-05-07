#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QCoreApplication>
#include <QMessageBox>
#include <QTimer>

#define cout qDebug()

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //去边框
    this->setWindowFlag(Qt::FramelessWindowHint);

    start = QPoint(14,14);
    end = QPoint(585,583);
    gridW = (end.x() - start.x())/18.0;
    gridH = (end.y() - start.y())/18.0;

    initChess();
    connect(&leftTimer, &QTimer::timeout,
    [=]()mutable
    {
        if(gameStatus)
        {
            --timeNum;
            ui->lcdnumberTime->display(timeNum);

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

void MainWindow::initChess()
{
    for (int i = 0; i < 19; ++i)
    {
        for(int j = 0; j < 19; ++j)
        {
            chess[i][j] = Empty;
        }
    }

    timeNum = 30;
    ui->lcdnumberTime->display(timeNum);

    if(leftTimer.isActive() == false)
    {
        this -> leftTimer.start(1000);
    }
}

void MainWindow::changeRole()
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
}

void MainWindow::judge(int i, int j)
{
    int count = 1;

    //统计水平方向个数
    for(int k = 1; k <= 4; k++)
    {
        if(i - k >= 0 && i - k <= 18)
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
        if(i + k >= 0 && i + k <= 18)
        {
            if(chess[i+k][j]==role)
            {
                ++count;
            }
            else break;
        }
    }
    if(count >= 5)
    {
        if(role==Black)
            QMessageBox::information(this,tr("Over"),QStringLiteral("游戏结束，黑方胜利！"),QMessageBox::Ok);
        else
            QMessageBox::information(this,tr("Over"),QStringLiteral("游戏结束，白方胜利！"),QMessageBox::Ok);
        gameStatus = false;
    }
    count = 1;

    //统计竖直方向个数
    for(int k = 1; k <= 4; k++)
    {
        if(j - k >= 0 && j - k <= 18)
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
        if(j + k >= 0 && j + k <= 18)
        {
            if(chess[i][j+k]==role)
            {
                ++count;
            }
            else break;
        }
    }
    if(count >= 5)
    {
        if(role==Black)
            QMessageBox::information(this,tr("Over"),QStringLiteral("游戏结束，黑方胜利！"),QMessageBox::Ok);
        else
            QMessageBox::information(this,tr("Over"),QStringLiteral("游戏结束，白方胜利！"),QMessageBox::Ok);
        gameStatus = false;
    }
    count = 1;

    //统计斜方向个数
    for(int k = 1; k <= 4; k++)
    {
        if(i - k >= 0 && i - k <= 18 && j - k >= 0 && j - k <= 18)
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
        if(i + k >= 0 && i + k <= 18 && j + k >= 0 && j + k <= 18)
        {
            if(chess[i+k][j+k]==role)
            {
                ++count;
            }
            else break;
        }
    }
    if(count >= 5)
    {
        if(role==Black)
            QMessageBox::information(this,tr("Over"),QStringLiteral("游戏结束，黑方胜利！"),QMessageBox::Ok);
        else
            QMessageBox::information(this,tr("Over"),QStringLiteral("游戏结束，白方胜利！"),QMessageBox::Ok);
        gameStatus = false;
    }
    count = 1;

    //统计斜方向个数
    for(int k = 1; k <= 4; k++)
    {
        if(i - k >= 0 && i - k <= 18 && j + k >= 0 && j + k <= 18)
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
        if(i + k >= 0 && i + k <= 18 && j - k >= 0 && j - k <= 18)
        {
            if(chess[i+k][j-k]==role)
            {
                ++count;
            }
            else break;
        }
    }
    if(count >= 5)
    {
        if(role==Black)
            QMessageBox::information(this,tr("Over"),QStringLiteral("游戏结束，黑方胜利！"),QMessageBox::Ok);
        else
            QMessageBox::information(this,tr("Over"),QStringLiteral("游戏结束，白方胜利！"),QMessageBox::Ok);
        gameStatus = false;
    }
    count = 1;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0,0,800,600,QPixmap(":/image/front.png"));
    p.drawPixmap(0,0,592,595, QPixmap(":/image/main.png"));

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
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
    {
        this->p = e->globalPos() - this->frameGeometry().topLeft();
    }

    int i = 0;
    int j = 0;
    i = qRound((e->x() - start.x()) / gridW);
    j = qRound((e->y() - start.y()) / gridH);
    if(gameStatus && i >= 0 && i <= 18 && j >= 0 && j <= 18)
    {
        if(chess[i][j] == Empty)
        {
            chess[i][j] = role;
            judge(i,j);
            changeRole();
            update();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    //如果按住的是左键，需要移动窗口
    if(e->pos().x()>600)
    {
        if(e->buttons() & Qt::LeftButton)
        {
            if(e->buttons() & Qt::LeftButton)
            {
                move(e->globalPos()-this->p);
            }
        }
    }
}

void MainWindow::on_toolButton_2_clicked()
{
    this->close();
}

void MainWindow::on_toolButtonBegin_clicked()
{
    initChess();
    gameStatus = true;
    role = Black;
    ui->labelBalck->show();
    ui->labelWhite->hide();
    update();
}
