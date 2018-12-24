#ifndef FILECOMPARE_H
#define FILECOMPARE_H

#include <QObject>
#include <QSet>
#include <QThreadPool>
#include "listmodel.h"

class DuplicateFilesModel : public ListModel<QStringList>
{
    Q_OBJECT
public:
    enum Roles {
        FileType = Qt::UserRole + 1,
    };
    Q_ENUM(Roles)

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
};
Q_DECLARE_METATYPE(DuplicateFilesModel*)

class Folders : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList inputFolders READ inputFolders WRITE setInputFolders NOTIFY inputFoldersChanged)
    Q_PROPERTY(QStringList outputFolders READ outputFolders WRITE setOutputFolders NOTIFY outputFoldersChanged)

public:
    QStringList inputFolders() const;
    QStringList outputFolders() const;
    Q_INVOKABLE void addInputFolder(const QString& folder);
    Q_INVOKABLE void build();
    Q_INVOKABLE void clear();

public slots:
    void setInputFolders(const QStringList& inputFolders);
    void setOutputFolders(const QStringList& outputFolders);

signals:
    void inputFoldersChanged(const QStringList& outputFolders);
    void outputFoldersChanged(const QStringList& outputFolders);

private:
    QStringList m_inputFolders;
    QStringList m_outputFolders;
};

class FileList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList files READ files NOTIFY filesChanged)
    Q_PROPERTY(QStringList extensions READ extensions NOTIFY extensionsChanged)
    Q_PROPERTY(QStringList filter READ filter WRITE setFilter NOTIFY filterChanged)

public:
    QStringList files() const;
    QStringList extensions() const;
    QStringList filter() const;
    Q_INVOKABLE void build(const QStringList &folders);
    Q_INVOKABLE void clear();

public slots:
    void setFilter(QStringList filter);

signals:
    void filesChanged(const QStringList& files);
    void extensionsChanged(const QStringList& extensions);
    void filterChanged(QStringList filter);

private:
    Folders m_folders;
    QSet<QString> m_files;
    QSet<QString> m_extensions;
    QStringList m_filter;
};

class FileCompare : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(DuplicateFilesModel* duplicateFiles READ duplicateFiles NOTIFY duplicateFilesChanged)

public:
    FileCompare();
    double progress() const;
    DuplicateFilesModel* duplicateFiles();
    Q_INVOKABLE void compare(const QStringList& files);
    Q_INVOKABLE void clear();

public slots:
    void setProgress(double progress);

signals:
    void progressChanged(double progress);
    void duplicateFilesChanged(const DuplicateFilesModel& duplicateFiles);

private:
    double m_progress;
    QMap<quint32, QStringList> m_fileInfo;
    DuplicateFilesModel m_duplicateFiles;
};

class FileCompareWorker : public QObject
{
    Q_OBJECT

public:
    FileCompareWorker(const QStringList& filePaths);
    void compare();

signals:
    void progressChanged(double progress);
    void file(quint32 hash, const QString& filePath);
    void finished();

private:
    QStringList m_filePaths;
    QThreadPool m_threadPool;
    int m_futureCounter;
};

#endif // FILECOMPARE_H
