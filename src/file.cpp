#include "file.h"
#include <QDebug>
#include <thread>

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

File::~File() {
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

uint64_t File::countInFile(const QString &str)
{
    if(!seek(0)){
        return 0;
    }


    uint64_t counter = 0;
    uint64_t currentSize = 0;

    QByteArray byteArray;
    while(!atEnd()){


      if(isOpen()){
        std::unique_lock<std::mutex> ul(m_mutexReadFile);

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

  std::thread t([ this, posScrollBar](){

   // exit(2);
    std::unique_lock<std::mutex> ul(m_mutexReadFile);
    if((size_t)posScrollBar < m_linesMap.size()){
        seek(m_linesMap.at(posScrollBar));

      qDebug() << posScrollBar  << "posScrollBar";
      qDebug() << m_linesMap.at(posScrollBar)  << "seek";


    }else if((size_t)posScrollBar > m_linesMap.size()){
        seek(m_linesMap.at(m_linesMap.size()-1));

      qDebug() << posScrollBar  << "posScrollBar";
      qDebug() << m_linesMap.at(posScrollBar)  << "seek";


    }else if(posScrollBar == 0){
        seek(0);
    }

    // QString text = read(SIZE_BLOCK);
    QString text = m_textStream.read(BLOCK_SIZE);

    emit needUpdateText(text,posScrollBar);

   });

  t.detach();
}


void File::close()
{
  std::unique_lock<std::mutex> ul(m_mutexReadFile);
  QFile::close();
}
