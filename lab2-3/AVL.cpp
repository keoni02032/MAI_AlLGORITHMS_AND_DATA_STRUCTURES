#include <iostream>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <stdint.h>

char NODE = 'y';
char END = 'n';

const size_t LEN_KEY = 256;

struct TData {
    char* key;
    unsigned long long value;
};

struct TNode {
    uint8_t height;
    TData* data;
    TNode* left;
    TNode* right;
};

class TTree {
public:

    TTree() {
        root = nullptr;
    }
    
    void Insert(TData data) {

        if (root == nullptr) {
            root = CreateNode(data);
        } else {
            int cmpRes = strcmp(root->data->key, data.key);
            if (cmpRes == 0) {
            } else if (cmpRes > 0) {
                root->left = InsertSub(root->left, data);
            } else {
                root->right = InsertSub(root->right, data);
            }
            root = Balance(root);
        }
    }

    void Delete(TData data) {
        if (root == nullptr) {
        } else {
            int cmpRes = strcmp(root->data->key, data.key);
            if (cmpRes > 0) {
                root->left = DeleteSub(root->left, data);
            } else if (cmpRes < 0) {
                root->right = DeleteSub(root->right, data);
            } else {
                TNode* l = root->left;
                TNode* r = root->right;
                free(root->data->key);
                free(root->data);
                free(root);
                root = nullptr;
                if (r == nullptr) {
                    root = l;
                    return;
                }
                TNode* min = Min(r);
                min->right = DelMin(r);
                min->left = l;
                root = Balance(min);
            }
            root = Balance(root);
        }
    }

    bool Search(TData &data) {
        TNode* node = root;
        if (node == nullptr) {
            return false;
        } else {
            int cmpRes = strcmp(node->data->key, data.key);
            if (cmpRes > 0) {
                if (!SearchSub(node->left, data)) {
                    return false;
                }
            } else if (cmpRes < 0) {
                if (!SearchSub(node->right, data)) {
                    return false;
                }
            } else {
                data.value = node->data->value;
                return true;
            }
        }
        return true;
    }
    
    bool Save(const char* filename) {

        FILE* f = fopen(filename, "wb");

        if (f == nullptr) {
            return false;
        }

        if (root == nullptr) {
            if (fwrite(&END, sizeof(char), 1, f) != 1) {
                return false;
            }
            return true;
        }

        bool ser = SaveSub(f, root);
        fclose(f);
        return ser;
    }

    bool Load(const char* filename) {

        FILE* f = fopen(filename, "rb");

        if (f == nullptr) {
            fclose(f);
            return false;
        }

        if (root) {
            DeleteTree(root);
        }

        bool deSer = LoadSub(f, root);
        fclose(f);
        return deSer;
    }

    ~TTree() {
        DeleteTree();
    }
private:

    TNode *root;
    TNode* CreateNode(TData data) {
        TNode* node = (TNode*) malloc(sizeof(TNode));
        node->data = (TData*) malloc(sizeof(TData));
        node->data->key = (char*) malloc(sizeof(char) * (strlen(data.key) + 1));
        strcpy(node->data->key, data.key);
        node->data->value = data.value;
        node->height = 1;
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }

    // void DeleteNode(TNode *&node);
    TNode* InsertSub(TNode* node, TData data) {

        if (node == nullptr) {
            node = CreateNode(data);
        } else {
            int cmpRes = strcmp(node->data->key, data.key);
            if (cmpRes == 0) {
            } else if (cmpRes > 0) {
                node->left = InsertSub(node->left, data);
            } else {
                node->right = InsertSub(node->right, data);
            }
            node = Balance(node);
        }
        return node;
    }

    TNode* Balance(TNode* node) {

        SetHeight(node);
        if (BalFact(node) == 2) {
            if (BalFact(node->right) < 0) {
                node->right = RotateRight(node->right);
            }
            node = RotateLeft(node);
        } else if (BalFact(node) == -2) {
            if (BalFact(node->left) > 0) {
                node->left = RotateLeft(node->left);
            }
            node = RotateRight(node);
        }
        return node;
    }

    TNode* RotateLeft(TNode* q) {
        TNode* p = q->right;
        q->right = p->left;
        p->left = q;
        SetHeight(q);
        SetHeight(p);
        return p;
    }

    TNode* RotateRight(TNode* p) {
        TNode* q = p->left;
        p->left = q->right;
        q->right = p;
        SetHeight(p);
        SetHeight(q);
        return q;
    }

    TNode* Min(TNode* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    
    void SetHeight(TNode* node) {
        uint8_t hl = GetHeight(node->left);
        uint8_t hr = GetHeight(node->right);
        if (hl > hr) {
            node->height = hl + 1;
        } else {
            node->height = hr + 1;
        }
    }
    
    TNode* DeleteSub(TNode* node, TData data) {
        if (node == nullptr) {
        } else {
            int cmpRes = strcmp(node->data->key, data.key);
            if (cmpRes > 0) {
                node->left = DeleteSub(node->left, data);
            } else if (cmpRes < 0) {
                node->right = DeleteSub(node->right, data);
            } else {
                TNode* l = node->left;
                TNode* r = node->right;
                free(node->data->key);
                free(node->data);
                free(node);
                node = nullptr;
                if (r == nullptr) {
                    return l;
                }
                TNode* min = Min(r);
                min->right = DelMin(r);
                min->left = l;
                return Balance(min);
            }
        }
        return Balance(node);
    }

    TNode* DelMin(TNode* node) {
        if (node->left == nullptr)
            return node->right;
        node->left = DelMin(node->left);
        return Balance(node);
    }

    int BalFact(TNode* node) {
        return GetHeight(node->right) - GetHeight(node->left);
    }

    int GetHeight(TNode* node) {
        if (node == nullptr) {
            return 0;
        } else {
            return node->height;
        }
    }
    
    bool SearchSub(TNode*&node, TData &data) {
        if (node == nullptr) {
            return false;
        } else {
            int cmpRes = strcmp(node->data->key, data.key);
            if (cmpRes > 0) {
                if (!SearchSub(node->left, data)) {
                    return false;
                }
            } else if (cmpRes < 0) {
                if (!SearchSub(node->right, data)) {
                    return false;
                }
            } else {
                data.value = node->data->value;
                return true;
            }

        }
        return true;
    }

    bool SaveSub(FILE *f, TNode *node) {
        
        if (node == nullptr) {
            if (fwrite(&END, sizeof(char), 1, f) != 1) {
                return false;
            }
            return true;
        }

        size_t length = strlen(node->data->key);
        fwrite(&NODE, sizeof(char), 1, f);
        fwrite(&length, sizeof(length), 1, f);
        fwrite(node->data->key, sizeof(char), length, f);
        fwrite(&node->data->value, sizeof(node->data->value), 1, f);
        fwrite(&node->height, sizeof(node->height), 1, f);

        if (!SaveSub(f, node->left)) {
            return false;
        }

        if (!SaveSub(f, node->right)) {

            return false;
        }
        return true;
    }

    bool LoadSub(FILE *f, TNode *&node) {

        char mark;
        size_t res = fread(&mark, sizeof(char), 1, f);

        if (mark == NODE) {

            TData data;
            size_t length;
            uint8_t height;

            res = fread(&length, sizeof(length), 1, f);
            data.key = (char*) malloc(sizeof(char) * (length + 1));
            res = fread(data.key, sizeof(char), length, f);
            data.key[length] = '\0';
            res = fread(&data.value, sizeof(data.value), 1, f);
            res = fread(&height, sizeof(height), 1, f);
            for (int i = 1; i < res; i *= 1000) {}

            node = CreateNode(data);
            node->height = height;

            free(data.key);
        } else {
            return true;
        }

        if (!LoadSub(f, node->left)) {
            return false;
        }

        if (!LoadSub(f, node->right)) {

            return false;
        }

        return true;
    }

    void DeleteTree(TNode*&node) {

        if (node->left) {
            DeleteTree(node->left);
        }

        if (node->right) {
            DeleteTree(node->right);
        }

        free(node->data->key);
        free(node->data);
        free(node);
        node = nullptr;
    }

    void DeleteTree() {
        if (root) {
            DeleteTree(root);
        }
    }

};

char ToLower(char ch) {
    return ch >= 'A' && ch <= 'Z' ? ch - 'A' + 'a' : ch;
}

bool IsLetter(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

void GetKey(char key[]);
void GetVal(unsigned long long &val);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    TTree tree;

    while (true) {

        TData data;
        char key[257];
        char keyForSearch[257];
        unsigned long long val;
        char mod, action;
        int idx;

        do {

            action = getchar();
        }
        while(action == '\n' || action == ' ');
        


        if (action == EOF) {
            return 0;
        }

        switch (action) {
            case '+':

                getchar();

                GetKey(key);
                GetVal(val);

                data.key = new char[strlen(key) + 1];
                strcpy(data.key, key);
                data.value = val;

                if (!tree.Search(data)) {
                    tree.Insert(data);
                    std::cout << "OK" << std::endl;
                } else {
                    std::cout << "Exist" << std::endl;
                }

                delete[] data.key;

                break;
            case '-':

                getchar();

                GetKey(key);

                data.key = new char[strlen(key) + 1];
                strcpy(data.key, key);

                if (!tree.Search(data)) {
                    std::cout << "NoSuchWord" << std::endl;
                } else {
                    tree.Delete(data);
                    std::cout << "OK" << std::endl;
                }
                delete[] data.key;
                break;
            case '!':

                getchar();

                GetKey(key);
                mod = key[0];

                idx = 0;

                while ((action = getchar()) != '\n')
                {
                    key[idx++] = action;
                }

                key[idx] = '\0';

                if (mod == 's') {

                    if (tree.Save(key)) {
                        std::cout << "OK" << std::endl;
                    }
                    else {
                        std::cout << "ERROR: Couldn't create file" << std::endl;
                    }
                }
                else {
                    if (tree.Load(key)) {
                        std::cout << "OK" << std::endl;
                    }
                    else {
                        std::cout << "ERROR: Couldn't load file" << std::endl;

                    }
                }

                break;

            default:

                keyForSearch[0] = ToLower(action);
                keyForSearch[1] = '\0';

                GetKey(key);
                strcat(keyForSearch, key);
                data.key = new char[strlen(keyForSearch) + 1];
                strcpy(data.key, keyForSearch);

                if (!tree.Search(data)) {
                    std::cout << "NoSuchWord" << std::endl;
                }
                else {
                    std::cout << "OK: " << data.value << std::endl;
                }
                delete[] data.key;
                break;
        }
    }
    return 0;
}


void GetKey(char key[]) {

    char ch;
    int idx = 0;


    while (true) {
        ch = ToLower(getchar());

        if (!IsLetter(ch)) {
            break;
        }

        key[idx++] = ch;
    }
    key[idx] = '\0';
}

void GetVal(unsigned long long &val) {
    char ch;
    val = 0;
    while ((ch = getchar()) != '\n') {
        val = val * 10 + ch - '0';
    }
}