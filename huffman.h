#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <queue>
#include <QDebug>
using namespace std;

#define MAX_SIZE 500
#define WRITE_BUFF_SIZE 10
#define PSEUDO_EOF 1024

typedef struct Huffman_node
{

    int id;//字符的编号
    unsigned int freq;//出现的频次
    string code;//对应的哈夫曼编码
    Huffman_node *left, *right, *parent;//左孩子、右孩子、父节点
} Node;

class Compare{
public:
    bool operator()(const Node*c1, const Node*c2)
    {
        return c1->freq > c2->freq;
    }
};

Node *nodeArray[MAX_SIZE];//叶子节点数组
Node *root;//根节点
int nodeSize;//叶子节点数
fstream inFile, outFile;//输入文件流、输出文件流
map<int, string> table;// 字符->哈夫曼编码 映射表
priority_queue<Node*, vector<Node*>, Compare> priorityQueue;//优先级队列，用于顺序

void create_node_array();//构造包含字符及其频率的数组
void create_pq();//构造优先级队列
void create_huffman_tree();//构造哈夫曼树
void create_map_table(Node* node,bool);//根据哈夫曼树建立哈夫曼映射表
bool calculate_huffman_codes();//计算哈夫曼编码
bool do_compress();//开始压缩
bool rebuid_huffman_tree();//从哈夫曼编码文件中重构哈夫曼树
void decode_huffman();//根据重构的哈夫曼树解码文件

bool compressHuff(string inFileName, string outFileName);//压缩文件
bool decompressHuff(string inFileName, string outFileName);//解压缩文件

bool compressHuff(string inFileName, string outFileName)
{
    bool flag = false;
    //打开需要压缩的文件
    inFile.open(inFileName.c_str(),ios::in);
    if(!inFile)
    {
        qDebug()<<"Open file error,path is:"<<inFileName.c_str();
        return false;
    }
    //打开压缩结果的文件
    outFile.open(outFileName.c_str(),ios::out);
    if(!outFile)
    {
        qDebug()<<"Open file error,path is:"<<outFileName.c_str();

        return false;
    }

    create_pq();//构造优先级队列

    create_huffman_tree();//构造哈夫曼树

    flag = calculate_huffman_codes();//计算哈夫曼编码
    if(!flag){
        return flag;
    }
    flag = do_compress();//开始压缩
    if(!flag){
        return flag;
    }

    inFile.close();
    outFile.close();

    return true;
}


void create_pq()
{
    int i;
    create_node_array();
    for(i = 0; i < nodeSize; ++i)
    {
        priorityQueue.push(nodeArray[i]);
    }
    qDebug()<<"create_pq size:"<<nodeSize;
}

void create_node_array()
{
    int i, count;
    int freq[MAX_SIZE] = {0};//记录出现频次
    char inChar;

    while(!inFile.eof())
    {
        inFile.get(inChar);
        if(inFile.eof())
            break;
        freq[(unsigned char)inChar]++;//出现一次，+1
    }
    count = 0;
    for(i = 0; i < MAX_SIZE; ++i)
    {

        if(freq[i] <= 0)
            continue;

        Node *node = new Node;
        node->id=i;
        node->freq=freq[i];
        node->code="";
        node->left=NULL;
        node->right=NULL;
        node->parent=NULL;

        nodeArray[count++] = node;

    }

    Node *node=new Huffman_node();
    node->id=PSEUDO_EOF;
    node->freq=1;
    node->code="";
    node->left=NULL;
    node->right=NULL;
    node->parent=NULL;

    nodeArray[count++]=node;

    nodeSize = count;
    qDebug()<<"size:"<<nodeSize;

}

void create_huffman_tree()
{
    root = NULL;
    while(!priorityQueue.empty())
    {
        Node* first = priorityQueue.top();
        priorityQueue.pop();

        if(priorityQueue.empty())
        {
            root = first;
            break;
        }
        Node *second = priorityQueue.top();
        priorityQueue.pop();

        Node *parentNode = new Node;
        parentNode->freq = first->freq + second->freq;
        if(first->freq <= second->freq)
        {
            parentNode->left = first;
            parentNode->right = second;
        }
        else
        {
            parentNode->left = second;
            parentNode->right = first;
        }

        first->parent = parentNode;
        second->parent = parentNode;

        priorityQueue.push(parentNode);

       }
    qDebug()<<"create_huffman_tree";
}

bool calculate_huffman_codes()
{

    if(root == NULL)
    {
        return false;
    }
    if(root->left != NULL)
        create_map_table(root->left, true);
    if(root->right != NULL)
        create_map_table(root->right, false);
    return true;
}

void create_map_table(Node* node,bool left)
{

    if(left){
        node->code = node->parent->code + "0";
    }
    else{
        node->code = node->parent->code + "1";
    }



    if(node->left == NULL && node->right == NULL)
        table[node->id] = node->code;
    else
    {
        if(node->left != NULL)
            create_map_table(node->left, true);
        if(node->right != NULL)
            create_map_table(node->right, false);
    }
}

bool do_compress()
{
    int length,i,j;
    char inChar;
    unsigned char outChar,tmpChar;
    string code,outString;
    map<int,string>::iterator tableIterator;

    outFile<<nodeSize<<endl;//先写入字符数（叶子节点）

    //将哈夫曼树写进文件
    for(tableIterator = table.begin(); tableIterator != table.end(); ++tableIterator)
    {
        outFile<<tableIterator->first<<" "<<tableIterator->second<<endl;
    }

    inFile.clear();
    inFile.seekg(ios::beg);

    code.clear();

    while(!inFile.eof())
    {
        inFile.get(inChar);
        if(inFile.eof())
            break;
        tableIterator = table.find((unsigned char)inChar);
        if(tableIterator!=table.end())
        {
            code += tableIterator->second;
        }
        else
        {
            qDebug()<<"Can't find the huffman code of character:"<<inChar;

            return false;
        }

        length = code.length();

        if(length>WRITE_BUFF_SIZE)
        {
            outString.clear();
            for(i = 0; i+7<length; i+=8)
            {
                outChar = 0;
                for(j = 0; j < 8; j++)
                {
                    if('0' == code[i+j])
                        tmpChar = 0;
                    else
                        tmpChar = 1;
                    outChar += tmpChar<<(7-j);
                }
                outString += outChar;
            }
            outFile<<outString;
            code = code.substr(i, length-i);

        }
    }
    tableIterator = table.find(PSEUDO_EOF);
    if(tableIterator != table.end())
        code += tableIterator->second;
    else
    {
        qDebug()<<"Can't find the huffman code of pseudo-EOF\n";

        return false;
    }
    length = code.length();
    outChar = 0;
    for(i = 0; i < length; i++)
    {
        if('0' == code[i])
            tmpChar = 0;
        else
            tmpChar = 1;
        outChar += tmpChar<<(7-(i%8));
        if(0==(i+1)%8 || i==length-1)
        {
            outFile<<outChar;
            outChar = 0;
        }
    }
    return true;
}


bool decompressHuff(string inFileName, string outFileName)//解压缩文件
{
    bool flag = false;
    //打开需要压缩的文件
    inFile.open(inFileName.c_str(),ios::in);
    if(!inFile)
    {
        qDebug()<<"Open file error,path is :"<<inFileName.c_str();

        return false;
    }
    //打开压缩结果的文件
    outFile.open(outFileName.c_str(),ios::out);
    if(!outFile)
    {
        qDebug()<<"Open file error,path is :"<<outFileName.c_str();
        return false;
    }
    flag = rebuid_huffman_tree();
    if(!flag)
    {
        cout<<"重建哈夫曼树出错"<<endl;
        return false;
    }
    decode_huffman();


    inFile.close();
    outFile.close();

    return true;
}



bool rebuid_huffman_tree()
{
    int i,j,id,length;
    string code;
    Node *node,*tmp,*childNode;

    root = new Node;
    root->left = NULL;
    root->right = NULL;
    root->parent = NULL;
    inFile>>nodeSize;
    if(nodeSize>MAX_SIZE)
    {
        qDebug()<<"The number of nodes is not valid.maybe the compressed file has been broken. ";

        return false;
    }
    for(i = 0; i<nodeSize; ++i)
    {
        inFile >> id;
        inFile >> code;
        length = code.length();
        node = root;
        for(j = 0; j < length; ++j)
        {
            if('0'==code[j])
                tmp = node->left;
            else if('1'==code[j])
                tmp = node->right;
            else
            {
                qDebug()<<"Decode error,huffman code is not made up with 0 or 1 ";

                return false;
            }
            if(tmp == NULL)
            {
                childNode = new Node;
                childNode->left = NULL;
                childNode->right = NULL;
                childNode->parent = node;

                if(j == length - 1)
                {
                    childNode->id = id;
                    childNode->code = code;
                }
                if('0' == code[j])
                    node->left = childNode;
                else
                    node->right = childNode;
                tmp = childNode;
            }
            else if(j==length-1)
            {
                qDebug()<<"Huffman code is not valid,maybe the compress file has been broken. ";

                return false;
            }
            else if (tmp->left==NULL&&tmp->right==NULL)
            {
                qDebug()<<"Huffman code is not valid,maybe the compressed file has been broken.";

                return false;
            }
            node = tmp;
        }

    }
    return true;
}

void decode_huffman()
{
    bool pseudo_eof;
    int i,id;
    char inChar;
    string outString;
    unsigned char uChar,flag;
    Node *node;

    outString.clear();
    node = root;
    pseudo_eof = false;
    inFile.get(inChar);
    while(!inFile.eof())
    {

        inFile.get(inChar);
        uChar = (unsigned char) inChar;
        flag = 0x80;
        for(i = 0; i < 8; ++i)
        {
            if(uChar&flag)
                node = node->right;
            else
                node = node->left;
            if(node->left == NULL && node->right==NULL)
            {
                id = node->id;
                if(id == PSEUDO_EOF)
                {
                    pseudo_eof = true;
                    break;
                }
                else
                {
                    outString += (char)node->id;
                    node = root;
                }
            }
            flag = flag>>1;
        }
        if(pseudo_eof)
            break;
        if(WRITE_BUFF_SIZE < outString.length())
        {
            outFile<<outString;
            outString.clear();
        }
    }
    if(!outString.empty())
        outFile<<outString;
}




#endif // HUFFMAN_H
