#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QtDebug>
#include "QColorDialog"
#include <QTime>
#include <iostream>
QImage img(500,500,QImage::Format_RGB888);
static QColor color;
static int LEFT=1;
static int RIGHT=2;
static int BOTTOM=4;
static int TOP=8;
static int xl = 100;
static int yl = 100;
static int xh = 250;
static int yh = 250;
int ver=0,temp,i,j,a[20],b[20];
float m[20],dx,dy,x[20];
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::dda(int x1,int y1,int x2,int y2)
{
    int dx,dy,p;
       int x,y;
       int i=0;
       x=x1;
       y=y1;
        dx=abs(x2-x1);
        dy=abs(y2-y1);
        if(dx>=dy)
        {
          p=2*dy-dx;
          while(i<=dx)
          {img.setPixel(x,y,color.rgb());
              if(p<0)
              {
                  p=p+2*dy;
              }
              else{
                  p=p+2*(dy-dx);
                  y=y+sign(y2-y1);
              }
              x=x+sign(x2-x1);
              i++;
          }
            }
        else{

            p=2*dx-dy;
            while(i<=dy)
            {img.setPixel(x,y,color.rgb());
                if(p<0)
                {
                    p=p+2*dx;
                }
                else{
                    p=p+2*(dx-dy);
                    x=x+sign(x2-x1);
                }
                y=y+sign(y2-y1);
                i++;
            }
        }
        ui->label->setPixmap(QPixmap::fromImage(img));
    }
int MainWindow::sign(float x){
    if(x<0)
        return -1;
    else
        return 1;
}
void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    if(start)
    {
        int p=ev->pos().x();
        int q=ev->pos().y();
        a[ver]=p;
        b[ver]=q;
        if(ev->button()==Qt::RightButton)
        {
            dda(a[0],b[0],a[ver-1],b[ver-1]);

        }
        else
        {
            if(ver%2 !=0)
            {
                dda(a[ver],b[ver],a[ver-1],b[ver-1]);
            }
        }
        ver++;
    }
}


void MainWindow::on_pushButton_clicked()
{
    color=QColorDialog::getColor();
}



void MainWindow::on_pushButton_2_clicked()
{
    dda(xl,yl,xh,yl);
    dda(xh,yl,xh,yh);
    dda(xh,yh,xl,yh);
    dda(xl,yh,xl,yl);
}

int MainWindow::region(int x, int y){
    int code = 0;

    if(y > yh) code |=TOP;
    if(y < yl) code |=BOTTOM;
    if(x < xl) code |=LEFT;
    if(x > xh) code |=RIGHT;
    return code;
}




void MainWindow::on_pushButton_3_clicked()
{
    for (int i=0; i<ver ;i= i+2 ) {
           int x1 = a[i];  int y1 = b[i];
           int x2 = a[i+1]; int y2 = b[i+1];
           int outcode1=region(x1,y1), outcode2=region(x2,y2);
               int accept = 0;


               while(1){
                       float m =(float)(y2-y1)/(x2-x1);

                       if(outcode1==0 && outcode2==0){
                           accept = 1;
                           break;
                       }

                       else if((outcode1 & outcode2)!=0){
                           break;
                       }else{
                           int x,y;
                           int temp;

                           if(outcode1==0)
                               temp = outcode2;
                           else
                               temp = outcode1;

                           if(temp & TOP){
                               x = x1+ (yh-y1)/m;
                               y = yh;
                           }
                           else if(temp & BOTTOM){
                               x = x1+ (yl-y1)/m;
                               y = yl;
                           }else if(temp & LEFT){
                               x = xl;
                               y = y1+ m*(xl-x1);
                           }else if(temp & RIGHT){
                               x = xh;
                               y = y1+ m*(xh-x1);
                           }

                           if(temp == outcode1){
                               x1 = x;
                               y1 = y;
                               outcode1 = region(x1,y1);
                           }else{
                               x2 = x;
                               y2 = y;
                               outcode2 = region(x2,y2);
                           }
                       }
               }

 if(accept){
    img.setPixel(i,j,qRgb(0,0,0));
    ui->label->setPixmap(QPixmap::fromImage(img));
    dda(x1,y1,x2,y2);
   }

       }
       int i =0;
       while ( i<500) {
           int j = 0;
           while (j<500) {
               if((i<50) or (i>250)){
                   img.setPixel(i,j,qRgb(0,0,0));
                   ui->label->setPixmap(QPixmap::fromImage(img));
               }
               if((j<50) or (j>250)){
       img.setPixel(i,j,qRgb(0,0,0));
       ui->label->setPixmap(QPixmap::fromImage(img));
       }
               j++;

           }
           i++;
       }


}
