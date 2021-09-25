#include "fakedata.h"
#include <qalgorithms.h>

FakeData::FakeData()
{

}

FakeFileTree::FakeFileTree(const int fileCount)
{
//    qDebug("filecount %d", fileCount);
    int totalChildCount = 0;
    isFolder = (qrand() % fileCount) > 2;
    name = (isFolder ? QString("文件夹 %1") : QString("文件 %1")).arg(qrand());

//    qDebug("isFolder: %d", isFolder);
    if (!isFolder) return;

    while (totalChildCount < fileCount) {
        int newChild = qrand() % (fileCount - totalChildCount) + 1;
        totalChildCount += newChild;
        auto child = new FakeFileTree(newChild);
        if (!child->isFolder) children.append(child);
        else children.push_front(child);
    }
}
