#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_set>
#include <cstdint>
#define MAXN 13

using namespace std;

int N;

struct Node {
    int board[MAXN][MAXN];
    double f, g, h; // f = g + h 
    Node* parent = nullptr;

    Node(int b[MAXN][MAXN], Node* p, double g_val) : parent(p), g(g_val) {
        h = 0.0;
        int cnt = 0;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) {
                board[i][j] = b[i][j];
                if (b[i][j]) cnt++;
            }
        h = double(ceil((int)h / 3) + 1);
        if (tmp % 3) h = h - 1.0;
        // h = double(ceil((int)h * 6));
        f = g + h;
    }
};

uint64_t hash_board(const int board[MAXN][MAXN]) {
    uint64_t hash = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            hash = (hash << 1) | board[i][j];
    return hash;
}

unordered_set<uint64_t> visited;

bool Is_result(Node* node) {
    bool flag = 1;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            if (node->board[i][j]) {
                return false;
            }
        }
    return true;
}

void Print_result() {
    cout << "yes" << endl;
}

vector<Node*> get_neighbors(Node* node) {
    vector<Node*> tmp;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            int new_board[MAXN][MAXN];
            // 第一种 L
            if (i >= 1 && j <= N - 2) {
                for (int k = 0; k < N; k++)
                    for (int l = 0; l < N; l++) {
                        new_board[k][l] = node->board[k][l];
                    }
                if (new_board[i - 1][j] && new_board[i][j] && new_board[i][j + 1]) {
                    new_board[i - 1][j] = 1 - new_board[i - 1][j];
                    new_board[i][j] = 1 - new_board[i][j];
                    new_board[i][j + 1] = 1 - new_board[i][j + 1];
                    uint64_t neighbor_hash = hash_board(new_board);
                    if (visited.find(neighbor_hash) == visited.end()) {
                        tmp.push_back(new Node(new_board, node, node->g + 1));
                        visited.insert(neighbor_hash);
                    }
                }
            }
            if (tmp.size() > 0) return tmp;
            // 第二种 L
            if (i <= N - 2 && j <= N - 2) {
                for (int k = 0; k < N; k++)
                    for (int l = 0; l < N; l++) {
                        new_board[k][l] = node->board[k][l];
                    }
                if (new_board[i + 1][j] && new_board[i][j] && new_board[i][j + 1]) {
                    new_board[i + 1][j] = 1 - new_board[i + 1][j];
                    new_board[i][j] = 1 - new_board[i][j];
                    new_board[i][j + 1] = 1 - new_board[i][j + 1];
                    uint64_t neighbor_hash = hash_board(new_board);
                    if (visited.find(neighbor_hash) == visited.end()) {
                        tmp.push_back(new Node(new_board, node, node->g + 1));
                        visited.insert(neighbor_hash);
                    }
                }
            }
            if (tmp.size() > 0) return tmp;
            // 第三种 L
            if (i <= N - 2 && j >= 1) {
                for (int k = 0; k < N; k++)
                    for (int l = 0; l < N; l++) {
                        new_board[k][l] = node->board[k][l];
                    }
                if (new_board[i + 1][j] && new_board[i][j] && new_board[i][j - 1]) {
                    new_board[i + 1][j] = 1 - new_board[i + 1][j];
                    new_board[i][j] = 1 - new_board[i][j];
                    new_board[i][j - 1] = 1 - new_board[i][j - 1];
                    uint64_t neighbor_hash = hash_board(new_board);
                    if (visited.find(neighbor_hash) == visited.end()) {
                        tmp.push_back(new Node(new_board, node, node->g + 1));
                        visited.insert(neighbor_hash);
                    }
                }
            }
            if (tmp.size() > 0) return tmp;
            // 第四种 L
            if (i >= 1 && j >= 1) {
                for (int k = 0; k < N; k++)
                    for (int l = 0; l < N; l++) {
                        new_board[k][l] = node->board[k][l];
                    }
                if (new_board[i - 1][j] && new_board[i][j] && new_board[i][j - 1]) {
                    new_board[i - 1][j] = 1 - new_board[i - 1][j];
                    new_board[i][j] = 1 - new_board[i][j];
                    new_board[i][j - 1] = 1 - new_board[i][j - 1];
                    uint64_t neighbor_hash = hash_board(new_board);
                    if (visited.find(neighbor_hash) == visited.end()) {
                        tmp.push_back(new Node(new_board, node, node->g + 1));
                        visited.insert(neighbor_hash);
                    }
                }
            }
            if (tmp.size() > 0) return tmp;
        }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            int new_board[MAXN][MAXN];
            // 第一种 L
            if (i >= 1 && j <= N - 2) {
                for (int k = 0; k < N; k++)
                    for (int l = 0; l < N; l++) {
                        new_board[k][l] = node->board[k][l];
                    }
                if (new_board[i - 1][j] + new_board[i][j] + new_board[i][j + 1] == 2) {
                    new_board[i - 1][j] = 1 - new_board[i - 1][j];
                    new_board[i][j] = 1 - new_board[i][j];
                    new_board[i][j + 1] = 1 - new_board[i][j + 1];
                    uint64_t neighbor_hash = hash_board(new_board);
                    if (visited.find(neighbor_hash) == visited.end()) {
                        tmp.push_back(new Node(new_board, node, node->g + 1));
                        visited.insert(neighbor_hash);
                    }
                }
            }
            if (tmp.size() > 0) return tmp;
            // 第二种 L
            if (i <= N - 2 && j <= N - 2) {
                for (int k = 0; k < N; k++)
                    for (int l = 0; l < N; l++) {
                        new_board[k][l] = node->board[k][l];
                    }
                if (new_board[i + 1][j] + new_board[i][j] + new_board[i][j + 1] == 2) {
                    new_board[i + 1][j] = 1 - new_board[i + 1][j];
                    new_board[i][j] = 1 - new_board[i][j];
                    new_board[i][j + 1] = 1 - new_board[i][j + 1];
                    uint64_t neighbor_hash = hash_board(new_board);
                    if (visited.find(neighbor_hash) == visited.end()) {
                        tmp.push_back(new Node(new_board, node, node->g + 1));
                        visited.insert(neighbor_hash);
                    }
                }
            }
            if (tmp.size() > 0) return tmp;
            // 第三种 L
            if (i <= N - 2 && j >= 1) {
                for (int k = 0; k < N; k++)
                    for (int l = 0; l < N; l++) {
                        new_board[k][l] = node->board[k][l];
                    }
                if (new_board[i + 1][j] + new_board[i][j] + new_board[i][j - 1] == 2) {
                    new_board[i + 1][j] = 1 - new_board[i + 1][j];
                    new_board[i][j] = 1 - new_board[i][j];
                    new_board[i][j - 1] = 1 - new_board[i][j - 1];
                    uint64_t neighbor_hash = hash_board(new_board);
                    if (visited.find(neighbor_hash) == visited.end()) {
                        tmp.push_back(new Node(new_board, node, node->g + 1));
                        visited.insert(neighbor_hash);
                    }
                }
            }
            if (tmp.size() > 0) return tmp;
            // 第四种 L
            if (i >= 1 && j >= 1) {
                for (int k = 0; k < N; k++)
                    for (int l = 0; l < N; l++) {
                        new_board[k][l] = node->board[k][l];
                    }
                if (new_board[i - 1][j] + new_board[i][j] + new_board[i][j - 1] == 2) {
                    new_board[i - 1][j] = 1 - new_board[i - 1][j];
                    new_board[i][j] = 1 - new_board[i][j];
                    new_board[i][j - 1] = 1 - new_board[i][j - 1];
                    uint64_t neighbor_hash = hash_board(new_board);
                    if (visited.find(neighbor_hash) == visited.end()) {
                        tmp.push_back(new Node(new_board, node, node->g + 1));
                        visited.insert(neighbor_hash);
                    }
                }
            }
            if (tmp.size() > 0) return tmp;
        }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            int new_board[MAXN][MAXN];
            // 第一种 L
            if (i >= 1 && j <= N - 2) {
                for (int k = 0; k < N; k++)
                    for (int l = 0; l < N; l++) {
                        new_board[k][l] = node->board[k][l];
                    }
                if (new_board[i - 1][j] + new_board[i][j] + new_board[i][j + 1] == 1) {
                    new_board[i - 1][j] = 1 - new_board[i - 1][j];
                    new_board[i][j] = 1 - new_board[i][j];
                    new_board[i][j + 1] = 1 - new_board[i][j + 1];
                    uint64_t neighbor_hash = hash_board(new_board);
                    if (visited.find(neighbor_hash) == visited.end()) {
                        tmp.push_back(new Node(new_board, node, node->g + 1));
                        visited.insert(neighbor_hash);
                    }
                }
            }
            if (tmp.size() > 2) return tmp;
            // 第二种 L
            if (i <= N - 2 && j <= N - 2) {
                for (int k = 0; k < N; k++)
                    for (int l = 0; l < N; l++) {
                        new_board[k][l] = node->board[k][l];
                    }
                if (new_board[i + 1][j] + new_board[i][j] + new_board[i][j + 1] == 1) {
                    new_board[i + 1][j] = 1 - new_board[i + 1][j];
                    new_board[i][j] = 1 - new_board[i][j];
                    new_board[i][j + 1] = 1 - new_board[i][j + 1];
                    uint64_t neighbor_hash = hash_board(new_board);
                    if (visited.find(neighbor_hash) == visited.end()) {
                        tmp.push_back(new Node(new_board, node, node->g + 1));
                        visited.insert(neighbor_hash);
                    }
                }
            }
            if (tmp.size() > 2) return tmp;
            // 第三种 L
            if (i <= N - 2 && j >= 1) {
                for (int k = 0; k < N; k++)
                    for (int l = 0; l < N; l++) {
                        new_board[k][l] = node->board[k][l];
                    }
                if (new_board[i + 1][j] + new_board[i][j] + new_board[i][j - 1] == 1) {
                    new_board[i + 1][j] = 1 - new_board[i + 1][j];
                    new_board[i][j] = 1 - new_board[i][j];
                    new_board[i][j - 1] = 1 - new_board[i][j - 1];
                    uint64_t neighbor_hash = hash_board(new_board);
                    if (visited.find(neighbor_hash) == visited.end()) {
                        tmp.push_back(new Node(new_board, node, node->g + 1));
                        visited.insert(neighbor_hash);
                    }
                }
            }
            if (tmp.size() > 2) return tmp;
            // 第四种 L
            if (i >= 1 && j >= 1) {
                for (int k = 0; k < N; k++)
                    for (int l = 0; l < N; l++) {
                        new_board[k][l] = node->board[k][l];
                    }
                if (new_board[i - 1][j] + new_board[i][j] + new_board[i][j - 1] == 1) {
                    new_board[i - 1][j] = 1 - new_board[i - 1][j];
                    new_board[i][j] = 1 - new_board[i][j];
                    new_board[i][j - 1] = 1 - new_board[i][j - 1];
                    uint64_t neighbor_hash = hash_board(new_board);
                    if (visited.find(neighbor_hash) == visited.end()) {
                        tmp.push_back(new Node(new_board, node, node->g + 1));
                        visited.insert(neighbor_hash);
                    }
                }
            }
            if (tmp.size() > 2) return tmp;
        }
    return tmp;
}

bool equal(Node* a, Node* b) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            if (a->board[i][j] != b->board[i][j]) return false;
        }
    return true;
}

double IDA_star(Node* node, double threshold) {
    if (Is_result(node)) {
        Print_result();
        return -1;
    }
    if (node->f > threshold) {
        return node->f;
    }
    double Min = INFINITY;
    vector<Node*> tmp = get_neighbors(node);
    for (auto neighbor : tmp) {
        if (node->parent != nullptr && equal(neighbor, node->parent)) {
            free(neighbor);
            continue;
        }
        double flag = IDA_star(neighbor, threshold);
        if (flag == -1) {
            free(neighbor);
            return -1;
        }
        if (flag < Min) Min = flag;
        free(neighbor);
    }
    return Min;
}



int main()
{
    cin >> N;
    int start[MAXN][MAXN];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> start[i][j];
    Node* now = new Node(start, nullptr, 0);

    double threshold = now->f;
    while (true) {
        visited.clear();
        double temp = IDA_star(now, threshold);
        if (temp == -1) {
            break;
        }
        if (temp == INFINITY) {
            cout << "No valid solution" << endl;
            break;
        }
        threshold = temp;
        cout << threshold << endl;
    }
    return 0;
}