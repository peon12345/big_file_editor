#pragma once
#include <QObject>
#include <QTextEdit>
#include <QScrollBar>
#include "file.h"



class TextArea : public QTextEdit { // наследуемся, чтобы добавить возможность нумерации строк, подцветку выделенной строки и тд
  Q_OBJECT
public:
  TextArea(QWidget *wdg = nullptr);
  bool empty() const;
  void hideScroll();
//protected:
//  void resizeEvent(QResizeEvent *e) override;
public slots:
  void updateText(const QString& text);

};

