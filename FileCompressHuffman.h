#pragma once

#include "huffman2.hpp"
#include <string>
using namespace std;

struct CharInfo
{
    CharInfo()
        :_chCount(0)
        ,_strCode()
    {}
    friend bool operator>(const CharInfo &left, const CharInfo &right)
    {
        return left._chCount > right._chCount;
    }
    friend CharInfo operator+(const CharInfo &left, const CharInfo &right)
    {
        CharInfo tmp;
        tmp._chCount = left._chCount + right._chCount;
        return tmp;
    }
    unsigned char _ch;
    long long _chCount; //存放每个字节出现的次数
    string _strCode; //存放每个字节对应的编码
};

class FileCompressHuffman
{
public:
    void CompressFile(string FilePath, string outPath);
    void UnCompressFile(string FilePath, string outPath);
private:
    //获取编码
    void GetCode(const HuffmanTreeNode<CharInfo>* hT);
    //获取文件名
    string GetFileName(string& FilePath);
    //获取文件信息
    void GetFileInfo(FILE* fOut, string& FilePath);
    //处理压缩后的数据
    void CompressData(FILE *fIn, FILE *fOut);
    //获取文件头部信息
    void GetHead(FILE *fIn, string& FileSuffixName);
    //获取压缩文件每一行的信息
    void GetLine(FILE *fIn, unsigned char* buf);
    //编码转化为原数据
    void CodeToData(FILE* fIn, FILE *fOut, HuffmanTreeNode<CharInfo>* hT);
private:
    CharInfo info[256];
};
