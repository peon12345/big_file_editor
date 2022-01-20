#pragma once
#include <QObject>
#include <QFile>
#include <QTextStream>

class File : public QFile
{
    Q_OBJECT
public:
    explicit File(const QString& path,QObject* object = nullptr);
    ~File() override;

    File  (const File& file) = delete;
    File& operator = (const File& file) = delete;

    File (File&& file) = delete;
    File& operator = (File&& file) = delete;

    bool isMassive() const;
    qint64 sizeFile() const;
    uint64_t countInFile(const QString& str);
    uint64_t countLines();
    float getAverageLineSize();

    QString readAllData(); // почему не работает этот метод, в упор не вижу

    static constexpr int SIZE_BLOCK = 10485760; //10mb
  private:
    bool m_isMassiveFile;
    mutable uint m_totalLines;
    std::vector<uint64_t> m_linesMap;
    QTextStream* m_textStream;
private:
    void setLinesInFile(uint64_t totalLines) const;
public slots:
    void readBlock(int posScrollBar);
    void updateText(const QString& text,int posScrollBar);

signals:
    void needUpdateText(const QString& text,int posScrollBar);
};


