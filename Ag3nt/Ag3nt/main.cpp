#include <cstdio>
#include <iostream>
#include <ctime>

#define T uint64_t
T iterations = 0;

using namespace std;

T TheLongestLine(T& sum, T& n)
{
    if (sum == 0) return 0;
    n++;
    if (sum > ((0.5 + 0.5 * n) * n)) TheLongestLine(sum, n);
    return n;
}

void zada4a_Ag3nta(T& amount, T line, T& level, T shift)
{
    iterations++;
    level = 0;
    while (TheLongestLine(shift, level) < (line - shift))
    {
        if (shift > 2)
        {
            if (shift < line - shift) zada4a_Ag3nta(amount, shift, level, 0);
            else zada4a_Ag3nta(amount, shift, level, shift - line + shift + 1);
        }
        else
        {
            amount++;
            level = 0;
        }
        shift++;
    }
}

struct A
{
    char c;
    uint64_t d;
    int s;
};

bool are_same(const string& name1, const string& name2) // compare the names including the registry
{
    if (name1.size() != name2.size()) return false;
    int size = name1.size();
    for (int i(0); i < size; i++)
    {
        if (((name1[i] >= 65) && (name1[i] <= 90)) || ((name2[i] >= 65) && (name2[i] <= 90)))
        {
            if (abs((int)(name1[i] - name2[i])) != 32) return false;
        }
        else if (name1[i] != name2[i]) return false;
    }
    return true;
}

void main()
{
 /*   {
        int time1 = time(NULL);
        /*    uint32_t* x = new uint32_t[100];
            for (int i(0); i < 100; i++)
            {
                uint32_t* skoka = new uint32_t;
                *skoka = 0;
                zada4a_Ag3nta(*skoka, i);
                x[i] = *skoka;
                delete skoka;
            }
            for (int i(0); i < 100; i++)
                std::cout << i << " " << x[i] << "/n"; 
        while (true)
        {
            int x;
            cin >> x;
            T y = 0, level = 0;
            int time1 = time(NULL);
            zada4a_Ag3nta(y, x, level, 0);
            cout << x << " " << (int)y << "\n";
            int time2 = time(NULL);
            cout << "Took " << time2 - time1 << " to calculate.\n";
            cout << "Took " << (T)(iterations / 1000000) << "*10^6 iterations to calculate\n";
            iterations = 0;
        }

        //   napishi_na_ekran << (uint32_t)skoka << novaya_stroka;


         //  delete[] x; 
    }*/
   // A structura = {1, 1, 1};
  //  A* b = &structura;
  //  cout << sizeof(structura) << " " << (int)&structura << endl;
    /*/
    while (true)
    {
        int n, iterations(0);
        cin >> n;
        long* q = new long[n + 1];
        for (int i(0); i < n + 1; i++) q[i] = 0;
        q[0] = 1;
        int time1 = time(NULL);
        for (int i = 1; i <= n; ++i)
        {
            for (int j = n; j >= i; --j)
            {
                q[j] += q[j - i];
                iterations++;
            }
            for (int i(1); i < n + 1; i++) cout << q[i] << (char)9;
            cout << endl;
        }
        int time2 = time(NULL);
        cout << (int)q[n] << endl;
        cout << "Took " << time2 - time1 << " seconds to calculate\n";
        cout << "Took " << iterations << " iterations to calculate\n";
        delete[] q;
    } 
 /*   int n = 215;
    long* q = new long[n + 1];
    for (int i(0); i < n + 1; i++) q[i] = 0;
    q[0] = 1;
    for (int i = 1; i <= n; ++i)
    {
        for (int j = n; j >= i; --j)
        {
            q[j] += q[j - i];
        }
    }
    for (int i(0); i < n + 1; i++)
        cout << i << " => " << (int)q[i] << endl;
    delete[] q;*/

  /*  time_t t;
    struct tm tmp;
    char MY_TIME[11];

    time(&t);
    localtime_s(&tmp, &t);

    // using strftime to display time 
    strftime(MY_TIME, sizeof(MY_TIME), "%F", &tmp);

    cout << MY_TIME << endl;

    tmp.tm_year = 2020 - 1900;
    tmp.tm_mon = 3 - 1;
    tmp.tm_mday = 15;
    cout << mktime(&tmp) << endl;*/

    cout << are_same("helllo", "HeLLo") << endl;
    

    system("PAUSE");
}
