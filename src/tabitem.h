#include <QHBoxLayout>
#include "textarea.h"
#include "smartscroll.h"
#include "file.h"

class TabItem : public QWidget {

public:
  TabItem (QWidget* parent = nullptr);

  bool empty() const;
  void setFile(File* file);
   const File* file();
private:
  TextArea m_textArea;
  File* m_file;
  SmartScroll m_scroll;
  QHBoxLayout m_layout;


private:
  int m_tempValueScrollBar;
};
