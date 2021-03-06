#pragma once
#include <QMainWindow>
#include "tabitem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionTarget_1_2_triggered();
    void closeTab(int index);

    void slotAbout();
    void slotNew();
    void slotExit();

protected:
   void resizeEvent(QResizeEvent *event) override;
private:
    std::vector<TabItem*> m_textAreas;
    Ui::MainWindow *ui;

    std::vector<int> m_counterNoNameDoc;
    int getNumDoc();
 private:
    void addNewItem(int index = 0);
};

