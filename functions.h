#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <stdexcept>

using namespace std;       //umozliwia pisanie cout, cin, vector, string itp. bez std::

//klasa abstrakcyjna pozycja
class Pozycja {
protected:     //klasy dziedziczace maja bezposredni dostep do tych pol
    string tytul;
    string isbn;      //unikalny identyfikator (ISBN dla ksiazek, ID dla innych)
    double cenaDoba;   //koszt wypozyczenia za dobę
    char   status;     //'D' - dostepna, 'W' - wypozyczona

public:
    //konstruktor przypisujacy odpowiednie wartosci
    Pozycja(const string& t, const string& i, double c, char s); //const string& unika niepotrzebnego kopiowania

    //wirtualny destruktor
    virtual ~Pozycja() = default;

    //wirtualne metody do polimorficznego zarzadzania obiektami
    virtual void        wyswietl()     const = 0;
    virtual string      getKategoria() const = 0;
    virtual Pozycja* clone()        const = 0;

    //gettery
    string getTytul()    const { return tytul; }
    string getIsbn()     const { return isbn; }
    double getCenaDoba() const { return cenaDoba; }
    char   getStatus()   const { return status; }
    void   setStatus(char s)   { status = s; }

    //przeciazone operatory (do porownywania i sortowania pozycji)
    bool operator<(const Pozycja& o) const { return cenaDoba < o.cenaDoba; }
    bool operator==(const Pozycja& o) const { return tytul == o.tytul || isbn == o.isbn; }
};

//klasy dziedziczace po pozycjach
                                     //ksiazka
class Ksiazka : public Pozycja {
    string autor;
    int rokWydania;
public:
    Ksiazka(const string& t, const string& i, double c, char s, const string& a, int r);
    Ksiazka(const Ksiazka& other);    //konstruktor kopiujacy
    void wyswietl() const override;    //nadpisanie metody wirtualnej
    string getKategoria() const override;
    Pozycja* clone() const override;
};

                                     //film
class Film : public Pozycja {
    string rezyser;
    int czasTrwania; //w minutach
public:
    Film(const string& t, const string& i, double c, char s, const string& r, int cz);
    Film(const Film& other);
    void wyswietl() const override;
    string getKategoria() const override;
    Pozycja* clone() const override;
};

                                     //gra
class Gra : public Pozycja {
    string platforma;
    int minWiek;
public:
    Gra(const string& t, const string& i, double c, char s, const string& p, int w);
    Gra(const Gra& other);
    void wyswietl() const override;
    string getKategoria() const override;
    Pozycja* clone() const override;
};

                   //DynamicznaPozycja (dla niestandardowych kategorii uzytkownika)
class DynamicznaPozycja : public Pozycja {
    string kat_nazwa;
    string dodatkowe_info;
public:
    DynamicznaPozycja(const string& t, const string& i, double c, char s, const string& k, const string& info);
    DynamicznaPozycja(const DynamicznaPozycja& other);
    void wyswietl() const override;
    string getKategoria() const override;
    Pozycja* clone() const override;
};

//szablon klasy biblioteka
template <typename T>
class Biblioteka {
private:
    vector<T*> zasoby;      //wektor wskaznikow umozliwiajacy polimorfizm

public:
    //domyslny konstruktor
    Biblioteka() = default;

    //destruktor (zwalnia dynamicznie alokowana pamiec (RAII))
    ~Biblioteka() {
        for (auto* p : zasoby)
            delete p;
    }

    //konstruktor kopiujacy (glebokie kopiowanie z uzyciem wzorca Prototype (clone))
    Biblioteka(const Biblioteka& other) {
        for (const auto* p : other.zasoby) {
            zasoby.push_back(p->clone());
        }
    }

    //operator przypisania chroniacy przed samoprzypisaniem
    Biblioteka& operator=(const Biblioteka& other) {
        if (this != &other) {
            for (auto* p : zasoby)
                delete p;
            zasoby.clear();
            for (const auto* p : other.zasoby)
                zasoby.push_back(p->clone());
        }
        return *this;
    }

    //dodanie nowej pozycji
    void dodaj(T* p) {
        zasoby.push_back(p);
    }

    //wypozyczenie pozycji (wyszukiwanie przez find_if i rzucanie wyjatkow)
    void wypozycz(const string& wzorzec) {
        auto it = find_if(zasoby.begin(), zasoby.end(), [&wzorzec](T* p) {
            return p->getTytul() == wzorzec || p->getIsbn() == wzorzec;
        });

        if (it == zasoby.end())
            throw out_of_range("Brak pozycji o podanym tytule/ISBN w bazie danych!");

        if ((*it)->getStatus() == 'W')
            throw logic_error("Ta pozycja jest aktualnie juz wypozyczona!");

        (*it)->setStatus('W');
    }

    //zwrot pozycji z obsluga wyjatkow
    void zwroc(const string& wzorzec) {
        auto it = find_if(zasoby.begin(), zasoby.end(), [&wzorzec](T* p) {
            return p->getTytul() == wzorzec || p->getIsbn() == wzorzec;
        });

        if (it == zasoby.end())
            throw out_of_range("Brak pozycji o podanym tytule/ISBN w bazie danych!");

        if ((*it)->getStatus() == 'D')
            throw logic_error("Ta pozycja nie jest wypozyczona! Znajduje sie w bibliotece.");

        (*it)->setStatus('D');
    }

    //sortowanie po cenie dobowej najmu (uzywa przeciazonego operatora <)
    void sortujPoCenie() {
        sort(zasoby.begin(), zasoby.end(), [](T* a, T* b) { return *a < *b; });
    }

    //metody pomocnicze
    bool empty() const { return zasoby.empty(); }
    const vector<T*>& getZasoby() const { return zasoby; }

    //iteratory dla zakresowej petli for
    auto begin() const { return zasoby.begin(); }
    auto end()   const { return zasoby.end(); }

    //deklaracja zaprzyjaznionego operatora strumieniowego
    template <typename U>
    friend ostream& operator<<(ostream& os, const Biblioteka<U>& bib);
};

//definicja zaprzyjaznionego operatora << dla klasy Biblioteka
template <typename T>
ostream& operator<<(ostream& os, const Biblioteka<T>& bib) {
    for (const auto* p : bib.zasoby) {
        p->wyswietl();   //wywolanie polimorficzne
    }
    return os;
}

//szablon funkcji (statystyki)
template <typename T>
void wypisz_statystyki(const Biblioteka<T>& bib) {
    const auto& v = bib.getZasoby();
    if (v.empty()) {
        cout << "  Biblioteka nie posiada zadnych zasobow.\n";
        return;
    }

    //uzycie algorytmu STL max_element
    auto mx = max_element(v.begin(), v.end(), [](T* a, T* b) { return *a < *b; });
    double suma = 0;

    for (const auto* p : v)
        suma += p->getCenaDoba();

    cout << "  Liczba wszystkich pozycji: " << v.size() << "\n";
    cout << "  Najdrozszy najem (doba):   " << (*mx)->getTytul() << " (" << (*mx)->getCenaDoba() << " zl)\n";
    cout << "  Laczny potencjalny dochod dobowy: " << suma << " zl\n";
}

//funkcje pomocnicze menu
void czekaj_na_enter();
void wypisz_menu();

#endif //FUNCTIONS_H
