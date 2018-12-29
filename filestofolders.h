#ifndef FILESTOFOLDERS_H
#define FILESTOFOLDERS_H

#include <QObject>
#include "filecompare.h"
#include "enums.h"

class SortFilesToFolders : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList inputFolders READ inputFolders NOTIFY inputFoldersChanged)
    Q_PROPERTY(QString outputFolder READ outputFolder WRITE setOutputFolder NOTIFY outputFolderChanged)
    Q_PROPERTY(QStringList sortBy READ sortBy CONSTANT)
    Q_PROPERTY(int sortBySelection READ sortBySelection WRITE setSortBySelection NOTIFY sortBySelectionChanged)

public:
    QStringList inputFolders() const;
    QString outputFolder() const;
    QStringList sortBy() const;
    int sortBySelection() const;
    Q_INVOKABLE void addInputFolder(const QString& folder);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void sortToFolders(int sortBy);

public slots:
    void setOutputFolder(const QString& outputFolder);
    void setSortBySelection(int sortBySelection);

signals:
    void inputFoldersChanged(const QStringList& inputFolders);
    void outputFolderChanged(const QString& outputFolder);
    void sortBySelectionChanged(int sortBySelection);

private:
    std::map<QString, QString> outputPathsForInputPaths(const QStringList &files, const std::function<std::optional<QDateTime>(const QString&)>& predicate);

    Folders m_inputFolders;
    QString m_outputFolder;
    int m_sortBySelection;
    FileList m_fileList;
};

#endif // FILESTOFOLDERS_H
