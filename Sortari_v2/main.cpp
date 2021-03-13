#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <string>

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

void Quick_v1(int* arr, int st, int dr, unsigned adancime, bool& Limitat)
{
    ///Quick cu pivot ales ca fiind elementul de la capatul din dreapta

    if(st < dr)
    {
        int pivot = arr[dr];

        int i = st;
        for(int j = st; j < dr; j++)
            if(arr[j] < pivot)
            {
                SWAP(arr[i], arr[j])
                i++;
            }
        SWAP(arr[i], arr[dr])

        if(adancime < 15000)
        {
            Quick_v1(arr, st, i - 1, adancime + 1, Limitat);
            Quick_v1(arr, i + 1, dr, adancime + 1, Limitat);
        }
        else
            Limitat = true;
    }
}

void Quick_v1(int* arr, unsigned N)
{
    bool Limitat;
    Quick_v1(arr, 0, N - 1, 1, Limitat);

    if(Limitat)
        cout << "(i) Recursivitate: Adancimea limita atinsa!" << '\n' << setw(23) << ' ';
}

void Quick_v2(int* arr, int st, int dr, unsigned adancime, bool& Limitat)
{
    ///Quick cu pivot ales ca fiind mediana din 3

    if(st < dr)
    {
        unsigned mij = (st + dr) / 2;
        if(arr[mij] < arr[st])
            SWAP(arr[mij], arr[st])
        if(arr[dr] > arr[mij])
            SWAP(arr[dr], arr[mij])
        if(arr[dr] < arr[st])
            SWAP(arr[dr], arr[st])

        int pivot = arr[dr];

        int i = st;
        for(int j = st; j < dr; j++)
            if(arr[j] < pivot)
            {
                SWAP(arr[i], arr[j])
                i++;
            }
        SWAP(arr[i], arr[dr])

        if(adancime < 15000)
        {
            Quick_v2(arr, st, i - 1, adancime + 1, Limitat);
            Quick_v2(arr, i + 1, dr, adancime + 1, Limitat);
        }
        else
            Limitat = true;
    }
}

void Quick_v2(int* arr, unsigned N)
{
    bool Limitat;
    Quick_v2(arr, 0, N - 1, 1, Limitat);

    if(Limitat)
        cout << "(i) Recursivitate: Adancimea limita atinsa!" << '\n' << setw(23) << ' ';
}

void Radix(int* arr, unsigned N, unsigned Baza, unsigned log2_Baza)
{
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

void Radix_v1(int* arr, unsigned N)
{
    const unsigned Baza = 2;
    const unsigned log2_Baza = 1;

    Radix(arr, N, Baza, log2_Baza);
}

void Radix_v2(int* arr, unsigned N)
{
    const unsigned Baza = 16;
    const unsigned log2_Baza = 4;

    Radix(arr, N, Baza, log2_Baza);
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

void std_sort(int* arr, unsigned N)
{
    sort(arr, arr + N);
}

void Generator(int* arr, unsigned N, unsigned Maxim)
{
    Maxim++;
    for(unsigned i = 0; i < N; i++)
        arr[i] = rand() % Maxim;
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

int main()
{
    using namespace std::chrono;

    const unsigned NrFn = 9;

    Sortare f[NrFn];
    f[0].Functie = Merge;
    f[0].Nume = (char*)"Merge";
    f[1].Functie = Counting;
    f[1].Nume = (char*)"Counting";
    f[2].Functie = Intro;
    f[2].Nume = (char*)"Intro";
    f[3].Functie = Radix_v1;
    f[3].Nume = (char*)"Radix <Baza 2>";
    f[4].Functie = Radix_v2;
    f[4].Nume = (char*)"Radix <Baza 16>";
    f[5].Functie = Quick_v1;
    f[5].Nume = (char*)"Quick <ultim elem.>";
    f[6].Functie = Quick_v2;
    f[6].Nume = (char*)"Quick <med. din 3>";
    f[7].Functie = Bubble;
    f[7].Nume = (char*)"Bubble";
    f[8].Functie = std_sort;
    f[8].Nume = (char*)"C++ sort()";

    ifstream Fisier_Teste("Teste.txt");
    unsigned NrTeste, N, Maxim;

    srand(time(NULL));
    Fisier_Teste >> NrTeste;
    for(unsigned T = 0; T < NrTeste; T++)
    {
        Fisier_Teste >> N >> Maxim;
        cout << "\n\n--N = " << N << "\n--Max = " << Maxim;

        int* arr = new int[N];
        Generator(arr, N, Maxim);

        for(unsigned i = 0; i < NrFn; i++)
        {
            cout << '\n' << setw(23) << left << f[i].Nume + (string)":";
            int* Dublura = new int[N];
            Copie(Dublura, arr, N);

            time_point<steady_clock> Start = steady_clock::now();

            f[i].Functie(Dublura, N);

            time_point<steady_clock> End = steady_clock::now();

            duration<double, milli> Durata = End - Start;

            if(!Sortare_Ok(Dublura, N))
                cout << setw(11) << "Esuat!";
            else
                cout << setw(11) << "Sortare OK!";
            cout << " | Durata: " << setw(13) << right << fixed << setprecision(4) << Durata.count() << " ms";

            delete[] Dublura;
        }
        delete[] arr;
    }

    Fisier_Teste.close();
    return 0;
}
