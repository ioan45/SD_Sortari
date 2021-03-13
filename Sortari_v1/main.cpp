#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <cstring>

#define NrTeste 7
#define NrFn 6

#define SWAP(a, b)\
{\
    int aux = a;\
    a = b;\
    b = aux;\
}

using namespace std;

struct Sortare
{
    void(*Functie)(int*, unsigned) = NULL;
    char* Nume = NULL;
};

void Bubble(int* arr, unsigned N)
{
    bool sw = true;
    unsigned cnt_itr = 0;
    while(sw)
    {
        sw = false;
        for(unsigned i = 0; i < N - 1 - cnt_itr; i++)
            if(arr[i] > arr[i + 1])
            {
                SWAP(arr[i], arr[i + 1])
                sw = true;
            }
        cnt_itr++;
    }
}

void Merge(int* arr, unsigned st, unsigned dr)
{
    if(st < dr)
    {
        unsigned mij = (st + dr) / 2;

        Merge(arr, st, mij);
        Merge(arr, mij + 1, dr);

        unsigned i = st, j = mij + 1, k = 0;
        int* Aux = new int[dr - st + 1];

        while(i <= mij && j <= dr)
        {
            if(arr[i] < arr[j])
                Aux[k++] = arr[i++];
            else
                Aux[k++] = arr[j++];
        }
        while(i <= mij)
            Aux[k++] = arr[i++];
        while(j <= mij)
            Aux[k++] = arr[j++];

        for(i = 0; i < k; i++)
            arr[i + st] = Aux[i];

        delete[] Aux;
    }
}

void Merge(int* arr, unsigned N)
{
    Merge(arr, 0, N - 1);
}

void Counting(int* arr, unsigned N)
{
    int Minim = arr[0];
    int Maxim = arr[0];
    for(unsigned i = 1; i < N; i++)
    {
        if(Minim > arr[i])
            Minim = arr[i];
        if(Maxim < arr[i])
            Maxim = arr[i];
    }

    unsigned Range = Maxim - Minim + 1;
    unsigned* Cnt = new unsigned[Range];
    for(unsigned i = 0; i < Range; i++)
        Cnt[i] = 0;
    for(unsigned i = 0; i < N; i++)
        Cnt[arr[i] - Minim]++;

    for(unsigned k = 0, i = 0; i < Range; i++)
        for(unsigned j = 0; j < Cnt[i]; j++)
            arr[k++] = i + Minim;

    delete[] Cnt;
}

void Quick(int* arr, int st, int dr)
{
    if(st < dr)
    {
        unsigned mij = (st + dr) / 2;
        if(arr[dr] > arr[st])
            SWAP(arr[dr], arr[st])
        if(arr[mij] > arr[st])
            SWAP(arr[dr], arr[mij])
        if(arr[mij] < arr[dr])
            SWAP(arr[mij], arr[dr])

        int pivot = arr[st];

        // Se muta in dreapta elementele mai mari decat pivotul

        int i = dr; //pozitia din dreapta unde se poate muta urmatorul element mai mare decat pivotul
        for(int j = dr; j > st; j--)
            if(arr[j] > pivot)
            {
                SWAP(arr[i], arr[j])
                i--;
            }
        SWAP(arr[i], arr[st])

        // i - pozitia pivotului dupa partitionare

        Quick(arr, st, i - 1);
        Quick(arr, i + 1, dr);
    }
}

void Quick(int* arr, unsigned N)
{
    Quick(arr, 0, N - 1);
}

void Radix(int* arr, unsigned N)
{
    const unsigned Baza = 16;
    const unsigned log2_Baza = 4;

    int Maxim = arr[0];
    for(unsigned i = 1; i < N; i++)
        if(Maxim < arr[i])
            Maxim = arr[i];

    int** container = new int*[Baza];
    for(unsigned i = 0; i < Baza; i++)
        container[i] = new int[N];
    unsigned contor[Baza];

    unsigned cif = 0;
    while(Maxim)
    {
        for(unsigned i = 0; i < Baza; i++)
            contor[i] = 0;
        for(unsigned rest, i = 0; i < N; i++)
        {
            rest = (arr[i] >> cif) & (Baza - 1);
            container[rest][contor[rest]++] = arr[i];
        }
        unsigned k = 0;
        for(unsigned i = 0; i < Baza; i++)
            for(unsigned j = 0; j < contor[i]; j++)
                arr[k++] = container[i][j];

        Maxim = Maxim >> log2_Baza;
        cif += log2_Baza;
    }

    for(unsigned i = 0; i < Baza; i++)
        delete[] container[i];
    delete[] container;
}

void Insertion(int* arr, unsigned N)
{
    for(unsigned i = 1; i < N; i++)
    {
        int NrCurent = arr[i];
        int k = i - 1;

        while(k >= 0 && arr[k] > NrCurent)
        {
            arr[k + 1] = arr[k];
            k--;
        }
        arr[k + 1] = NrCurent;
    }
}

void Heapify(int* arr, unsigned N, unsigned idx_parinte)
{
    unsigned fiu_st = 2 * idx_parinte + 1;
    if(fiu_st < N)
    {
        unsigned Max_idx = fiu_st;

        unsigned fiu_dr = fiu_st + 1;
        if(fiu_dr < N && arr[fiu_st] < arr[fiu_dr])
            Max_idx = fiu_dr;

        if(arr[idx_parinte] < arr[Max_idx])
        {
            SWAP(arr[idx_parinte], arr[Max_idx])
            Heapify(arr, N, Max_idx);
        }
    }
}

void Heap(int* arr, unsigned N)
{
    for(int i = N / 2 - 1; i >= 0; i--)
        Heapify(arr, N, i);

    for(unsigned i = N - 1; i > 0; i--)
    {
        SWAP(arr[0], arr[i])
        Heapify(arr, i, 0);
    }
}

void Intro(int* arr, unsigned N, int st, int dr, unsigned adancime)
{
    if(N < 17)
        Insertion(arr, N);
    else if(!adancime)
        Heap(arr, N);
    else if(st < dr)
    {
        // ~~Partea de Quicksort~~

        // Mediana din 3

        unsigned mij = (st + dr) / 2;
        if(arr[st] > arr[mij])
            SWAP(arr[st], arr[mij])
        if(arr[dr] > arr[mij])
            SWAP(arr[dr], arr[mij])
        if(arr[dr] < arr[st])
            SWAP(arr[dr], arr[st])

        // Partitionare

        int pivot = arr[dr];
        int lt = st - 1, rt = dr;
        while(1)
        {
            do
                lt++;
            while(arr[lt] < pivot);
            do
                rt--;
            while(arr[rt] > pivot);

            if(lt > rt)
                break;
            SWAP(arr[lt], arr[rt])
        }
        SWAP(arr[lt], arr[dr])

        Intro(arr, N, st, lt - 1, adancime - 1);
        Intro(arr, N, lt + 1, dr, adancime - 1);
    }

}

void Intro(int* arr, unsigned N)
{
    if(N)
    {
        unsigned n = N, adancime = 0;
        while(n > 1)
        {
            n = n >> 1;
            adancime++;
        }
        Intro(arr, N, 0, N - 1, 2 * adancime);
    }
}

int Generator()
{
    return rand() % 10000000;
}

void Copie(int* destinatie, int* sursa, unsigned N)
{
    for(unsigned i = 0; i < N; i++)
        destinatie[i] = sursa[i];
}

bool Sortare_Ok(int* arr, unsigned N)
{
    for(unsigned i = 1; i < N; i++)
        if(arr[i] < arr[i - 1])
            return false;
    return true;
}

bool Timpi_Testare(double Timpi[][NrTeste], const Sortare* f)
{
    using namespace std::chrono;

    srand(time(NULL));

    bool Reusit = true;
    for(unsigned N = 10, T = 0; T < NrTeste; T++, N *= 10)
    {
        int* arr = new int[N];
        generate(arr, arr + N, Generator);

        bool Stop = true;
        for(unsigned Fn = 0; Fn < NrFn; Fn++)
        {
            if(T > 0 && (Timpi[Fn][T - 1] == -1 || Timpi[Fn][T - 1] > 10000))
                Timpi[Fn][T] = -1;
            else
            {
                int* Dublura = new int[N];
                Copie(Dublura, arr, N);

                time_point<steady_clock> Start = steady_clock::now();

                f[Fn].Functie(Dublura, N);

                time_point<steady_clock> End = steady_clock::now();

                if(!Sortare_Ok(Dublura, N))
                {
                    cout << "Algoritm incorect: " << f[Fn].Nume << '\n';
                    Reusit = false;
                    Stop = true;
                    delete[] Dublura;
                    break;
                }

                duration<double, milli> Durata = End - Start;
                Timpi[Fn][T] = Durata.count();

                delete[] Dublura;
                Stop = false;
            }
        }
        delete[] arr;
        if(Stop)
            break;
    }
    return Reusit;
}

void Timpi_Afisare(double Timpi[][NrTeste], const Sortare* f)
{
    unsigned Col_Width = 15;
    unsigned Name_Width = 0;

    for(unsigned i = 0; i < NrFn; i++)
    {
        unsigned len = strlen(f[i].Nume);
        if(len > Name_Width)
            Name_Width = len;
    }
    Name_Width++;

    cout << setw(Name_Width) << ' ';
    for(unsigned N = 10, i = 0; i < NrTeste; i++, N *= 10)
        cout << setw(Col_Width) << "N = " + to_string(N);
    cout << endl;
    for(unsigned i = 0; i < NrFn; i++)
    {
        cout << setw(Name_Width) << left << f[i].Nume << right;
        for(unsigned j = 0; j < NrTeste; j++)
            if(Timpi[i][j] != -1)
                cout << setw(Col_Width) << fixed << setprecision(4) << Timpi[i][j];
            else
                cout << setw(Col_Width) << "-----";
        cout << endl;
    }
    cout << "\n--Unitate de masura: Milisecunda\n";
}

int main()
{
    Sortare f[NrFn];
    f[0].Functie = Bubble;
    f[0].Nume = (char*)"Bubble";
    f[1].Functie = Merge;
    f[1].Nume = (char*)"Merge";
    f[2].Functie = Counting;
    f[2].Nume = (char*)"Counting";
    f[3].Functie = Quick;
    f[3].Nume = (char*)"Quick";
    f[4].Functie = Radix;
    f[4].Nume = (char*)"Radix";
    f[5].Functie = Intro;
    f[5].Nume = (char*)"Intro";

    double Timpi[NrFn][NrTeste];

    cout << "Running...\n";
    bool Succes = Timpi_Testare(Timpi, f);
    if(Succes)
        Timpi_Afisare(Timpi, f);

    return 0;
}
