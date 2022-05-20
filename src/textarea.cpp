#include "textarea.h"


#include <QTextStream>
#include <QDebug>
#include <thread>
#include <QCloseEvent>
#include <QMessageBox>
#include <QRandomGenerator>


TextArea::TextArea(QWidget * wdg) : QTextEdit(wdg)
{

}


bool TextArea::empty() const
{
  return (QTextEdit::document()->isEmpty() );

}

void TextArea::hideScroll()
{
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

//void TextArea::resizeEvent(QResizeEvent *e)
//{
//    if(m_file != nullptr){
//        if(!m_file->isMassive()){
//            QTextEdit::resizeEvent(e);
//            return void();
//        }else{
//            //int posScrollBarBeforeResize = verticalScrollBar()->sliderPosition();
//            QTextEdit::resizeEvent(e);

////            verticalScrollBar()->setMinimum(0);
////            verticalScrollBar()->setMaximum(m_maxValueScrollBar);

////            verticalScrollBar()->setValue(posScrollBarBeforeResize);
////            verticalScrollBar()->setSliderPosition(posScrollBarBeforeResize);

//        }
//    }else{
//        QTextEdit::resizeEvent(e);
//        return void();
//      }
//}




void TextArea::updateText(const QString &text)
{
    this->setText(text);
}





