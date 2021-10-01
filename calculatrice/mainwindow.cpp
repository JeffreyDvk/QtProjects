#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QButtonGroup>
#include <QStringList>
#include <QDesktopWidget>
#include <QMessageBox>
#include <cmath>
#include <QString>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    move(qApp->desktop()->availableGeometry().center().x() - this->width()/2,qApp->desktop()->availableGeometry().center().y() - this->height()/2 -50);
    QStringList *data = new QStringList;
    eq.append("Calcules récents :");
    ui->dataSeen->setText(eq.join("<br>"));


    QButtonGroup *btngrp = new QButtonGroup(this);
    btngrp->addButton(ui->btn_0);
    btngrp->addButton(ui->btn_1);
    btngrp->addButton(ui->btn_2);
    btngrp->addButton(ui->btn_3);
    btngrp->addButton(ui->btn_4);
    btngrp->addButton(ui->btn_5);
    btngrp->addButton(ui->btn_6);
    btngrp->addButton(ui->btn_7);
    btngrp->addButton(ui->btn_8);
    btngrp->addButton(ui->btn_9);
    btngrp->addButton(ui->btn_vir);
    btngrp->addButton(ui->btn_mul);
    btngrp->addButton(ui->btn_div);
    btngrp->addButton(ui->btn_plus);
    btngrp->addButton(ui->btn_sous);
    btngrp->addButton(ui->btn_mod);
    btngrp->addButton(ui->btn_egal);
    btngrp->addButton(ui->btn_ans);
    btngrp->addButton(ui->btn_ce);
    btngrp->addButton(ui->btn_clear);
    btngrp->addButton(ui->btn_pi);
    btngrp->addButton(ui->btn_exp);
    btngrp->addButton(ui->btn_pow2);
    btngrp->addButton(ui->btn_sqrt);
    connect(btngrp,static_cast<void(QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked),[=](QAbstractButton *btn)mutable{
        QString content = ui->screen->text();
        QString print = content;
        QString error = "";
        QString newtext = print.replace(0,data->join("").size(),"");
        QString btnText = btn->text();
        print = "";


        if(btnText=="CE"){
            if(content=="Operation Impossible" or content=="Division par 0 impossible" or content=="L'opération modulo ne s'effectue qu'entre deux entier"){
                content = data->join("");
            }
            else if(data->size()==1 and content.size()==1){
                data->clear();
                content = "";
            }
            else if(data->isEmpty() and newtext!=""){
                content.remove(content.size()-1,1);
            }
            else if(newtext=="" and !data->isEmpty()){
                if(!QString("123456789").contains(data->at(data->size()-1))){
                    data->pop_back();
                    content = data->join("");
                }
                else {
                    data->pop_back();
                    content=data->join("");
                }
            }
            else{
                content = data->join("") + newtext.remove(newtext.size()-1,1);
            }
            print = content;
        }
        else if(btnText=="ANS"){
            print = content + ans;
        }
        else if(btnText=="ERASE"){
            data->clear();
        }
        else if(btnText=="-"){
            if(data->isEmpty() and content.isEmpty()){ print = "-"; }
            else if(!data->isEmpty()){
                 if(newtext.isEmpty()){
                     if(data->at(data->size()-1)=="*" or data->at(data->size()-1)=="/" or data->at(data->size()-1)=="%" or data->at(data->size()-1)=="+"){
                         print = data->join("") + "-";
                     }
                     else{
                         data->append(btnText);
                         print = data->join("");
                     }
                 }
                 else{
                     data->append(newtext);
                     data->append(btnText);
                     print = data->join("");
                 }
            }
            else{
                data->append(newtext);
                data->append(btnText);
                print = data->join("");
            }
        }
        else if(btnText=="√"){
            if(newtext!=""){
                data->append(newtext);
            }
            data->append("√");
            print = data->join("");
        }
        else if(btnText=="+" or btnText=="*" or btnText=="/" or btnText=="%" or btnText=="x²"){
            btnText = btnText=="x²" ? "²" : btnText;
            if(!content.isEmpty() or !data->isEmpty()){
                if(!newtext.isEmpty()){data->append(newtext);}
                data->append(btnText);
                print = data->join("");
            }
        }
        else if(btnText=="." and (content.contains(".") or content.isEmpty())){
            print = data->join("") + newtext;
        }
        else if(btnText=="="){
            std::cout << "yo";
            if(newtext!=""){data->append(newtext);}
            QString last = data->back();
            QRegExp rx("*[+/%*]");
            int rp = 1;
            rx.setPatternSyntax(QRegExp::Wildcard);
            if(!last.contains(rx) or !last.endsWith("-")){
                QString exp = data->join("") + " = ";
                bool begin = true;
                while (begin and error=="") {
                    begin = false;
                    double val = 0;
                        //std::cout << "yep" << std::endl;
                    if(rp==1){
                        for(int i=0;i < data->size();i++){
                            QString end = data->at(i).back();
                            if(end=="π" or end=="e"){
                                QString start = data->at(i).front();
                                if(start=="π" or start=="e"){
                                    data->replace(i,start=="π" ? "3.14159265358979323846" : QString::number(std::exp(1)));
                                }
                                else{
                                    data->replace(i,end=="π" ? data->at(i).split("π").at(0) : data->at(i).split("e").at(0));
                                    if(!data->size()-1==0){
                                        QStringList cpy = QStringList(data->mid(0,1));
                                        *data = QStringList(data->mid(0,i));
                                        data->append("*");
                                        data->append(end=="π" ? "3.14159265358979323846" : QString::number(std::exp(1)));
                                        data->append(cpy);
                                    }
                                    else{
                                        data->append("*");
                                        data->append(end=="π" ? "3.14159265358979323846" : QString::number(std::exp(1)));
                                    }
                                }
                            }
                        }
                        rp=2;
                    }
                    if(data->contains("²")){
                        for(int i=0;i< data->size();i++){
                            if(data->at(i)=="²"){
                                data->replace(i-1,QString::number(data->at(i-1).toDouble() * data->at(i-1).toDouble()));
                                data->removeAt(i);
                                --i;
                            }
                        }
                        begin = true;
                    }
                    else if(data->contains("*") or data->contains("/")){
                        for(int i=0;i< data->size();i++){
                            QString op = data->at(i);
                            if(op=="*" or op=="/"){
                                double a(data->at(i-1).toDouble()),b(data->at(i+1).toDouble());
                                if(b==0){
                                    error = "Division par 0 impossible";
                                    break;
                                }
                                val = op=="*" ? a*b : a/b;
                                data->replace(i-1,QString::number(val));
                                data->removeAt(i);
                                data->removeAt(i);
                                --i;
                            }
                        }
                        begin = true;
                    }
                    else if(data->contains("%")){
                        for(int i=0;i< data->size();i++){
                            if(data->at(i)=="%"){
                                int a(data->at(i-1).toInt()),b(data->at(i+1).toInt());
                                double c(a),d(b);
                                if((data->at(i-1).toDouble()-c) != 0 or (data->at(i+1).toDouble()-d) !=0){
                                    error = "L'opération modulo ne s'effectue qu'entre deux entier";
                                    break;
                                }
                                val = data->at(i-1).toInt() % data->at(i+1).toInt();
                                data->replace(i-1,QString::number(val));
                                data->removeAt(i);
                                data->removeAt(i);
                                --i;
                            }
                        }
                        begin = true;
                    }
                    else if(data->contains("+") or data->contains("-")){
                        for(int i=0;i< data->size();i++){
                            QString op = data->at(i);
                            if(op=="+" or op=="-"){
                                double a(data->at(i-1).toDouble()),b(data->at(i+1).toDouble());
                                val = op=="+" ? a+b : a-b;
                                data->replace(i-1,QString::number(val));
                                data->removeAt(i);
                                data->removeAt(i);
                                --i;
                            }
                        }
                        begin = true;
                    }
                }
                if(error==""){
                    content="";
                    ans = data->at(0);
                    print = "";
                    eq.append(exp + data->join(""));
                    data->clear();
                }else{
                    print = error;
                }
            }
            else{
               print = "Operation Impossible";
            }
        }
        else{
            if(content.startsWith("0")){
                if(content.startsWith("0.")){
                    print = data->join("") + newtext + btnText;
                }
                else if(btnText=="."){
                    print = data->join("") + newtext + btnText;
                }
                else{
                    content.remove(0,1);
                    print = data->join("")+btnText;
                }
            }
            else{
                print = data->join("")+newtext+btnText;
            }
        }
        ui->screen->setText(print);
        ui->dataSeen->setText(eq.join("<br><br>"));
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
