#include <iostream>
#include <vector>
#include <list>
using namespace std;

const int NKNAPSACKCAP = 10;
class Goods //定义货物数据类型
{
public:
    int weight;
    int value;
    friend ostream& operator<<(ostream &os, const Goods &out);
};

ostream& operator<<(ostream &os, const Goods &out)
{
    os << "重量：" << out.weight << " 价值： " << out.value;
    return os;
}
typedef vector<Goods> AllGoods;//定义所有货物数据类型

class Node
{
public:
    Node *parent;
    int nWeight;
    int nValue;
    int id;
    bool leftChild;
    Node(Node *_parent, bool _left, int weight, int value, int _id) :parent(_parent), leftChild(_left), nWeight(weight), nValue(value), id(_id)
    {}

    ~Node()
    {
    }
};

class Knapsack
{
private:
    int capacity;//背包容量
    int nGoodsNum;//物品数
    vector<Goods> goods;
    int nMaxValue;
    int nCurrentWeight;
    int nCurrentValue;
    vector<bool> bestResult;

    int bound(int i)
    {
        int nLeftCapacity = capacity - nCurrentWeight;
        int tempMaxValue = nCurrentValue;

        while (i < nGoodsNum && goods[i].weight <= nLeftCapacity)
        {
            nLeftCapacity -= goods[i].weight;
            tempMaxValue += goods[i].value;
        }

        if (i < nGoodsNum)
        {
            tempMaxValue += (float)(goods[i].value) / goods[i].weight * nLeftCapacity;
        }

        return tempMaxValue;
    }
public:
    Knapsack(AllGoods &AllGoods, int nKnapsackCap)
    {
        nGoodsNum = AllGoods.size();
        capacity = nKnapsackCap;
        nCurrentWeight = 0;
        nCurrentValue = 0;
        nMaxValue = 0;

        for (int i = 0; i < nGoodsNum; ++i)
        {
            goods.push_back(AllGoods[i]);
            bestResult.push_back(false);
        }
    }

    void sortByUintValue()
    {
        stable_sort(goods.begin(), goods.end(), [](const Goods& left, const Goods& right)
        {return (left.value * right.weight > left.weight * right.value); });
    }

    void printGoods()
    {
        for (size_t i = 0; i < goods.size(); ++i)
        {
            cout << goods[i] << endl;
        }
    }

    void printResult()
    {
        cout << "MAX VALUE: " << nMaxValue << endl;
        for (int i = 0; i < nGoodsNum; ++i)
        {
            if (bestResult[i])
            {
                cout << goods[i] << endl;
            }
        }
    }

    void knapsack_0_1_branchAndBound()
    {
        list<Node*> activeNodes;
        list<Node*> diedNodes;
        sortByUintValue();

        //判断第一个物品是否可放入背包
        if (goods[0].weight < capacity)
        {
            activeNodes.push_back(new Node(nullptr, true, goods[0].weight, goods[0].value,0));
        }
        activeNodes.push_back(new Node(nullptr, false, 0,0,0));

        Node *curNode = nullptr;
        Node *preNode = nullptr;
        int curId;
        while (!activeNodes.empty())
        {
            //取出队列中最靠前的节点
            curNode = activeNodes.front();
            activeNodes.pop_front();

            diedNodes.push_back(curNode);//放入死节点队列

            if (curNode->id + 1 == nGoodsNum)//如果当前出队节点为最低层节点
            {
                continue;
            }

            if (nMaxValue < curNode->nValue)//若此节点物品价值大于背包中最大物品价值
            {//将背包中物品替换为当前节点所表示物品
                nMaxValue = curNode->nValue;
                preNode = curNode;
                while (nullptr != preNode)
                {
                    bestResult[preNode->id] = preNode->leftChild;
                    preNode = preNode->parent;
                }
            }

            nCurrentValue = curNode->nValue;
            nCurrentWeight = curNode->nWeight;
            curId = curNode->id;
            if (nMaxValue >= bound(curId + 1))//剪枝
            {
                continue;
            }

            //判断下个物品是否可加入队列
            if (nCurrentWeight + goods[curId + 1].weight <= capacity)
            {
                activeNodes.push_back(new Node(curNode, true, nCurrentWeight + goods[curId + 1].weight, nCurrentValue + goods[curId + 1].value, curId + 1));
            }

            activeNodes.push_back(new Node(curNode, false, nCurrentWeight,nCurrentValue,curId + 1));
        }

        while (!diedNodes.empty())
        {
            curNode = diedNodes.front();
            delete curNode;
            diedNodes.pop_front();
        }
    }
};

//获取物品信息，此处只是将书上例子输入allGoods
void GetAllGoods(AllGoods &allGoods)
{
    Goods goods;

    goods.weight = 2;
    goods.value = 6;
    allGoods.push_back(goods);

    goods.weight = 2;
    goods.value = 3;
    allGoods.push_back(goods);

    goods.weight = 2;
    goods.value = 8;
    allGoods.push_back(goods);

    goods.weight = 6;
    goods.value = 5;
    allGoods.push_back(goods);

    goods.weight = 4;
    goods.value = 6;
    allGoods.push_back(goods);

    goods.weight = 5;
    goods.value = 4;
    allGoods.push_back(goods);
}

int main()
{
    AllGoods allGoods;
    GetAllGoods(allGoods); //要求按照单位物品价值由大到小排序
    Knapsack knap(allGoods,NKNAPSACKCAP);
    knap.printGoods();
    knap.knapsack_0_1_branchAndBound();
    knap.printResult();
    return 0;
}