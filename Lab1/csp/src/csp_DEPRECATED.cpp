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

struct NodeRequests {
	int d;
	int s;
	int counts; // �����counts��������Ű��������
	// vector<int> aunts;
	bool aunts[200];
};

struct Node {
	int d;
	int s;
	int counts = 0; // �ǿ�ѡ��ʣ��ֵ
	int chose; // ѡ��İ���
	// vector<int> aunts; // ��ѡ�İ��̼���
	bool aunts[200];
};

int N, D, S; // N �����̣�D ��ֵ��������ÿ���ְ���� S
int request[200][400][6]; // request[i][j][k] ��i����������ֵ��j��ĵ�k�ְ�
int shifts[400][6]; // shifts[j][k]=i ��j���k������ŵ��ǵ�i������
int counts[200]; // ��i�����̵��Ű���
int ans = -1;
int base;
int Top;

NodeRequests Requests[200][400];
Node Shifts[400][6];

// �Ȱ��� request �����ܰ���
// Ȼ�����ÿ���Ű࣬��Ϊ��㣬���� CSP

void Update() {
	// �Ƿ����а��̵��Ű඼��������Ű����Ҫ��
	//for (int i = 0; i < N; i++) counts[i] = 0;
	//for (int j = 0; j < D; j++)
	//	for (int k = 0; k < S; k++)
	//		counts[Shifts[j][k].chose]++;
	for (int i = 0; i < N; i++) {
		if (counts[i] < base) {
			return;
		}
	}

	int cnt = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < D; j++) {
			for (int k = 0; k < S; k++) {
				if (request[i][j][k] == 1 && Shifts[j][k].chose == i) cnt++;
			}
		}
	}
	if (cnt > ans) {
		ans = cnt;
		cout << ans << endl;
		for (int j = 0; j < D; j++) {
			for (int k = 0; k < S; k++) {
				shifts[j][k] = Shifts[j][k].chose;
			}
		}
	}
}



// ��������DFS
// �ȿ����������д�����İ�Σ�turn = 1
// �����д�����İ�ζ������꣬����ڶ��ֿ��ǣ��Ǿ��ǲ������κ����룬ΨһĿ������������н�
bool DFS1(int turn, int curans, int depth, int doneshifts) {
	if (turn == 0 && curans <= ans) return false;
	// cout << "curans = " << curans << endl;
	// cout << "turn = "<<turn<<"  depth = " << depth << endl;
	//bool flag_end = true; // �Ƿ����а�ζ����Ű�
	//for (int j = 0; j < D; j++)
	//	for (int k = 0; k < S; k++) {
	//		if (Shifts[j][k].chose < 0) {
	//			flag_end = false;
	//			if (Shifts[j][k].counts == 0) {
	//				return false; // ����ĳ���Ű�ʱ�䣬δ���Űಢ��û�п�ѡ�İ��̣���ôһ���޷����㣬ֱ�� return false 
	//			}
	//		}
	//	}
	//if (flag_end) {
	//	Update();
	//	if (curans == ans) cout << "YES!!!!!!!!!!!" << endl;
	//	return true;
	//}

	if (doneshifts == D * S) {
		Update();
		if (curans == ans) cout << "YES!!!!!!!!!!!" << endl;
		return true;
	}

	// ���ҳ���ǰδ�Ű�ġ�������ġ������������ٵİ��
	int tmp_d = 0, tmp_s = 0, tmp_c = 0;
	bool flag = false; // �Ƿ��ҵ����������İ��
	if (turn == 1) {
		for (int j = 0; j < D; j++)
			for (int k = 0; k < S; k++) {
				if (Shifts[j][k].chose == -1 && Requests[j][k].counts > 0) { // �������������δ�Ű�İ�Σ�����֮ǰû��������
					if (flag == false) {
						flag = true;
						tmp_c = Requests[j][k].counts;
						tmp_d = Requests[j][k].d; tmp_s = Requests[j][k].s;
					}
					else if (Requests[j][k].counts < tmp_c) {
						tmp_c = Requests[j][k].counts;
						tmp_d = Requests[j][k].d; tmp_s = Requests[j][k].s;
					}
				}
			}
	}
	if (flag) {
		// �ҵ���ǰδ�Ű࣬����������İ�Σ��ж���Ļ���ѡ��������п�ѡ���������ٵ��Ǹ�
		// ���������
		// cout << "tmp_d = " << tmp_d << " tmp_s = " << tmp_s << endl;
		Shifts[tmp_d][tmp_s].chose = -1;
		int aunt = -1;

		for (int i = 0; i < N; i++) {
			// �������ѡ��İ��̣�ѡ���Ѿ��Ű������ٵ�
			//aunt = -1;
			//tmp_c = D * S;
			//for (int i = 0; i < N; i++) {
			//	if (Shifts[tmp_d][tmp_s].aunts[i] == 1 && counts[i] < tmp_c) {
			//		aunt = i;
			//		tmp_c = counts[i];
			//	}
			//}
			//if (aunt == -1) break;
			if (counts[i] > Top) continue;
			if (Shifts[tmp_d][tmp_s].aunts[i] == 0) continue;
			if (Requests[tmp_d][tmp_s].aunts[i] == 0) continue;

			aunt = i;
			Shifts[tmp_d][tmp_s].chose = aunt;
			counts[aunt]++;
			bool flag_l = false;
			bool flag_r = false;
			bool flag_n = false;
			bool flag_p = false;
			if (tmp_s > 0) {
				// auto it0 = find(Shifts[tmp_d][tmp_s - 1].aunts.begin(), Shifts[tmp_d][tmp_s - 1].aunts.end(), aunt);
				if (Shifts[tmp_d][tmp_s - 1].aunts[aunt] == 1) {
					// if (Shifts[tmp_d][tmp_s - 1].counts <= 1) continue;
					flag_l = true;
					Shifts[tmp_d][tmp_s - 1].counts--;
					Shifts[tmp_d][tmp_s - 1].aunts[aunt] = 0;
				}
			}
			if (tmp_s < N - 1) {
				// auto it0 = find(Shifts[tmp_d][tmp_s + 1].aunts.begin(), Shifts[tmp_d][tmp_s + 1].aunts.end(), aunt);
				if (Shifts[tmp_d][tmp_s + 1].aunts[aunt] == 1) {
					// if (Shifts[tmp_d][tmp_s + 1].counts <= 1) continue;
					flag_r = true;
					Shifts[tmp_d][tmp_s + 1].counts--;
					Shifts[tmp_d][tmp_s + 1].aunts[aunt] = 0;
				}
			}
			if (tmp_s == N - 1 && tmp_d < D - 1) {
				// auto it0 = find(Shifts[tmp_d][tmp_s + 1].aunts.begin(), Shifts[tmp_d][tmp_s + 1].aunts.end(), aunt);
				if (Shifts[tmp_d + 1][0].aunts[aunt] == 1) {
					// if (Shifts[tmp_d + 1][0].counts <= 1) continue;
					flag_n = true;
					Shifts[tmp_d + 1][0].counts--;
					Shifts[tmp_d + 1][0].aunts[aunt] = 0;
				}
			}
			if (tmp_s == 0 && tmp_d > 0) {
				// auto it0 = find(Shifts[tmp_d][tmp_s + 1].aunts.begin(), Shifts[tmp_d][tmp_s + 1].aunts.end(), aunt);
				if (Shifts[tmp_d - 1][N - 1].aunts[aunt] == 1) {
					// if (Shifts[tmp_d + 1][0].counts <= 1) continue;
					flag_p = true;
					Shifts[tmp_d - 1][N - 1].counts--;
					Shifts[tmp_d - 1][N - 1].aunts[aunt] = 0;
				}
			}
			DFS1(1, curans + 1, depth + 1, doneshifts + 1); // ��ǰ��ΰ�����һλ���̣�Ȼ��DFS
			Shifts[tmp_d][tmp_s].chose = -2;
			counts[aunt]--;
			if (flag_l) {
				Shifts[tmp_d][tmp_s - 1].counts++;
				Shifts[tmp_d][tmp_s - 1].aunts[aunt] = 1;
			}
			if (flag_r) {
				Shifts[tmp_d][tmp_s + 1].counts++;
				Shifts[tmp_d][tmp_s + 1].aunts[aunt] = 1;
			}
			if (flag_n) {
				Shifts[tmp_d + 1][0].counts++;
				Shifts[tmp_d + 1][0].aunts[aunt] = 1;
			}
			if (flag_p) {
				Shifts[tmp_d - 1][N - 1].counts++;
				Shifts[tmp_d - 1][N - 1].aunts[aunt] = 1;
			}
		}
		// cout << "DFS(1) no aunt: " << endl;
		Shifts[tmp_d][tmp_s].chose = -2;
		DFS1(1, curans, depth + 1, doneshifts); // ��ǰ���δ�����ˣ���֮��ð�ξ���Ϊ�����룬����DFS
		Shifts[tmp_d][tmp_s].chose = -1;
		// cout << "DFS(1) end" << aunt << endl;
		return false;
	}

	tmp_c = 0x3f3f3f3f;
	// ����������İ�ζ����ǹ���
	// ʣ�µĲ���Ҫ�ٿ��ǡ����롱��ֻ���ҵ����������Ľ⼴��
	if (flag == false) {
		// �ҵ�δ�Ű�ġ���ѡ���������ٵİ��
		for (int j = 0; j < D; j++)
			for (int k = 0; k < S; k++) {
				if (Shifts[j][k].chose < 0) { // δ�Ű�İ��
					if (Shifts[j][k].counts < tmp_c) {
						tmp_c = Requests[j][k].counts;
						tmp_d = Requests[j][k].d; tmp_s = Requests[j][k].s;
					}
				}
			}
		int aunt = -1;
		for (int i = 0; i < N; i++) {
			if (counts[i] > Top) continue;
			if (Shifts[tmp_d][tmp_s].aunts[i] == 0) continue;
			aunt = i;
			Shifts[tmp_d][tmp_s].chose = aunt;
			counts[aunt]++;
			bool flag_l = false;
			bool flag_r = false;
			bool flag_n = false;
			bool flag_p = false;
			if (tmp_s > 0) {
				// auto it0 = find(Shifts[tmp_d][tmp_s - 1].aunts.begin(), Shifts[tmp_d][tmp_s - 1].aunts.end(), aunt);
				if (Shifts[tmp_d][tmp_s - 1].aunts[aunt] == 1) {
					// if (Shifts[tmp_d][tmp_s - 1].counts <= 1) continue;
					flag_l = true;
					Shifts[tmp_d][tmp_s - 1].counts--;
					Shifts[tmp_d][tmp_s - 1].aunts[aunt] = 0;
				}
			}
			if (tmp_s < N - 1) {
				// auto it0 = find(Shifts[tmp_d][tmp_s + 1].aunts.begin(), Shifts[tmp_d][tmp_s + 1].aunts.end(), aunt);
				if (Shifts[tmp_d][tmp_s + 1].aunts[aunt] == 1) {
					// if (Shifts[tmp_d][tmp_s + 1].counts <= 1) continue;
					flag_r = true;
					Shifts[tmp_d][tmp_s + 1].counts--;
					Shifts[tmp_d][tmp_s + 1].aunts[aunt] = 0;
				}
			}
			if (tmp_s == N - 1 && tmp_d < D - 1) {
				// auto it0 = find(Shifts[tmp_d][tmp_s + 1].aunts.begin(), Shifts[tmp_d][tmp_s + 1].aunts.end(), aunt);
				if (Shifts[tmp_d + 1][0].aunts[aunt] == 1) {
					// if (Shifts[tmp_d + 1][0].counts <= 1) continue;
					flag_n = true;
					Shifts[tmp_d + 1][0].counts--;
					Shifts[tmp_d + 1][0].aunts[aunt] = 0;
				}
			}
			if (tmp_s == 0 && tmp_d > 0) {
				// auto it0 = find(Shifts[tmp_d][tmp_s + 1].aunts.begin(), Shifts[tmp_d][tmp_s + 1].aunts.end(), aunt);
				if (Shifts[tmp_d - 1][N - 1].aunts[aunt] == 1) {
					// if (Shifts[tmp_d + 1][0].counts <= 1) continue;
					flag_p = true;
					Shifts[tmp_d - 1][N - 1].counts--;
					Shifts[tmp_d - 1][N - 1].aunts[aunt] = 0;
				}
			}
			DFS1(0, curans, depth + 1, doneshifts + 1); // ��ǰ��ΰ�����һλ���̣�Ȼ��DFS
			Shifts[tmp_d][tmp_s].chose = -2; // δ�Ű࣬����������
			counts[aunt]--;
			if (flag_l) {
				Shifts[tmp_d][tmp_s - 1].counts++;
				Shifts[tmp_d][tmp_s - 1].aunts[aunt] = 1;
			}
			if (flag_r) {
				Shifts[tmp_d][tmp_s + 1].counts++;
				Shifts[tmp_d][tmp_s + 1].aunts[aunt] = 1;
			}
			if (flag_n) {
				Shifts[tmp_d + 1][0].counts++;
				Shifts[tmp_d + 1][0].aunts[aunt] = 1;
			}
			if (flag_p) {
				Shifts[tmp_d - 1][N - 1].counts++;
				Shifts[tmp_d - 1][N - 1].aunts[aunt] = 1;
			}
		}
		return false;
	}
	return false;
}

int main()
{
	string inputfile;
	cin >> inputfile;
	ifstream input_file(inputfile);
	if (!input_file) {
		cerr << "Unable to open input file\n";
		exit(1);
	}
	string line;
	getline(input_file, line);
	istringstream iss1(line);
	getline(iss1, line, ',');
	N = stoi(line);
	getline(iss1, line, ',');
	D = stoi(line);
	getline(iss1, line, ',');
	S = stoi(line);
	cout << N << " " << D << " " << S << endl;
	// cin >> N >> D >> S;
	// N = 2; D = 2; S = 2;

	base = D * S / N;
	Top = base + D * S - base * N;
	for (int j = 0; j < D; j++)
		for (int k = 0; k < S; k++) {
			Shifts[j][k].d = j; Shifts[j][k].d = k;
			Shifts[j][k].chose = -1;
			Shifts[j][k].counts = N; // ��ѡ������
			for (int i = 0; i < N; i++) {
				Shifts[j][k].aunts[i] = 1;
			}
		}
	for (int i = 0; i < N; i++)
		for (int j = 0; j < D; j++) {
			getline(input_file, line);
			istringstream iss2(line);
			for (int k = 0; k < S; k++) {
				Requests[j][k].d = j; Requests[j][k].s = k;
				getline(iss2, line, ',');
				request[i][j][k] = stoi(line);
				// request[i][j][k] = 0;
				// cin >> request[i][j][k];
				if (request[i][j][k]) {
					Requests[j][k].counts++;
					Requests[j][k].aunts[i] = 1;
				}
			}
		}

	input_file.close();
	// ��������
	// ��һ���������� request �ģ��Ⱦ���������
	// Խ�ٵİ�������ģ���������
	// �ڶ�������ʣ�µ�
	// Ŀǰ�ݲ����ǵ�һ������ѡ��󣬵ڶ���ѡ��ʧ��
	DFS1(1, 0, 0, 0);
	cout << "ans = " << ans << endl;
	for (int j = 0; j < D; j++) {
		for (int k = 0; k < S; k++) {
			cout << shifts[j][k] << " ";
		}
		cout << endl;
	}
	return 0;
}