#include "file.h"
#include <QDebug>
#include <thread>
#include <QElapsedTimer>

File::File(const QString &path, QObject* object) : QFile(path,object),
    m_isMassiveFile(false),
    m_totalLines(0),
    m_textStream(this)
{
    //здесь узнаем о файле , например, размер файла, нужно ли включать ленивую загрузку

    if(sizeFile() > BLOCK_SIZE){ // порог для ленивой загрузки, нужно указавывать в настройках
        m_isMassiveFile = true;
    }

}

bool File::isMassive() const
{
    return m_isMassiveFile;
}

qint64 File::sizeFile() const
{
    return size();
}

uint64_t File::countLines()
{
    m_linesMap.clear();
    m_linesMap.push_back(0);

    uint64_t lines =  countInFile("\n");

    if(lines >= 0){
      setLinesInFile(lines);
    }

    return lines;
}

float File::getAverageLineSize()
{
    if(m_totalLines == 0){
        m_totalLines = countLines();
    }

    return sizeFile() / m_totalLines;
}
//19796881
uint64_t File::countInFile(const QString &str)
{
    if(!seek(0)){
        return 0;
    }

    uint64_t counter = 0;
    uint64_t currentSize = 0;

    QByteArray byteArray;
    qDebug() << "pre mutex lock count";
    std::unique_lock<std::mutex> ul(m_mutexReadFile);
    qDebug() << "after mutex lock count" ;
    seek(0);



    while(!atEnd()){

      if(isOpen()){

        byteArray = read(BLOCK_SIZE * 5); // 50mb

        for(int i = 0; i < byteArray.size();i++){
            if(str == byteArray.at(i)){

                counter++;
                if(str == '\n'){

                    m_linesMap.push_back(currentSize + i);
                }
            }
        }

        currentSize += byteArray.size();

    }
    }

    qDebug() << "scroll range complete" << counter;
    return counter;
}

void File::setLinesInFile(uint64_t totalLines) const
{
    m_totalLines = totalLines;
}

QString File::readAllData()
{

    return m_textStream.read(BLOCK_SIZE);
}

void File::readBlock(int posScrollBar)
{

  qDebug() << "size" << m_linesMap.size();


  std::thread t([ this, posScrollBar](){

    qDebug() << "before mutex lock read block";
    std::unique_lock<std::mutex> ul(m_mutexReadFile);

    qDebug() << "after mutex lock read block";
    if((size_t)posScrollBar < m_linesMap.size()){
        seek(m_linesMap.at(posScrollBar));

      qDebug() << posScrollBar  << "posScrollBar";
      qDebug() << m_linesMap.at(posScrollBar)  << "seek";


    }else if((size_t)posScrollBar > m_linesMap.size()){
        seek(m_linesMap.at(m_linesMap.size()-1));

      qDebug() << posScrollBar  << "posScrollBar";
      qDebug() << m_linesMap.at(m_linesMap.size()-1)  << "seek";


    }else if(posScrollBar == 0){
        seek(0);
    }

    QString text = m_textStream.read(BLOCK_SIZE);

   // std::vector<uint64_t> test(m_linesMap.begin() + m_linesMap.size()-50,m_linesMap.end());

    emit needUpdateText(text);
    qDebug() << "block readed";

   });

   t.detach();


}


void File::close()
{
  std::unique_lock<std::mutex> ul(m_mutexReadFile);
  QFile::close();
}




void File::startLinesCount()
{
  m_futureLines = std::async(std::launch::async,&File::countLines,this);
  m_timerCheckLines.start(2000);

  connect(&m_timerCheckLines,&QTimer::timeout, [ this] () {

    if(m_futureLines.wait_for(std::chrono::seconds(0)) == std::future_status::ready){
      m_timerCheckLines.stop();
      emit linesCounted(m_futureLines.get());
      }
    } );


}














