#include <iostream>
#include <cstdlib>

struct KV {
    unsigned long long key;
    char value[256]; 
};

template <class T>
struct vector {
    T* body;
    size_t size;
    size_t CAP;
};

template <class T>
void v_create(vector<T> &v, size_t size)
{
    v.body = new T[size + 1];
    v.CAP = size + 1;
    v.size = size;
}

template <class T>
void v_delete(vector<T> &v)
{
    delete[] v.body;
    v.body = NULL;
    v.size = 0;
    v.CAP = 0;
}

template <class T>
void VectorSwap(T &v1,T &v2)
{
    T v = v1;
    v1 = v2;
    v2 = v;
}

template<class T>
void VectorPushBack(vector<T> &v, T val)
{
    if (v.size == v.CAP) {
        v.CAP *= 2;
        T* v2 = new T[v.CAP];
        
        for (size_t i = 0; i < v.size; ++i) {
            v2[i] = v.body[i];
        }
        
        delete[] v.body;

        v.body = v2;
    }

    v.body[v.size++] = val;
}

template <class T>
void CountingSort(vector<T> &v, unsigned long long div) {
    if (v.size < 2) {
        return;
    }

    vector<KV> result;
    v_create<KV>(result, v.size);

    unsigned long long temp[10] = {0};

    for (size_t i = 0; i < v.size; i++) {
        temp[(v.body[i].key / div) % 10]++;
    }

    for (int i = 1; i < 10; i++) {
        temp[i] += temp[i - 1];
    }

    for (size_t i = v.size; i > 0; --i) {
        result.body[temp[(v.body[i - 1].key / div) % 10] - 1] = v.body[i - 1];
        temp[(v.body[i - 1].key / div) % 10]--;
    }

    VectorSwap(v, result);
    v_delete<KV>(result);
}

template <class T>
void radixsort(vector<T> &v, unsigned long long max)
{
    for (unsigned long long exp = 1; max / exp > 0; exp *= 10) {
        CountingSort(v, exp);
    }
}

bool goodval(char a) {
    if (a >= '0' && a <= '9') {
        return true;
    }
    return false;
}

void ValueToKey(KV &data) {
    for (unsigned long long j = 0; data.value[j] != '\t'; ++j) {
        if (goodval(data.value[j])) {
            data.key = data.key * 10 + data.value[j] - '0';
        }
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    KV data;

    unsigned long long max = 0;
    vector<KV> v;
    v_create<KV>(v, 0);

    while (std::cin.getline(data.value, 256)) {
        data.key = 0;

        if (data.value[0] == '\0') {
            continue;
        }

        ValueToKey(data);
        if (data.key > max) {
            max = data.key;
        }
        VectorPushBack<KV>(v, data);
    }
    radixsort(v, max);
    for (size_t i = 0; i < v.size; i++) {
        std::cout << v.body[i].value << std::endl;
    }

    v_delete<KV>(v);
    return 0;
}