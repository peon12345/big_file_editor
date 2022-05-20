#include "smartscroll.h"
#include <QDebug>
#include <cmath>

SmartScroll::SmartScroll(QWidget *parent) : QScrollBar(parent)
{
}

void SmartScroll::setMax(int max)
{
    setMaximum(max);
}
