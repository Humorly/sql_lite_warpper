#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "sql_lite_warpper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 查询结果
    sql_lite_warpper sql_("test.db");

    sql_.create("CREATE TABLE company_department (type TEXT NOT NULL, name TEXT NOT NULL, password TEXT NOT NULL)");

    sql_.insert("insert into company_department (type, name, password) values (?, ?, ?)",
        std::tuple<std::string, QString, std::string>("A", ("test 111"), "123"));
    sql_.insert("insert into company_department (type, name, password) values (?, ?, ?)",
        std::tuple<std::string, QString, std::string>("A", ("test 222"), "123"));
    sql_.insert("insert into company_department (type, name, password) values (?, ?, ?)",
        std::tuple<std::string, QString, std::string>("A", ("test 333"), "123"));

    sql_.update("update company_department set type=? where name='test 111'", std::tuple<std::string>("B"));
    std::vector<std::tuple<std::string, QString, std::string>> dest_;
    sql_.select("select * from company_department", std::tuple<>(), dest_);

    for (auto i = 0; i < dest_.size(); ++i)
    {
        qDebug() << std::get<0>(dest_[i]).c_str() << " ";
        qDebug() << std::get<1>(dest_[i]) << " ";
        qDebug() << std::get<2>(dest_[i]).c_str() << "\n";
    }
    dest_.clear();

    sql_.select("select type, name, password from company_department where type = ?", std::tuple<std::string>("A"), dest_);
    for (auto i = 0; i < dest_.size(); ++i)
    {
        qDebug() << std::get<0>(dest_[i]).c_str() << " ";
        qDebug() << std::get<1>(dest_[i]) << " ";
        qDebug() << std::get<2>(dest_[i]).c_str() << "\n";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

