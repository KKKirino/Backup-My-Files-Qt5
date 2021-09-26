#ifndef FAKEDATA_H
#define FAKEDATA_H

#include <QString>
#include <QVector>

class FakeData
{
public:
    FakeData();
};

class FakeFileTree
{
public:
    FakeFileTree(const int fileCount = 100);

    QVector<FakeFileTree*> children = {};
    QString name;

    bool isFolder;
    bool operator < (const FakeFileTree& other) const {
        return isFolder < other.isFolder;
    };

private:
    FakeFileTree* buildFakeFolder();
    FakeFileTree* buildFakeFile();
};

#endif // FAKEDATA_H
