#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "aboutdialog.h"

#include <QDebug>
#include <QPushButton>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  QPushButton *addTabButton = new QPushButton("+",ui->tabWidget);

  connect(addTabButton,&QPushButton::clicked,this,&MainWindow::addNewItem);
  connect(ui->tabWidget,&QTabWidget::tabCloseRequested,this,&MainWindow::closeTab);
  setWindowTitle("Text Editor");


  ui->tabWidget->setCornerWidget(addTabButton,Qt::TopLeftCorner);
  ui->tabWidget->setTabsClosable(true);
  ui->tabWidget->setMovable(true);

  addNewItem();
}


MainWindow::~MainWindow()
{
  delete ui;
  while(m_textAreas.empty()){
    delete m_textAreas.back();
    m_textAreas.pop_back();
  }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event);
  //    for(auto& textArea : m_textAreas){
  //    textArea->resize(textArea->width(),textArea->height());
  //    }

//  TabItem* area = m_textAreas.at(ui->tabWidget->currentIndex());
//  area->resize(area->width(),area->height());
}

int MainWindow::getNumDoc()
{
  for(int i = 0 ; static_cast<size_t> (i) < m_counterNoNameDoc.size(); i++){

    if((i+1) != m_counterNoNameDoc.at(i)){
      //int dif = counterNewDoc.at(i) - (i+1);
      m_counterNoNameDoc.insert(m_counterNoNameDoc.begin() + i, i + 1);
      return i+1;

    }
  }

  if(m_counterNoNameDoc.empty()){
    m_counterNoNameDoc.push_back(1);
    return m_counterNoNameDoc.back();
  }else{
    m_counterNoNameDoc.push_back( m_counterNoNameDoc.back() + 1 );
    return m_counterNoNameDoc.back();
  }
}

void MainWindow::addNewItem(int index)
{
  Q_UNUSED(index);

  TabItem* item = new TabItem(this);

  ui->tabWidget->addTab(item,"new "+QString::number(getNumDoc())+" ");
  ui->tabWidget->setCurrentWidget(item);

  m_textAreas.push_back(item);
}



void createFile(){

  QString pathTxtFile;

  pathTxtFile  = QDir::currentPath();
  pathTxtFile += QDir::separator();
  pathTxtFile += "test.txt";

  QFile file(pathTxtFile);
  if(!file.open(QFile::WriteOnly)){
    return void();
  }

  QTextStream stream(&file);

  QString str;
  QString icounter;
  for(size_t i = 0; i < 19999999; ++i){
    str.clear();
    icounter.clear();
    icounter = QString::number(i);

    str += icounter + " " + icounter + " " + icounter + " " + icounter + " " + icounter + " " + icounter + " " + icounter + " " + icounter;
    stream << str;
    stream << '\n';
  }
  qDebug() << str;
  file.close();
  exit(0);

}



void MainWindow::on_actionTarget_1_2_triggered()
{

  QString path;
  // path = QDir::currentPath(); // ?????????? ????????????????????????
  path = "F:\big-text-viewer";

  //createFile();


  QString pathFile = QFileDialog::getOpenFileName(0, "Open File", ""); //"*.txt " , ???????? ???????????????? ??????, ?????????? ???????????? ???????????? ???? ???????? ??????????
  if(pathFile == ""){
    return void();
  }

  File* file = new File(pathFile,this);

  if(!file->open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text)){
      QMessageBox msg;
      msg.setText("????????????");
      msg.setInformativeText("???? ?????????????? ?????????????? ????????!");
      msg.setStandardButtons(QMessageBox::Ok);
      msg.exec();
      return void();
  }


 if( m_textAreas.at(ui->tabWidget->currentIndex())->empty()){

  m_textAreas.at(ui->tabWidget->currentIndex())->setFile(file);
   }else{

  addNewItem();
  m_textAreas.at(ui->tabWidget->currentIndex())->setFile(file);

   }

  int pos = ui->tabWidget->tabText(ui->tabWidget->currentIndex()).indexOf(" ");
  int numDoc = ui->tabWidget->tabText(ui->tabWidget->currentIndex()).right(pos).toInt();
  m_counterNoNameDoc.erase(std::remove(m_counterNoNameDoc.begin(), m_counterNoNameDoc.end(), numDoc), m_counterNoNameDoc.end());

  int posSeparator = pathFile.lastIndexOf("/");
  int dif = pathFile.length() - posSeparator;
  QString fileName = pathFile.right(dif-1);
  ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),fileName);
}

void MainWindow::closeTab(int index)
{
  //???? ?????????????? ???? ???????????????? ???????????????????? ????????????????????, ???????? ???????? ????????????????, ???????? ?? ???????? ?????? ???????? ??????
  if(!m_textAreas.at(index)->file()){

    int pos = ui->tabWidget->tabText(index).indexOf(" ");
    int numDoc = ui->tabWidget->tabText(index).right(pos).toInt();

    m_counterNoNameDoc.erase(std::remove(m_counterNoNameDoc.begin(), m_counterNoNameDoc.end(), numDoc), m_counterNoNameDoc.end());
  }

  ui->tabWidget->removeTab(index);
  delete m_textAreas[index];
  m_textAreas.erase(m_textAreas.begin() + index);

  if(m_textAreas.empty()){
    addNewItem();  // ???????? ?????????????????? ??????????????, ???? ???????????? ?????????????? ??????????????????
  }

}

void MainWindow::slotAbout()
{
  AboutDialog dlg(this);
  dlg.exec();
}

void MainWindow::slotNew()
{
  addNewItem();
}

void MainWindow::slotExit()
{
  exit(0);
}





