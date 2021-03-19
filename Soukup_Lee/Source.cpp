#include <iomanip>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stack> 
#include <queue>
#include <vector>
#include <ctime>
#include "glut.h"
#include "glaux.h"
#include "control.h"
#include "image.h"
#include <chrono>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glaux.lib")

using namespace std;

//��� ����������� ������� ���������� ���������
class Timer {
private: // ���������� ������������ ��� �������� ������� � ��������� �����
    using clock_t = chrono::high_resolution_clock;
    using second_t = chrono::duration<double, std::ratio<1> >;
    chrono::time_point<clock_t> m_beg;
public:
    Timer() : m_beg(clock_t::now()) {}
    void reset() { m_beg = clock_t::now(); }
    double elapsed() const {
        return chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    }
};


//int newDRP[100][100];
//char Lboard[100][100], Sboard[100][100];
int n=1, count_barrier, bx, by, count_trace, ist_x, ist_y, target_x, target_Y;
bool Lblock[10000], Sblock[10000];
int Lpath[10000], Spath[10000],
    dx[4] = { -1,0,1,0 }, dy[4] = { 0,-1,0,1 };
int startX_mass[100], startY_mass[100], target_X_mass[100], target_Y_mass[100];
int super_id;

void print_termenal_and_barrier();
void print_DRP();
void Print_newDRP(char Sboard[100][100]);
void RenderScene();

int TO_TARGET(int curr, int target)                                                             //���������� �� ���������
{
    int Manh_dist;
    int tx = target / n, ty = target % n, cx = curr / n, cy = curr % n;                         //����� ���������� ���������� � � � ��������� � ���������
    Manh_dist = abs(tx - cx) + abs(ty - cy);
    return Manh_dist;                                                                             //������� ��������������� ����������
}

int DIRECTION(int next, bool visit[], int target)                                                    //�����������
{
    int Manh_dist = INT_MAX;
    int tx = target / n, ty = target % n, nx = next / n, ny = next % n;                              //����� ���������� ���������� � � � ��������� � ���������

    for (int i = 0; i < 4; i++) 
    {
        int x = nx + dx[i], y = ny + dy[i];                                                             //��
        int id = x * n + y;
        if ((x >= 0 && x < n && y >= 0 && y < n) && visit[id] == false && Sblock[id] == false)          //���������� ���������� ���������� ����� ���������� � ���������� � ����������� �� ������ ����������� ???
        {
            if (Manh_dist > abs(tx - x) + abs(ty - y))
            {
                Manh_dist = abs(tx - x) + abs(ty - y);
                super_id = id;
            }
        }
    }
    return Manh_dist;                                                                                         //������� �����������, ������� � �������� ������� ����������� ���������� ??
}

int NGHBR_IN_DIR(int curr, bool visit[], int target_func)                                   //�������� �� ����� �����������????
{
    int x = curr / n, y = curr % n;                                                 //
    bool flag = true;
    int id;

    if (DIRECTION(curr, visit, target_func) <= TO_TARGET(curr, target_func)) 
    {
        flag = false;
        id = super_id;
    }
    return flag == true ? -1 : id;
}

bool SoukupAlgo(int x1, int y1, int x2, int y2)
{
     bool visit[10000];
    memset(visit, false, (n * n) + 1);                              //��������� ��� 0

    int start_func = x1 * n + y1, target_func = x2 * n + y2;                       //

    stack<int>FILO_stack;                                                //FILO
    queue<int>LIFO_queue;                                                //LIFO   (������������ �������)           
       // cout << "in FILO_stack " << start_func << endl;
    FILO_stack.push(start_func);                                                //��������� ������� � �������
    Spath[start_func] = -1;
    visit[start_func] = true;

    int cnt = 0;

    while (!FILO_stack.empty())                  //���� � ������� ���� ��-��    
    {     
        int first_element = FILO_stack.top();              //������ ������� � �������

        if (first_element == target_func) 
        {
            return true;
        }

        if (DIRECTION(first_element, visit, target_func) <= TO_TARGET(first_element, target_func))      //���������� ���������� �� ������� ��������� �� ��� ��������� � ���������� ��
        {
            int id = super_id;
            //cout<<"in FILO_stack "<<id<<endl;
            FILO_stack.push(id);                             //��������� ������� � �������
            visit[id] = true;
            Spath[id] = first_element;
            if (id == target_func)
            {
                return true;
            }

            while (NGHBR_IN_DIR(id, visit, target_func) >= 0)               //���� ����������� �� ��������, �������� �� ���� �� ���� �� ��������� ���������
            {
                int new_id = NGHBR_IN_DIR(id, visit, target_func);
                //cout<<"in FILO_stack "<<new_id<<endl;
                FILO_stack.push(new_id);                                     //��������� ������� � �������
                visit[new_id] = true;
                Spath[new_id] = id;
                if (new_id == target_func) 
                {
                    return true;
                }
                id = new_id;
            }
        }
        while (!FILO_stack.empty())                                          //���� ���� �������� � �������
        {
            first_element = FILO_stack.top();                                          //���������� � ������� �������� � �����
            int tx = first_element / n, ty = first_element % n;                             //���������� ����-�� ����� � ������ ������ 
            for (int i = 0; i < 4; i++) 
            {
                int x = tx + dx[i], y = ty + dy[i];
                int id = x * n + y;
                if ((x >= 0 && x < n && y >= 0 && y < n) && visit[id] == false && Sblock[id] == false)                  //���������� ������� ���������� �����������
                {
                    //cout<<"in LIFO_queue "<<id<<endl;
                    LIFO_queue.push(id);                                     //�������� ������� � �������
                    visit[id] = true;
                    Spath[id] = first_element;
                }
            }
            FILO_stack.pop();                                        //������� ������ ������� �� �������
        }
        while (!LIFO_queue.empty())                                  //���� � ������� ���� ��������                              
        {
            FILO_stack.push(LIFO_queue.front());                          //��������� � FILO_stack ������ ������� �� LIFO_queue
            LIFO_queue.pop();                                        //������� ������ ������� �� �������
        }
    }
    return false;
}


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    cout << endl << "�� ��������� ���������, ����������� �������� ������� ����������� ������� �������." << endl
        << "������ �������� ������� ������ ��-44 ���������� �����." << endl
        << "�������� ������� �������� �������������� ����������. ��� ������������ ����� ���� �������� 10�10 ����� � �������������" << endl
        << "������� ���������� ���������� S1, S2, S3 � T1, T2, T3 ��������������, � ����� input " << endl;

    ifstream in("input.txt");                                                                       //��������� ������ ������ 
    ofstream out2("soukup.txt");
    in >> n;           //�� �������                                                                 //������ ?
    in >> count_barrier;                                                                                  //���-�� �����������
    for (int i = 0; i < n; i++)                                                                     //��������� ��� 0
    {
        for (int j = 0; j < n; j++) 
        {
            Lboard[i][j] = '0';
            Sboard[i][j] = '0';
        }
    }
    memset(Lblock, false, (n * n) + 1);                                                                 //��������� ����� ������ Lblock � Sblock 0
    memset(Sblock, false, (n * n) + 1);

    for (int i = 0; i < count_barrier; i++)                                                               //����������� ����������� �� �����
    {
        in >> bx >> by;         
        Lboard[bx][by] = '#';
        Sboard[bx][by] = '#';
        Lblock[bx * n + by] = true;                                                                         //��
        Sblock[bx * n + by] = true;
    }
    bool ans[2][100];

    in >> count_trace;                                                                                  //���-�� ����� (�����) 

    cout << endl << endl;
    _getch();
    print_termenal_and_barrier();
    cout << endl << endl;

   /* ������� � ��������� 4 �������
    1) ������ � ����-� ����������
    2) ������ � ����-� ����������
    3) ������ � ����-� ����������
    4) ������ � ����-� ����������
    ���������� ��������� � ��������� ���������������� ���������
    ������� � �������*/
    for (int i = 0; i < count_trace; i++)                                                                   //����������� ��������� �� �����
    {
        in >> ist_x >> ist_y >> target_x >> target_Y;                                     //��������� ����-�� ��������� � ����-�� ���������
        startX_mass[i] = ist_x;                                       //start_x
        startY_mass[i] = ist_y;                                       //start_y
        target_X_mass[i] = target_x;                                     //target_x
        target_Y_mass[i] = target_Y;                                     //target_y
        Lboard[ist_x][ist_y] = 'S';
        Lboard[target_x][target_Y] = 'T';
        Sboard[ist_x][ist_y] = 'S';
        Sboard[target_x][target_Y] = 'T';
    }
    print_termenal_and_barrier();
    cout << endl << endl;
    Timer t;

    //--------------------------for Soukup Algorithm---------------------
    out2 << "Soukup Algorithm : " << endl;

    for (int i = 0; i < count_trace; i++)                                       //��� ������ ���� ��������-��������
    {
        ist_x = startX_mass[i];
        ist_y = startY_mass[i];
        target_x = target_X_mass[i];
        target_Y = target_Y_mass[i];
        //for Lee's algorithm
        if (SoukupAlgo(ist_x, ist_y, target_x, target_Y) == true)                           //���� ����������� ������ �������, ������������� �� � �������
        {   

            int id = target_x * n + target_Y;
            Sblock[id] = true;
            Sblock[ist_x * n + ist_y] = true;
            while (Spath[id] != -1)
            {
                Sboard[Spath[id] / n][Spath[id] % n] = '*';
                Sblock[Spath[id]] = true;
                id = Spath[id];
            }
            Sboard[ist_x][ist_y] = 'S';
            ans[1][i] = true;
        }
        else                                                         //���� ������ �������� �����������
        {     
            Sboard[ist_x][ist_y] = 's';
            Sboard[target_x][target_Y] = 't';
            ans[1][i] = false;
        }
    }
    double time_algotim = t.elapsed();
    
    for (int i = 0; i < n; i++)                                                     //����� ���������� � ����
    {
        for (int j = 0; j < n; j++) out2 << Sboard[i][j] << " ";
        out2 << endl;
    }
    out2 << endl;
    for (int i = 0; i < count_trace; i++)                                               //���� ������ �������� �����������
    {
        if (ans[1][i] == false)
        {
            out2 << " (" << ist_x << "," << ist_y << ")->(" << target_x << "," << target_Y << ") ���������� �������� �����������." << endl;
        }
    }

    _getch();
    print_DRP();
    cout << endl << endl;

    Print_newDRP(Sboard);
    cout <<endl << "��������� ������� : " << time_algotim*1000000000 <<" ns" <<'\n';
    _getch();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("�������� �������");
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1);
    glutReshapeFunc(reshape);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(Read_kb);
    glutSpecialFunc(ReadSpec);
    glutMainLoop();
   
    return 0;
}