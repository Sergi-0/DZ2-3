#include <iostream>
#include <fstream>
#include <string>
#include <list>
using namespace std;

struct Node {
    Node* left;
    Node* right;
    int data;
    Node* prev;
};

class BSTree {
public:
    BSTree(); // конструктор по умолчанию
    BSTree(initializer_list<int> list); // конструктор с параметром
    bool add_element(int value); // функция добавления
    bool delete_element(int value); // функция удаления
    bool find_element(int value); // функция поиска элемента
    void print(); // функция вывода дерева в консоль
    bool save_to_file(const string& path); // функция сохранения в файл
    void load_from_file(const string& path); // функция загрузки из файла
    ~BSTree(); //деструктор
private:
    Node* root;
};

BSTree::BSTree() {
    root = new Node;
    root->data = 0;
    root->left = nullptr;
    root->right = nullptr;
    root->prev = nullptr;
}

BSTree::BSTree(initializer_list<int> list) {

    int n = 1;

    if (list.size() > 0) {

        root = new Node;
        root->data = *list.begin();
        root->left = nullptr;
        root->right = nullptr;
        root->prev = nullptr;

        if (list.size() > n) {

            while (list.size() > n) {

                Node* p = root;
                Node* c = nullptr;

                while (p != nullptr) {
                    c = p;
                    if (*(list.begin() + n) > p->data) p = p->right;
                    else if(*(list.begin() + n) < p->data) p = p->left;
                }

                if (*(list.begin() + n) > c->data) {
                    c->right = new Node;
                    c->right->data = *(list.begin() + n);
                    c->right->prev = c;
                    c->right->left = nullptr;
                    c->right->right = nullptr;
                }

                else {
                    c->left = new Node;
                    c->left->data = *(list.begin() + n);
                    c->left->prev = c;
                    c->left->left = nullptr;
                    c->left->right = nullptr;
                }

                ++n;
            }
        }
    }

    else cout << "Empty list!";
}

bool BSTree::add_element(int value) {
    Node* p = root;
    Node* c = p;

    if (this->find_element(value) != 0) { cout << "Такой элемент уже есть!" << endl << endl; return false; }
    if (root->data == 0 && root->left == nullptr && root->right == nullptr) { root->data = value; return true; }

    while (p != nullptr) {
        c = p;
        if (value > p->data) p = p->right;
        else if (value < p->data) p = p->left;
    }

    if (value > c->data) {
        c->right = new Node;
        c->right->data = value;
        c->right->prev = c;
        c->right->left = nullptr;
        c->right->right = nullptr;
    }

    else {
        c->left = new Node;
        c->left->data = value;
        c->left->prev = c;
        c->left->left = nullptr;
        c->left->right = nullptr;
    }

    return true;
}

bool BSTree::delete_element(int value) { // 1,2 эл, неп.в

    Node* p = root;
    Node* c = p;

    while (p->data != value) {
        c = p;
        if (value > p->data) p = p->right;
        else if (value < p->data) p = p->left;
        if (p == nullptr) return false;
    }

    if (p->right == nullptr && p->left == nullptr) {

        if (p == root) {
            delete root;
            root = nullptr;
            return true;
        }

        else {
            if (c->right == p) c->right = nullptr;
            else c->left = nullptr;
            delete p;
            return true;
        }
    }

    if (p->left == nullptr && p->right == nullptr) {
        if (p->left == nullptr) {
            if (c->right == p) {
                c->right = p->right;
                p->right->prev = c;
                
            }
            else {
                c->left = p->right;
                p->right->prev = c;
            }
            delete p;
            return true;
        }
        else {
            if (c->right == p) {
                c->right = p->left;
                p->left->prev = c;
            }
            else {
                c->left = p->left;
                p->left->prev = c;
            }
            delete p;
            return true;
        }
    }

    if (p->left != nullptr && p->right != nullptr) {
        p = p->left;
        Node* min = p;
        while (p != nullptr) {
            min = p;
            p = p->left;
        }

        if (value > c->data) {
            c->right->data = min->data;
            min->prev->left = nullptr;
        }

        else {
            c->left->data = min->data;
            min->prev->left = nullptr;
        }
        delete min;
        return true;
    }
    return false;
}

bool BSTree::find_element(int value) {

    if (root->data == value) return true;

    Node* p = root;
    Node* c = p;

    while (p->data != value) {
        c = p;
        if (value > p->data) p = p->right;
        else if (value < p->data) p = p->left;
        if (p == nullptr) return false;
    }

   return true;
}

void print1(Node* a, ostream& m) {

    if (a == nullptr) return;
    else {
        print1(a->left, m);
        m << a->data << " ";
        print1(a->right, m);
    }
}

void BSTree::print() {
    print1(root, cout);
}

bool BSTree::save_to_file(const string& path) { // функция записи в файл
    ofstream file(path);
    print1(root, file);
    return true;
}

void BSTree::load_from_file(const string& path) { // функция загрузки из файла
    ifstream file(path);
    string a;
    list<int> b;
    int k = 0;
    while (getline(file, a, ' ')) b.push_back(atoi(a.c_str()));
    root->data = *b.begin();
    for (const auto& i : b) {
        ++k;
        if (k == 1) continue;
        this->add_element(i);  
    }
}

void del_tr(Node* nd) {
    if (nd != nullptr) {
        del_tr(nd->left);
        del_tr(nd->right);
        delete nd;
    }
}

BSTree::~BSTree() {
    del_tr(root);
}

int main()
{
    setlocale(LC_ALL, "Rus");
    BSTree a({6,3,8,2,4,7,9,1}); // нельзя вводить одинаковые элементы в лист
    a.add_element(67);
    a.delete_element(3);
    cout << a.find_element(68) << endl;;
    a.print();
    a.save_to_file("output.txt");
    cout << endl;
    BSTree b;
    b.load_from_file("input.txt");
    b.print();

    cout << endl << endl;

    BSTree test;
    int t, test1;
    t = 5;
    while (t != 0) {
        cout << "Выберите команду: 1 - добавить элемент, 2 - удалить элемент, 3 - найти элемент, 4 - напечатать дерево, 0 - выход. Ваш выбор: ";
        cin >> t;
        cout << endl;

        if (t == 1) {
            cout << "Введите элемент: ";
            cin >> test1;
            cout << endl;
            test.add_element(test1);
        }

        if (t == 2) {
            cout << "Введите элемент: ";
            cin >> test1;
            cout << endl;
            test.delete_element(test1);
        }

        if (t == 3) {
            cout << "Введите элемент: ";
            cin >> test1;
            cout << endl;
            cout << test.find_element(test1) << endl;
            cout << endl;
        }

        if (t == 4) { test.print(); cout << endl;}
        if (t == 0) break;
    }
}
