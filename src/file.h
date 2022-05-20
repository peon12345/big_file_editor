#pragma once
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <mutex>
#include <future>
#include <QTimer>

class File : public QFile
{
    Q_OBJECT
public:
    File(const QString& path,QObject* object = nullptr);

    bool isMassive() const;
    qint64 sizeFile() const;
    uint64_t countInFile(const QString& str);
    uint64_t countLines();
    float getAverageLineSize();

    QString readAllData(); // почему не работает этот метод, в упор не вижу
    void startLinesCount();

    //static constexpr int BLOCK_SIZE = 10485760; //10mb
    static constexpr int BLOCK_SIZE = 1000; //10mb
  private:
    bool m_isMassiveFile;
    mutable uint m_totalLines;
    std::vector<uint64_t> m_linesMap;
    QTextStream m_textStream;
private:
    void setLinesInFile(uint64_t totalLines) const;

private:
    std::mutex m_mutexReadFile;
    std::future<uint64_t> m_futureLines;
    QTimer m_timerCheckLines;
public slots:
    void readBlock(int posScrollBar);
signals:
    void needUpdateText(const QString& text);
    void linesCounted(uint64_t lines);
  // QIODevice interface
public:
  void close() override;
};


