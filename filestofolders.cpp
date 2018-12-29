#include "filestofolders.h"
#include <QRegularExpression>
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <functional>
#include <QDir>

void SortFilesToFolders::addInputFolder(const QString &folder)
{
    m_inputFolders.addInputFolder(folder);
    emit inputFoldersChanged(m_inputFolders.inputFolders());
}

void SortFilesToFolders::clear()
{
    m_inputFolders.clear();
    m_outputFolder.clear();
    emit inputFoldersChanged(m_inputFolders.inputFolders());
    emit outputFolderChanged(m_outputFolder);
}

void SortFilesToFolders::sortToFolders(int sortBy)
{
    const auto sortByEnum = Enums::toEnum<Enums::SortBy>(sortBy);
    m_fileList.build(m_inputFolders.inputFolders());
    const auto files = m_fileList.files();

    std::map<QString,QString> filesToCopy;
    switch (sortByEnum) {
    case Enums::SortBy::DateTaken:
        filesToCopy = outputPathsForInputPaths(files, [=](const QString& filePath) -> std::optional<QDateTime> {
            const auto dateTime = QFileInfo(filePath).birthTime();
            if (!dateTime.isValid())
                return std::nullopt;

            return std::move(dateTime);
        });
        break;
    case Enums::SortBy::DateModified:
        filesToCopy = outputPathsForInputPaths(files, [=](const QString& filePath) -> std::optional<QDateTime> {
            const auto dateTime = QFileInfo(filePath).lastModified();
            if (!dateTime.isValid())
                return std::nullopt;

            return std::move(dateTime);
        });
        break;
    case Enums::SortBy::IosFilename:
        QRegularExpression regExp(QString(R"((\d\d\d\d-\d\d-\d\d).{0,}(\d\d\.\d\d\.\d\d))"));
        filesToCopy = outputPathsForInputPaths(files, [=](const QString& filePath) -> std::optional<QDateTime> {
            const auto match = regExp.match(filePath);
            if (!match.hasMatch())
                return std::nullopt;

            return QDateTime::fromString(match.captured(1) + " " + match.captured(2), "yyyy-MM-dd hh.mm.ss");
        });
        break;
    }

    for (const auto& [inputPath, outputPath] : filesToCopy) {
        QDir(m_outputFolder).mkpath(QFileInfo(outputPath).absolutePath());
        const auto success = QFile::copy(inputPath, outputPath);
        if (!success)
            qDebug() << "Unable to copy file " << inputPath << " to " << outputPath;
    }
}

int SortFilesToFolders::sortBySelection() const
{
    return m_sortBySelection;
}

QStringList SortFilesToFolders::sortBy() const
{
    return Enums::toStringList<Enums::SortBy>();
}

void SortFilesToFolders::setOutputFolder(const QString &outputFolder)
{
    if (m_outputFolder == outputFolder)
        return;

    m_outputFolder = outputFolder;
    emit outputFolderChanged(m_outputFolder);
}

void SortFilesToFolders::setSortBySelection(int sortBySelection)
{
    if (m_sortBySelection == sortBySelection)
        return;

    m_sortBySelection = sortBySelection;
    emit sortBySelectionChanged(m_sortBySelection);
}

std::map<QString,QString> SortFilesToFolders::outputPathsForInputPaths(const QStringList& filePaths, const std::function<std::optional<QDateTime>(const QString &)> &predicate)
{
    QHash<quint32, int> hashes;
    std::map<QString,QString> filesToCopy;


    for (auto& filePath : filePaths) {
        const auto dateTime = predicate(filePath);
        if (!dateTime) {
            qDebug() << "No match";
            continue;
        }
        auto outputFilePath = m_outputFolder + dateTime->toString("/yyyy/MM/dd/hh-mm-ss");
        outputFilePath.append("." + QFileInfo(filePath).completeSuffix());
        const auto hash = qHash(outputFilePath);
        if (hashes.contains(hash)) {
            auto value = hashes.value(hash);
            outputFilePath = QFileInfo(outputFilePath).absolutePath() + "/" + QFileInfo(outputFilePath).baseName() + "_" + QString::number(value) + "." + QFileInfo(outputFilePath).completeSuffix();
            hashes.insert(hash, value++);
        } else {
            hashes.insert(hash, 1);
        }

        filesToCopy.insert({filePath, outputFilePath});
    }

    return filesToCopy;
}

QString SortFilesToFolders::outputFolder() const
{
    return m_outputFolder;
}

QStringList SortFilesToFolders::inputFolders() const
{
    return m_inputFolders.inputFolders();
}
