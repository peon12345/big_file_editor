#include "filemanager.h"

FileManager::FileManager(QObject *object) : QObject(object)
{

}

FileManager::~FileManager()
{
  while(!m_openFiles.empty()){
    m_openFiles.back()->close();
    delete m_openFiles.back();
    m_openFiles.pop_back();
  }
}

int FileManager::openFile(const QString &path)
{
  File* file = new File(path,this);

  if(!file->open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text)){
    return 0;
  }

  m_openFiles.push_back(file);

  return 1;
}

int FileManager::saveFile(const QString &path)
{
  Q_UNUSED(path);
  return 1;
}

File *FileManager::getFile(int index)
{
  if(index == -1){
    index = m_openFiles.size() - 1; // не передали индекс - берем последний открытый файл
  }


  return m_openFiles.at(index);
}

void FileManager::closeFile(size_t index)
{
  if(index < m_openFiles.size()){
    m_openFiles.at(index)->close();
    delete m_openFiles.at(index);
    m_openFiles.erase(m_openFiles.begin() + index);
  }
}


