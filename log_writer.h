#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QPushButton>
#include <QMainWindow>
#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum
{
    invalid_table_type = -1,
    department_table_type = 0,
    staff_table_type = 1,
    resign_staff_table_type = 2,
};

enum
{
    invalid_index = -1,
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:


private:
    Ui::MainWindow *ui;
    // 整体布局
    QVBoxLayout* hole_layout_;
    // 表格界面
    QTableWidget* content_table_;
    // 表格类型
    int table_type_;

    // 搜索内容
    QLineEdit* search_;
};
#endif // MAINWINDOW_H
