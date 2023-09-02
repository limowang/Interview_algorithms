#include <iostream>

using namespace std;

struct Node
{
    int value;

    Node* next;
    Node(int x) : value(x),next(nullptr) {}
};

//��תָ����Χ�ڵĽڵ�
pair<Node*,Node*> rev(Node* head,Node* tail)
{
    Node* tH = tail;
    Node* tT = head;
    Node* cmp = tail -> next;
    Node* pre = head;
    Node* cur = head -> next;
    head -> next = nullptr;

    while(cur != cmp)
    {
        Node* next = cur -> next;
        cur -> next = pre;
        pre = cur;
        cur = next;
    }

    return {tH,tT};
}

Node* KReverse(Node* head,int k)
{
    int K = k;
    Node* cur = head;
    while(cur != nullptr && --k > 0)
    {
        cur = cur -> next;
    }

    //����ڵ�������K��
    // if(cur == nullptr)
    //     return head;
    if(k > 0)
        return head;

    
    if(cur -> next == nullptr)
    {
        auto it = rev(head,cur);
        return it.first;
    }
    //Node* next = KReverse(cur -> next,k);   --------��������k�����˱仯
    Node* next = KReverse(cur -> next, K);
    auto it = rev(head,cur);
    it.second -> next = next;
    
    return it.first;
}


void printL(Node *l){
    Node *p=l;
    while(p){
        cout<<p->value<<' ';
        p = p->next;
    }
    cout<<'\n';
}

int main()
{
    Node* head = new Node(1);
    head -> next = new Node(2);
    head -> next -> next = new Node(3);
    head -> next -> next -> next = new Node(4);
    head -> next -> next -> next -> next = new Node(5);
    //head -> next -> next -> next -> next -> next = nullptr;

    // auto it = rev(head, head -> next -> next -> next);
    // Node* cur = it.first;
    // printL(cur);

    Node* res = KReverse(head,2);
    while(res != nullptr)
    {
        cout << res -> value << endl;
        res = res -> next;
    }

    system("pause");
    return 0;
}