#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_set>
#include <cstdint>
#include <stack>
#include <cstring>
#include <chrono>
#define MAXN 13

using namespace std;
using namespace chrono;

struct Step {
    int xx, yy;
    int ss;
    Step(int xx, int yy, int ss) : xx(xx), yy(yy), ss(ss) {}
};

int N;
bool vis[MAXN][MAXN];

int DFS(int x, int y, int b[MAXN][MAXN]) {
    vis[x][y] = 1;
    int tmp = 1;
    if (x > 0 && b[x - 1][y] == 1 && vis[x - 1][y] == 0) tmp += DFS(x - 1, y, b);
    if (y > 0 && b[x][y - 1] == 1 && vis[x][y - 1] == 0) tmp += DFS(x, y - 1, b);
    if (x < N - 1 && b[x + 1][y] == 1 && vis[x + 1][y] == 0) tmp += DFS(x + 1, y, b);
    if (y < N - 1 && b[x][y + 1] == 1 && vis[x][y + 1] == 0) tmp += DFS(x, y + 1, b);
    if (x > 0 && y > 0 && b[x - 1][y - 1] == 1 && vis[x - 1][y - 1] == 0) tmp += DFS(x - 1, y - 1, b);
    if (x > 0 && y < N - 1 && b[x - 1][y + 1] == 1 && vis[x - 1][y + 1] == 0) tmp += DFS(x - 1, y + 1, b);
    if (x < N - 1 && y > 0 && b[x + 1][y - 1] == 1 && vis[x + 1][y - 1] == 0) tmp += DFS(x + 1, y - 1, b);
    if (x < N - 1 && y < N - 1 && b[x + 1][y + 1] == 1 && vis[x + 1][y + 1] == 0) tmp += DFS(x + 1, y + 1, b);
    return tmp;
}

struct Node {
    int board[MAXN][MAXN];
    int f = 0, g = 0, h = 0; // f = g + h 
    int xx = 0, yy = 0, ss = 0;
    Node* parent = nullptr;

    Node(int b[MAXN][MAXN], Node* p, int g_val, int xx, int yy, int ss) : parent(p), g(g_val), xx(xx), yy(yy), ss(ss) {
        int tmp = 0, cnt = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                vis[i][j] = 0;
                board[i][j] = b[i][j];
                if (b[i][j] == 1) cnt++;
            }
        }
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (vis[i][j] == 0 && board[i][j] == 1) {
                    tmp = tmp + int((DFS(i, j, board) + 2) / 3);
                }
            }
        }
        // tmp = ceil(cnt / 3);
        // if (cnt % 2 != tmp % 2) tmp++;
        h = tmp;
        f = g + h;
    }
};

struct Compare {
    bool operator()(const Node* a, const Node* b) {
        return a->f > b->f; // 与sort的cmp逻辑刚好是相反的
    }
};

priority_queue<Node*, vector<Node*>, Compare> OpenList;

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

stack<Step*, vector<Step*>> Steps;

void Print_result(Node* a) {
    // cout << "yes" << endl;
    int cnt = 0;
    Node* b = a;
    while (b->parent != nullptr) {
        cnt++;
        Steps.push(new Step(b->xx, b->yy, b->ss));
        b = b->parent;
    }
    std::cout << cnt << endl;
    while (!Steps.empty()) {
        std::cout << Steps.top()->xx << ", " << Steps.top()->yy << ", " << Steps.top()->ss << endl;
        Steps.pop();
    }
}

int new_board[MAXN][MAXN];
void add_neighbors(Node* node) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            new_board[i][j] = node->board[i][j];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (node->board[i][j] == 1) {
                if (i > 0 && j > 0) {
                    new_board[i][j] = 1 - new_board[i][j]; new_board[i - 1][j] = 1 - new_board[i - 1][j]; new_board[i - 1][j - 1] = 1 - new_board[i - 1][j - 1];
                    OpenList.push(new Node(new_board, node, node->g + 1, i - 1, j, 3));
                    new_board[i][j] = 1 - new_board[i][j]; new_board[i - 1][j] = 1 - new_board[i - 1][j]; new_board[i - 1][j - 1] = 1 - new_board[i - 1][j - 1];
                }
                if (i > 0 && j > 0) {
                    new_board[i][j] = 1 - new_board[i][j]; new_board[i][j - 1] = 1 - new_board[i][j - 1]; new_board[i - 1][j - 1] = 1 - new_board[i - 1][j - 1];
                    OpenList.push(new Node(new_board, node, node->g + 1, i, j - 1, 1));
                    new_board[i][j] = 1 - new_board[i][j]; new_board[i][j - 1] = 1 - new_board[i][j - 1]; new_board[i - 1][j - 1] = 1 - new_board[i - 1][j - 1];
                }
                if (i > 0 && j < N - 1) {
                    new_board[i][j] = 1 - new_board[i][j]; new_board[i - 1][j] = 1 - new_board[i - 1][j]; new_board[i - 1][j + 1] = 1 - new_board[i - 1][j + 1];
                    OpenList.push(new Node(new_board, node, node->g + 1, i - 1, j, 4));
                    new_board[i][j] = 1 - new_board[i][j]; new_board[i - 1][j] = 1 - new_board[i - 1][j]; new_board[i - 1][j + 1] = 1 - new_board[i - 1][j + 1];
                }
                if (i > 0 && j < N - 1) {
                    new_board[i][j] = 1 - new_board[i][j]; new_board[i][j + 1] = 1 - new_board[i][j + 1]; new_board[i - 1][j + 1] = 1 - new_board[i - 1][j + 1];
                    OpenList.push(new Node(new_board, node, node->g + 1, i, j + 1, 2));
                    new_board[i][j] = 1 - new_board[i][j]; new_board[i][j + 1] = 1 - new_board[i][j + 1]; new_board[i - 1][j + 1] = 1 - new_board[i - 1][j + 1];
                }
                if (i < N - 1 && j > 0) {
                    new_board[i][j] = 1 - new_board[i][j]; new_board[i][j - 1] = 1 - new_board[i][j - 1]; new_board[i + 1][j - 1] = 1 - new_board[i + 1][j - 1];
                    OpenList.push(new Node(new_board, node, node->g + 1, i, j - 1, 4));
                    new_board[i][j] = 1 - new_board[i][j]; new_board[i][j - 1] = 1 - new_board[i][j - 1]; new_board[i + 1][j - 1] = 1 - new_board[i + 1][j - 1];
                }
                if (i < N - 1 && j > 0) {
                    new_board[i][j] = 1 - new_board[i][j]; new_board[i + 1][j] = 1 - new_board[i + 1][j]; new_board[i + 1][j - 1] = 1 - new_board[i + 1][j - 1];
                    OpenList.push(new Node(new_board, node, node->g + 1, i + 1, j, 2));
                    new_board[i][j] = 1 - new_board[i][j]; new_board[i + 1][j] = 1 - new_board[i + 1][j]; new_board[i + 1][j - 1] = 1 - new_board[i + 1][j - 1];
                }
                if (i < N - 1 && j < N - 1) {
                    new_board[i][j] = 1 - new_board[i][j]; new_board[i][j + 1] = 1 - new_board[i][j + 1]; new_board[i + 1][j + 1] = 1 - new_board[i + 1][j + 1];
                    OpenList.push(new Node(new_board, node, node->g + 1, i, j + 1, 3));
                    new_board[i][j] = 1 - new_board[i][j]; new_board[i][j + 1] = 1 - new_board[i][j + 1]; new_board[i + 1][j + 1] = 1 - new_board[i + 1][j + 1];
                }
                if (i < N - 1 && j < N - 1) {
                    new_board[i][j] = 1 - new_board[i][j]; new_board[i + 1][j] = 1 - new_board[i + 1][j]; new_board[i + 1][j + 1] = 1 - new_board[i + 1][j + 1];
                    OpenList.push(new Node(new_board, node, node->g + 1, i + 1, j, 1));
                    new_board[i][j] = 1 - new_board[i][j]; new_board[i + 1][j] = 1 - new_board[i + 1][j]; new_board[i + 1][j + 1] = 1 - new_board[i + 1][j + 1];
                }
                if (i >= 1 && j <= N - 2) {
                    new_board[i - 1][j] = 1 - new_board[i - 1][j]; new_board[i][j] = 1 - new_board[i][j]; new_board[i][j + 1] = 1 - new_board[i][j + 1];
                    OpenList.push(new Node(new_board, node, node->g + 1, i, j, 1));
                    new_board[i - 1][j] = 1 - new_board[i - 1][j]; new_board[i][j] = 1 - new_board[i][j]; new_board[i][j + 1] = 1 - new_board[i][j + 1];
                }
                if (i <= N - 2 && j <= N - 2) {
                    new_board[i + 1][j] = 1 - new_board[i + 1][j]; new_board[i][j] = 1 - new_board[i][j]; new_board[i][j + 1] = 1 - new_board[i][j + 1];
                    OpenList.push(new Node(new_board, node, node->g + 1, i, j, 4));
                    new_board[i + 1][j] = 1 - new_board[i + 1][j]; new_board[i][j] = 1 - new_board[i][j]; new_board[i][j + 1] = 1 - new_board[i][j + 1];
                }
                if (i <= N - 2 && j >= 1) {
                    new_board[i + 1][j] = 1 - new_board[i + 1][j]; new_board[i][j] = 1 - new_board[i][j]; new_board[i][j - 1] = 1 - new_board[i][j - 1];
                    OpenList.push(new Node(new_board, node, node->g + 1, i, j, 3));
                    new_board[i + 1][j] = 1 - new_board[i + 1][j]; new_board[i][j] = 1 - new_board[i][j]; new_board[i][j - 1] = 1 - new_board[i][j - 1];
                }
                if (i >= 1 && j >= 1) {
                    new_board[i - 1][j] = 1 - new_board[i - 1][j]; new_board[i][j] = 1 - new_board[i][j]; new_board[i][j - 1] = 1 - new_board[i][j - 1];
                    OpenList.push(new Node(new_board, node, node->g + 1, i, j, 2));
                }
                return;
            }
        }
    }

}

bool equal(Node* a, Node* b) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            if (a->board[i][j] != b->board[i][j]) return false;
        }
    return true;
}



int main()
{
    string inputfile, outputfile;
    cin >> inputfile;
    const char* inputCStr = inputfile.c_str();
    size_t found = inputfile.find("input");
    outputfile = inputfile;
    outputfile.replace(found, 5, "output"); // Replace "input" with "output"
    const char* outputCStr = outputfile.c_str();
    FILE* file1, * file2;
    freopen_s(&file1, inputCStr, "r", stdin);
    freopen_s(&file2, outputCStr, "w", stdout);
    cin >> N;
    int start[MAXN][MAXN];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> start[i][j];

    auto start_T = std::chrono::steady_clock::now();
    OpenList.push(new Node(start, nullptr, 0, 0, 0, 0));


    while (!OpenList.empty()) {
        Node* now = OpenList.top();
        OpenList.pop();
        if (Is_result(now)) {
            Print_result(now);
            break;
        }
        add_neighbors(now);
        if (OpenList.empty()) {
            std::cout << "No valid solution" << endl;
            break;
        }
    }
    auto end_T = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_T - start_T);
    // std::cout << "Runtime: " << duration.count() << " microseconds" << endl;
    return 0;
}
