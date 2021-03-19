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

class Timer {
private: // Псевдонимы используются для удобного доступа к вложенным типам
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
int n=1, nblocks, bx, by, ntarget, sx, sy, desx, desy;
bool Lblock[10000], Sblock[10000];
int Lpath[10000], Spath[10000], dx[4] = { -1,0,1,0 }, dy[4] = { 0,-1,0,1 };
int srcX[100], srcY[100], desX[100], desY[100];
int super_id;

void print_termenal_and_barrier();
void print_DRP();
void Print_newDRP(char Sboard[100][100]);
void RenderScene();

int TO_TARGET(int curr, int target)                             //расстояние до приемника
{
    int dis;
    int tx = target / n, ty = target % n, cx = curr / n, cy = curr % n;                     //снова определяем координаты Х и У приемника и источника
    dis = abs(tx - cx) + abs(ty - cy);
    return dis;                                                                             //находим манхеттоновское расстояние
}

int DIRECTION(int next, bool visit[], int target)                           //направление
{
    int dis = INT_MAX;
    int tx = target / n, ty = target % n, nx = next / n, ny = next % n;                     //снова определяем координаты Х и У приемника и источника

    for (int i = 0; i < 4; i++) 
    {
        int x = nx + dx[i], y = ny + dy[i];                                                             //хз
        int id = x * n + y;
        if ((x >= 0 && x < n && y >= 0 && y < n) && visit[id] == false && Sblock[id] == false)          //определяем наименьшее расстояние между источником и приемником в зависимости от выбора направления ???
        {
            if (dis > abs(tx - x) + abs(ty - y))
            {
                dis = abs(tx - x) + abs(ty - y);
                super_id = id;
            }
        }
    }
    return dis;                                                                                         //находим направление, начиная с которого получим наименьшеее расстояние ??
}

int NGHBR_IN_DIR(int curr, bool visit[], int des)
{
    int x = curr / n, y = curr % n;                                                 //хз
    bool flag = true;
    int id;

    if (DIRECTION(curr, visit, des) <= TO_TARGET(curr, des)) 
    {
        flag = false;
        id = super_id;
    }
    return flag == true ? -1 : id;
}

bool SoukupAlgo(int x1, int y1, int x2, int y2)
{
     bool visit[10000];
    memset(visit, false, (n * n) + 1);                              //заполняем все 0

    int src = x1 * n + y1, des = x2 * n + y2;

    stack<int>plist;                                                ////фифо
    queue<int>nlist;                                                //лифо            
       // cout << "in plist " << src << endl;
    plist.push(src);                                                //добавляем элемент в очередь
    Spath[src] = -1;
    visit[src] = true;

    int cnt = 0;

    while (!plist.empty())                  //пока в очереди есть эл-ты    
    {     
        int pid = plist.top();              //первый элемент в очереди

        if (pid == des) 
        {
            return true;
        }

        if (DIRECTION(pid, visit, des) <= TO_TARGET(pid, des))      //определяем расстояние от каждого источника до его приямника ис равниваем их
        {
            int id = super_id;
            //cout<<"in plist "<<id<<endl;
            plist.push(id);                             //добавляем элемент в очередь
            visit[id] = true;
            Spath[id] = pid;
            if (id == des)
            {
                return true;
            }

            while (NGHBR_IN_DIR(id, visit, des) >= 0)               //если направление не меняется, движемся по нему же пока не достигнем приемника
            {
                int new_id = NGHBR_IN_DIR(id, visit, des);
                //cout<<"in plist "<<new_id<<endl;
                plist.push(new_id);                                     //добавляем элемент в очередь
                visit[new_id] = true;
                Spath[new_id] = id;
                if (new_id == des) 
                {
                    return true;
                }
                id = new_id;
            }
        }
        while (!plist.empty())                                          //пока есть элементы в очереди
        {
            pid = plist.top();                                          //обращаемся к первому элементу в стеке
            int tx = pid / n, ty = pid % n;                             //определяем коор-ты точки в данный момент 
            for (int i = 0; i < 4; i++) 
            {
                int x = tx + dx[i], y = ty + dy[i];
                int id = x * n + y;
                if ((x >= 0 && x < n && y >= 0 && y < n) && visit[id] == false && Sblock[id] == false)                  //определяем порядок построения трассировки
                {
                    //cout<<"in nlist "<<id<<endl;
                    nlist.push(id);                                     //добавить элемент в очередь
                    visit[id] = true;
                    Spath[id] = pid;
                }
            }
            plist.pop();                                        //удаляем первый элемент из очереди
        }
        while (!nlist.empty())                                  //если в очереди есть элементы                              
        {
            plist.push(nlist.front());                          //добавляем в plist первый элемент из nlist
            nlist.pop();                                        //удаляем первый элемент из очереди
        }
    }
    return false;
}


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    cout << endl << "Вы запустили программу, реализующую алгоритм быстрой трассировки методом Соукупа." << endl
        << "Работу выполнил студент группы ЭН-44 Скорописов Артем." << endl
        << "Алгоритм Соукупа является субоптимальным алгомитмом. ДРП представляет собой поле размером 10х10 ячеек с препятствиями" << endl
        << "Введите координаты терминалов S1, S2, S3 и T1, T2, T3 соответственно, в файле input " << endl;

    ifstream in("input.txt");                                                                       //считываем вхдные данные 
    ofstream out2("soukup.txt");
    in >> n;                                                                                        //размер ?
    in >> nblocks;                                                                                  //кол-во препятствий
    for (int i = 0; i < n; i++)                                                                     //заполняем ДРП 0
    {
        for (int j = 0; j < n; j++) 
        {
            Lboard[i][j] = '0';
            Sboard[i][j] = '0';
        }
    }
    memset(Lblock, false, (n * n) + 1);                                                                 //заполняем блоки памяти Lblock и Sblock 0
    memset(Sblock, false, (n * n) + 1);

    for (int i = 0; i < nblocks; i++)                                                               //расставляем препятствия из файла
    {
        in >> bx >> by;
        Lboard[bx][by] = '#';
        Sboard[bx][by] = '#';
        Lblock[bx * n + by] = true;                                                                         //хз
        Sblock[bx * n + by] = true;
    }
    bool ans[2][100];

    in >> ntarget;                                                                                  //кол-во трасс (путей) 

    cout << endl << endl;
    _getch();
    print_termenal_and_barrier();
    cout << endl << endl;

    for (int i = 0; i < ntarget; i++)                                                                   //расставляем терминалы из файла
    {
        in >> sx >> sy >> desx >> desy;                                     //считываем коор-ты источника и коор-ты приемника
        srcX[i] = sx;                                       //start_x
        srcY[i] = sy;                                       //start_y
        desX[i] = desx;                                     //target_x
        desY[i] = desy;                                     //target_y
        Lboard[sx][sy] = 'S';
        Lboard[desx][desy] = 'T';
        Sboard[sx][sy] = 'S';
        Sboard[desx][desy] = 'T';
    }
    print_termenal_and_barrier();
    cout << endl << endl;
    Timer t;
    //--------------------------for Soukup Algorithm---------------------
    out2 << "Soukup Algorithm : " << endl;

    for (int i = 0; i < ntarget; i++)                                       //для каждой пары приемник-источник
    {
        sx = srcX[i];
        sy = srcY[i];
        desx = desX[i];
        desy = desY[i];
        //for Lee's algorithm
        if (SoukupAlgo(sx, sy, desx, desy) == true)                           //find destination and return Lpath;
        {   

            int id = desx * n + desy;
            Sblock[id] = true;
            Sblock[sx * n + sy] = true;
            while (Spath[id] != -1)
            {
                Sboard[Spath[id] / n][Spath[id] % n] = '*';
                Sblock[Spath[id]] = true;
                id = Spath[id];
            }
            Sboard[sx][sy] = 'S';
            ans[1][i] = true;
        }
        else                                                         //there is no route
        {     
            Sboard[sx][sy] = 's';
            Sboard[desx][desy] = 't';
            ans[1][i] = false;
        }
    }
    double time_algotim = t.elapsed();
    
    for (int i = 0; i < n; i++)                                                     //вывод результата в файл
    {
        for (int j = 0; j < n; j++) out2 << Sboard[i][j] << " ";
        out2 << endl;
    }
    out2 << endl;
    for (int i = 0; i < ntarget; i++)                                               //если нельзя провести трассировку
    {
        if (ans[1][i] == false)
        {
            out2 << " (" << sx << "," << sy << ")->(" << desx << "," << desy << ") невозможно провести трассировку." << endl;
        }
    }

    _getch();
    print_DRP();
    cout << endl << endl;

    Print_newDRP(Sboard);
    cout <<endl << "Потрачено времени : " << time_algotim*1000000000 <<" ns" <<'\n';
    _getch();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Алгоритм Соукупа");
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1);
    glutReshapeFunc(reshape);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(Read_kb);
    glutSpecialFunc(ReadSpec);
    glutMainLoop();
   
    return 0;
}