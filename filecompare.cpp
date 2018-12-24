#include "filecompare.h"
#include <QtConcurrent>
#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QFuture>
#include <QFutureWatcher>
#include <QThread>

FileCompareWorker::FileCompareWorker(const QStringList &filePaths)
    : m_filePaths(filePaths),
      m_futureCounter(0)
{
    m_threadPool.setMaxThreadCount(QThread::idealThreadCount());
}

void FileCompareWorker::compare()
{
    const auto size = m_filePaths.size();
    for (auto i = 0; i < size; i++) {
        const auto filePath = m_filePaths.at(i);

        auto watcher = new QFutureWatcher<std::pair<quint32,QString>>;
        connect(watcher, &QFutureWatcher<std::pair<quint32,QString>>::finished, watcher, &QFutureWatcher<std::pair<quint32,QString>>::deleteLater);
        connect(watcher, &QFutureWatcher<std::pair<quint32,QString>>::finished, this, [=](){

            const auto hash = watcher->result().first;
            const auto filePath = watcher->result().second;

            emit file(hash, filePath);

            if (size == ++m_futureCounter)
                emit finished();
        });

        watcher->setFuture(QtConcurrent::run(&m_threadPool, [=]() -> std::pair<quint32,QString> {
            QFile file(filePath);
            if (!file.open(QFile::ReadOnly)) {
                qDebug() << "Unable to open file: " << filePath;
                return std::pair<quint32,QString>();
            }

            const auto fileData = file.readAll();
            const auto hash = qHash(fileData);

            const auto progressPercent = (( double(i) + 1) / double(size)) * 100;
            emit progressChanged(progressPercent);

            return std::pair<quint32,QString>(hash, filePath);
        }));
    }
}

DuplicateFilesModel *FileCompare::duplicateFiles()
{
    return &m_duplicateFiles;
}

void FileCompare::compare(const QStringList &files)
{
    clear();

    const auto thread = new QThread;
    const auto worker = new FileCompareWorker(files);
    worker->moveToThread(thread);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(thread, &QThread::started, worker, &FileCompareWorker::compare);
    connect(worker, &FileCompareWorker::progressChanged, this, &FileCompare::setProgress);
    connect(worker, &FileCompareWorker::finished, thread, &QThread::quit);
    connect(worker, &FileCompareWorker::finished, worker, &FileCompareWorker::deleteLater);
    connect(worker, &FileCompareWorker::file, this, [=](quint32 hash, const QString& filePath){
        if (m_fileInfo.contains(hash)) {
            auto filePaths = m_fileInfo.value(hash);
            filePaths.append(filePath);
            m_fileInfo.insert(hash, filePaths);
        } else {
            m_fileInfo.insert(hash, QStringList(filePath));
        }
    });
    connect(worker, &FileCompareWorker::finished, this, [=](){
        for (const auto& item : m_fileInfo) {
            Q_ASSERT(item.size() > 0);
            if (item.size() == 1)
                continue;

            m_duplicateFiles.insertRow(item);
            emit duplicateFilesChanged(m_duplicateFiles);
        }
    });

    thread->start();
}

FileCompare::FileCompare()
    : m_progress(0)
{
}

double FileCompare::progress() const
{
    return m_progress;
}

void FileCompare::setProgress(double progress)
{
    if (m_progress > progress)
        return;

    m_progress = progress;
    emit progressChanged(m_progress);
}

void FileCompare::clear()
{
    m_progress = 0;
    m_fileInfo.clear();
    m_duplicateFiles.clear();
    emit progressChanged(m_progress);
    emit duplicateFilesChanged(m_duplicateFiles);
}

QStringList Folders::inputFolders() const
{
    return m_inputFolders;
}

void Folders::addInputFolder(const QString &folder)
{
    m_inputFolders.push_back(folder);
    emit inputFoldersChanged(m_inputFolders);
}

void Folders::build()
{
    QSet<QString> folders;
    const auto size = m_inputFolders.size();
    for (auto i = 0; i < size; i++) {
        const auto folder = m_inputFolders.at(i);
        QDirIterator it(folder, QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            const auto folder = it.next();
            folders.insert(folder);
        }
    }

    setOutputFolders(folders.toList());
}

void Folders::clear()
{
    m_inputFolders.clear();
    m_outputFolders.clear();
    emit inputFoldersChanged(m_inputFolders);
    emit outputFoldersChanged(m_outputFolders);
}

void Folders::setInputFolders(const QStringList &inputFolders)
{
    if (m_inputFolders == inputFolders)
        return;

    m_inputFolders = inputFolders;
    emit inputFoldersChanged(m_inputFolders);
}

void Folders::setOutputFolders(const QStringList &outputFolders)
{
    if (m_outputFolders == outputFolders)
        return;

    m_outputFolders = outputFolders;
    emit outputFoldersChanged(m_outputFolders);
}

QStringList Folders::outputFolders() const
{
    return m_outputFolders;
}

void FileList::build(const QStringList &folders)
{
    clear();
    m_folders.clear();
    m_folders.setInputFolders(folders);
    m_folders.build();
    const auto outputFolders = m_folders.outputFolders();

    const auto size = outputFolders.size();
    for (auto i = 0; i < size; i++) {
        const auto folder = outputFolders.at(i);
        auto it = m_filter.empty() || m_filter.at(0) == "" ? QDirIterator(folder, QDir::Files) : QDirIterator(folder, m_filter, QDir::Files);
        while (it.hasNext()) {
            const auto filePath = it.next();
            m_files.insert(filePath);
            m_extensions.insert(QFileInfo(filePath).suffix());
        }
    }

    emit filesChanged(m_files.toList());
    emit extensionsChanged(m_extensions.toList());
}

QStringList FileList::extensions() const
{
    auto extensionList = m_extensions.toList();
    extensionList.sort();
    return std::move(extensionList);
}

QStringList FileList::files() const
{
    auto fileList = m_files.toList();
    fileList.sort();
    return std::move(fileList);
}

void FileList::clear()
{
    m_folders.clear();
    m_files.clear();
    m_extensions.clear();
    emit filesChanged(m_files.toList());
    emit extensionsChanged(m_extensions.toList());
}

void FileList::setFilter(QStringList filter)
{
    if (m_filter == filter)
        return;

    m_filter = filter;
    emit filterChanged(m_filter);
}

QStringList FileList::filter() const
{
    return m_filter;
}

QVariant DuplicateFilesModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)

    if (index.row() < static_cast<int>(m_items.size()))
        return m_items.at(index.row());

    return QVariant();
}

bool DuplicateFilesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role)

    if (index.row() < static_cast<int>(m_items.size())) {
        m_items[index.row()] = value.toStringList();
        return true;
    }

    return false;
}
