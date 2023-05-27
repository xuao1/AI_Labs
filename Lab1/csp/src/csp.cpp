#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <queue>
#include <chrono>
#include <cstring>

using namespace std;
using namespace chrono;


int N, D, S; // N �����̣�D ��ֵ��������ÿ���ְ���� S
bool request[2400][200]; // �� i ����Σ��� j �������Ƿ����� 
int shifts[2400];	// ��ǰ���������Ű氲��
int counts[200];	// ÿ�����̵��Ѿ��Ű���Ŀ
int ans = -1;		// �Ѿ������������Ž��������
int ans_shifts[2400];	// �Ѿ������������Ž���Ű氲�ţ���ÿ����ε�
int base;			// ÿ���������ձ��Ű���Ŀ���½�
int Top;			// ÿ���������ձ��Ű���Ŀ���Ͻ�
int resum[2400];	// ��׺�ͣ���֦�ã�������� 2.2.3
bool HaveRequests[2400];	// ĳ������Ƿ�������

void Update(int cnt) {
	if (cnt <= ans) return;
	for (int i = 0; i < N; i++) {
		if (counts[i] < base) return;
	}
	ans = cnt;
	// cout << ans << endl;
	for (int i = 0; i < D * S; i++)
		ans_shifts[i] = shifts[i];
}

void Search1(int num, int pre, int cnt) {
	if (cnt + resum[num] <= ans) return;
	if (num == D * S) {
		Update(cnt);
		return;
	}
	int* vis = new int[N];
	for (int i = 0; i < N; i++) {
		vis[i] = 0;
	}
	int index = -1;
	for (int k = 0; k < N; k++) {
		index = -1;
		for (int i = 0; i < N; i++) {
			if (request[num][i] == 0) continue;
			if (i == pre) continue;
			if (vis[i]) continue;
			if (counts[i] >= Top) continue;
			if (index == -1) {
				index = i;
			}
			else if (counts[i] < counts[index]) {
				index = i;
			}
		}
		if (index == -1) break;
		else {
			vis[index] = 1;
			counts[index]++;
			shifts[num] = index;
			Search1(num + 1, index, cnt + 1);
			counts[index]--;
			shifts[num] = 0;
		}
	}

	for (int k = 0; k < N; k++) {
		index = -1;
		for (int i = 0; i < N; i++) {
			if (request[num][i] == 1) continue;
			if (i == pre) continue;
			if (vis[i]) continue;
			if (counts[i] >= Top) continue;
			if (index == -1) {
				index = i;
			}
			else if (counts[i] < counts[index]) {
				index = i;
			}
		}
		if (index == -1) break;
		else {
			vis[index] = 1;
			counts[index]++;
			shifts[num] = index;
			Search1(num + 1, index, cnt);
			counts[index]--;
			shifts[num] = 0;
		}
	}

	delete[]vis;
}

int main()
{
	string inputfile;
	cin >> inputfile;
	std::cout << inputfile << endl;
	ifstream input_file(inputfile);
	if (!input_file) {
		cerr << "Unable to open input file\n";
		exit(1);
	}
	auto start = high_resolution_clock::now();
	string line;
	getline(input_file, line);
	istringstream iss1(line);
	getline(iss1, line, ',');
	N = stoi(line);
	getline(iss1, line, ',');
	D = stoi(line);
	getline(iss1, line, ',');
	S = stoi(line);
	std::cout << N << " " << D << " " << S << endl;
	base = D * S / N;
	Top = base + D * S - base * N;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < D; j++) {
			getline(input_file, line);
			istringstream iss2(line);
			for (int k = 0; k < S; k++) {
				getline(iss2, line, ',');
				if (stoi(line) == 1) {
					request[j * S + k][i] = 1;
					HaveRequests[j * S + k] = 1;
				}

			}
		}
	for (int i = D * S - 1; i >= 0; i--) {
		resum[i] = resum[i + 1];
		if (HaveRequests[i]) {
			resum[i]++;
		}
	}

	Search1(0, -1, 0);
	auto end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end - start);
	std::cout << "Runtime: " << duration.count() << " ms" << endl;

	input_file.close();
	string outputfile;
	size_t found = inputfile.find("input");
	outputfile = inputfile;
	outputfile.replace(found, 5, "output"); // Replace "input" with "output"
	const char* outputCStr = outputfile.c_str();
	FILE* file;
	freopen_s(&file, outputCStr, "w", stdout);
	if (ans == -1) std::cout << "No valid schedule found." << endl;
	else {
		for (int j = 0; j < D; j++) {
			for (int k = 0; k < S; k++) {
				std::cout << ans_shifts[j * S + k];
				if (k < S - 1) std::cout << ",";
			}
			std::cout << endl;
		}
		std::cout << ans << endl;
	}

	fclose(stdout);

	return 0;
}