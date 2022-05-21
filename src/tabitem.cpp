#include "tabitem.h"
#include <QDebug>

TabItem::TabItem(QWidget *parent) : QWidget(parent),m_file(nullptr),m_tempValueScrollBar(0)
{

  m_layout.addWidget(&m_textArea);

  setLayout(&m_layout);
}



bool TabItem::empty() const
{
  return m_textArea.empty();
}

void TabItem::setFile(File* file)
{

  if(!file){
      return void();

    }

  m_file = file;
  connect(m_file,&File::needUpdateText,&m_textArea,&TextArea::updateText); //файл достал кусок текста и передает его  в TextArea

  if(file->isMassive()){

     m_textArea.hideScroll();
     m_layout.addWidget(&m_scroll);
     connect(m_file,&File::linesCounted,&m_scroll,&SmartScroll::setMax);


     connect(m_textArea.verticalScrollBar(),&QScrollBar::valueChanged,[this](int value){


         if(value != 0){

         if(m_tempValueScrollBar > value){ // значит воднялись наверх

           m_scroll.setValue(m_scroll.value() - (m_tempValueScrollBar - value  ));

           qDebug() << "QTextEdit "<< value;
           qDebug() << "m_scroll "<< m_scroll.value();

           }else if(m_tempValueScrollBar < value){

           m_scroll.setValue(m_scroll.value() + ( value - m_tempValueScrollBar));


           qDebug() << "QTextEdit "<< value << "max" << m_textArea.verticalScrollBar()->maximum();
           qDebug() << "m_scroll "<< m_scroll.value();


           if(value >= m_textArea.verticalScrollBar()->maximum()){
               m_file->readBlock(m_scroll.value());
             }


           }

        m_tempValueScrollBar = value;
           }
       });

      connect(&m_scroll,&QScrollBar::sliderReleased,[this] () {
         m_file->readBlock(m_scroll.value());
        });



      m_file->readBlock(0); // прочитаем какую то малую часть, и вставим текст
      m_file->startLinesCount();


    }else {
      m_file->readBlock(0);
    }
}

const File *TabItem::file()
{
  return m_file;
}
