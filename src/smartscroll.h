#include <QScrollBar>
#include "file.h"

class SmartScroll : public QScrollBar {

public:
  SmartScroll(QWidget* parent = nullptr);

public slots:
  void setMax(int max);

};
