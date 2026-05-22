#include "functions.h"
using namespace std;

//konstruktor klasy bazowej Pozycja
Pozycja::Pozycja(const string& t, const string& i, double c, char s)
    : tytul(t), isbn(i), cenaDoba(c), status(s) {}

//klasa ksiazka
Ksiazka::Ksiazka(const string& t, const string& i, double c, char s, const string& a, int r)
    : Pozycja(t, i, c, s), autor(a), rokWydania(r) {}

Ksiazka::Ksiazka(const Ksiazka& other)
    : Pozycja(other), autor(other.autor), rokWydania(other.rokWydania) {}

void Ksiazka::wyswietl() const {
    cout << "  [Ksiazka]    Tytul: " << tytul << " | ISBN: " << isbn << " | Autor: " << autor
         << " | Rok: " << rokWydania << " | Status: " << status
         << " | Cena/Doba: " << cenaDoba << " zl\n";
}
string Ksiazka::getKategoria() const { return "Ksiazka"; }
Pozycja* Ksiazka::clone() const { return new Ksiazka(*this); }

//klasa film
Film::Film(const string& t, const string& i, double c, char s, const string& r, int cz)
    : Pozycja(t, i, c, s), rezyser(r), czasTrwania(cz) {}

Film::Film(const Film& other)
    : Pozycja(other), rezyser(other.rezyser), czasTrwania(other.czasTrwania) {}

void Film::wyswietl() const {
    cout << "  [Film]       Tytul: " << tytul << " | ID: " << isbn << " | Rezyser: " << rezyser
         << " | Czas: " << czasTrwania << " min | Status: " << status << " | Cena/Doba: " << cenaDoba << " zl\n";
}
string Film::getKategoria() const { return "Film"; }
Pozycja* Film::clone() const { return new Film(*this); }

//klasa gra
Gra::Gra(const string& t, const string& i, double c, char s, const string& p, int w)
    : Pozycja(t, i, c, s), platforma(p), minWiek(w) {}

Gra::Gra(const Gra& other)
    : Pozycja(other), platforma(other.platforma), minWiek(other.minWiek) {}

void Gra::wyswietl() const {
    cout << "  [Gra]        Tytul: " << tytul << " | ID: " << isbn << " | Platforma: " << platforma
         << " | Wiek: " << minWiek << "+ | Status: " << status << " | Cena/Doba: " << cenaDoba << " zl\n";
}
string Gra::getKategoria() const { return "Gra"; }
Pozycja* Gra::clone() const { return new Gra(*this); }

//klasa DynamicznaPozycja
DynamicznaPozycja::DynamicznaPozycja(const string& t, const string& i, double c, char s, const string& k, const string& info)
    : Pozycja(t, i, c, s), kat_nazwa(k), dodatkowe_info(info) {}

DynamicznaPozycja::DynamicznaPozycja(const DynamicznaPozycja& other)
    : Pozycja(other), kat_nazwa(other.kat_nazwa), dodatkowe_info(other.dodatkowe_info) {}

void DynamicznaPozycja::wyswietl() const {
    cout << "  [" << kat_nazwa << "] " << tytul << " | ID: " << isbn << " | Info: " << dodatkowe_info
         << " | Status: " << status << " | Cena/Doba: " << cenaDoba << " zl\n";
}
string DynamicznaPozycja::getKategoria() const { return kat_nazwa; }
Pozycja* DynamicznaPozycja::clone() const { return new DynamicznaPozycja(*this); }

//funkcje pomocnicze wejscia/wyjscia
void czekaj_na_enter() {
    string s;
    getline(cin, s);
}

void wypisz_menu() {
    array<string, 5> opcje = {
        "(1). Pokaz wszystkie zasoby i statystyki biblioteki",
        "(2). Pokaz zasoby podzielone strukturalnie na kategorie",
        "(3). Dodaj nowa pozycje (Ksiazka, Film, Gra, Wlasna)",
        "(4). Wypozycz / Zwroc pozycje z biblioteki (Wyjatki)",
        "(5). Wyjdz z programu i wygeneruj raport koncowy"
    };
    cout << "--- SYSTEM ZARZADZANIA WYPOZYCZALNIA / BIBLIOTEKA ---\n\n";
    for (const auto& opcja : opcje)
        cout << "  " << opcja << "\n";
    cout << "\nWybierz opcje: ";
}
