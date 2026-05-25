#include "functions.h"

using namespace std;

//funkcje pomocnicze do walidacji danych wejściowych
double wczytajLiczbe(const string& prompt, double min = 0.0) {
    double wartosc;
    while (true) {
        cout << prompt;
        if (cin >> wartosc) { //zwraca false jak cos innego niz liczba
            if (wartosc >= min) {
                break;
            } else {
                cout << "  Wartosc nie moze byc ujemna. Sprobuj ponownie.\n";
            }
        } else {
            cout << "  To nie jest poprawna liczba. Sprobuj ponownie.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return wartosc;
}

int wczytajLiczbeCalkowita(const string& prompt, int min = 0) {
    int wartosc;
    while (true) {
        cout << prompt;
        if (cin >> wartosc) {
            if (wartosc >= min) {
                break;
            } else {
                cout << "  Wartosc nie moze byc mniejsza niz " << min << ". Sprobuj ponownie.\n";
            }
        } else {
            cout << "  To nie jest poprawna liczba calkowita. Sprobuj ponownie.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return wartosc;
}

char wczytajStatus() {
    char status;
    while (true) {
        cout << "Podaj status pozycji (D - dostepna, W - wypozyczona): ";
        cin >> status;
        status = toupper(status); //akceptuje tylko D i W(mala czcionka tez)
        if (status == 'D' || status == 'W')
            break;
        cout << "  Niepoprawny status. Dozwolone wylacznie: D lub W.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return status;
}

//globalna baza danych systemu
Biblioteka<Pozycja> biblioteka;

int main() {
    //tablica stałych kategorii oparta o std::array (4 pozycje)
    array<string, 4> kategorie = {"Ksiazka", "Film", "Gra", "Inne (Wlasne)"};

    int opcja = 0;
    while (opcja != 5) { //glowna petla(dziala dopoki uzytkownik nie wybierze opcji 5)
        cout << "\n\n";
        wypisz_menu();

        if (!(cin >> opcja)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue; //wraca na poczatek petli
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcja) {  //konkretne przypadki 
            case 1: {
                cout << "\n\n--- AKTUALNE ZASOBY BIBLIOTEKI ---\n\n";
                if (biblioteka.empty())
                    cout << "  Brak zarejestrowanych pozycji w systemie.\n";
                else {
                    cout << biblioteka;
                }
                cout << "\n--- GLOBALNE STATYSTYKI ---\n";
                wypisz_statystyki(biblioteka);
                break;
            }
            case 2: {
                cout << "\n\n--- PRZEGLAD ZASOBOW WG KATEGORII ---\n";
                for (const auto& kat : kategorie) {
                    if (kat == "Inne (Wlasne)")
                        continue;
                    cout << "\n[" << kat << "]:\n";
                    bool znalazlem = false;
                    for (const auto* p : biblioteka) {
                        if (p->getKategoria() == kat) {
                            p->wyswietl();
                            znalazlem = true;
                        }
                    }
                    if (!znalazlem) cout << "  (brak pozycji w tej kategorii)\n";
                }
                      //pozycja jest inna jak jej kategoria nie pasuje do zadnej z 3 opcji standardowych
                cout << "\n[Inne Wlasne Kategorie Zdefiniowane Przez Uzytkownika]:\n";
                bool inne_znalazlem = false;
                for (const auto* p : biblioteka) { //zakresowa petla for
                    bool standardowa = false;
                    for (size_t i = 0; i < 3; ++i) {           //sprawdzamy 3 wbudowane kategorie
                        if (p->getKategoria() == kategorie[i]) {
                            standardowa = true;
                            break;
                        }
                    }
                    if (!standardowa) {
                        p->wyswietl();
                        inne_znalazlem = true;
                    }
                }
                if (!inne_znalazlem) cout << "  (brak pozycji)\n";
                break;
            }
                                    //dodawanie nowego zasobu do biblioteki
            case 3: {
                cout << "\n\n--- REJESTRACJA NOWEGO ZASOBU ---\n\n";
                cout << "Wybierz kategoria rejestracji:\n";
                for (size_t i = 0; i < kategorie.size(); ++i)
                    cout << "  " << i + 1 << ". " << kategorie[i] << "\n";
                cout << "\nPodaj numer (1-4): ";
                int kat_wybor;
                cin >> kat_wybor;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (kat_wybor < 1 || kat_wybor > 4) {
                    cout << "Bledny wybor kategorii zasobu!\n";
                    break;
                }

                string wybrana_kat = kategorie[kat_wybor - 1];
                string tytul, isbn;
                double cenaDoba;

                cout << "\n>>> Tworzenie pozycji w kategorii: " << wybrana_kat << " <<<\n\n";

                while (true) {
                    cout << "Podaj tytul: ";
                    getline(cin, tytul);
                    if (!tytul.empty())
                        break;
                    cout << "  Tytul nie moze byc pusty!\n";
                }

                while (true) {
                    cout << "Podaj ISBN / Unikalne ID: ";
                    getline(cin, isbn);
                    if (!isbn.empty())
                        break;
                    cout << "  Identyfikator nie moze byc pusty!\n";
                }

                //scenariusz 1: ksiazka
                if (kat_wybor == 1) {
                    string autor;
                    int rok;
                    cout << "Podaj autora ksiazki: ";
                    getline(cin, autor);
                    rok = wczytajLiczbeCalkowita("Podaj rok wydania ksiazki: ", 0);
                    char status = wczytajStatus();
                    cenaDoba = wczytajLiczbe("Podaj cene za dobe najmu (zl): ", 0.0);
                    biblioteka.dodaj(new Ksiazka(tytul, isbn, cenaDoba, status, autor, rok));
                }
                //scenariusz 2: film
                else if (kat_wybor == 2) {
                    string rezyser;
                    int czas;
                    cout << "Podaj rezysera filmu: ";
                    getline(cin, rezyser);
                    czas = wczytajLiczbeCalkowita("Podaj czas trwania filmu (minuty): ", 1);
                    char status = wczytajStatus();
                    cenaDoba = wczytajLiczbe("Podaj cene za dobe najmu (zl): ", 0.0);
                    biblioteka.dodaj(new Film(tytul, isbn, cenaDoba, status, rezyser, czas));
                }
                //scenariusz 3: gra
                else if (kat_wybor == 3) {
                    string platforma;
                    int wiek;
                    cout << "Podaj platforme docelowa (np. PC, PS5): ";
                    getline(cin, platforma);
                    wiek = wczytajLiczbeCalkowita("Podaj minimalny wiek gracza: ", 0);
                    char status = wczytajStatus();
                    cenaDoba = wczytajLiczbe("Podaj cene za dobe najmu (zl): ", 0.0);
                    biblioteka.dodaj(new Gra(tytul, isbn, cenaDoba, status, platforma, wiek));
                }
                //scenariusz 4: wlasna dynamiczna kategoria
                else if (kat_wybor == 4) {
                    string wlasna_kat, info;
                    cout << "Podaj nazwe nowej kategorii (np. Czasopismo): ";
                    getline(cin, wlasna_kat);
                    cout << "Podaj dodatkowe informacje opisowe: ";
                    getline(cin, info);
                    char status = wczytajStatus();
                    cenaDoba = wczytajLiczbe("Podaj cene za dobe najmu (zl): ", 0.0);
                    biblioteka.dodaj(new DynamicznaPozycja(tytul, isbn, cenaDoba, status, wlasna_kat, info));
                }

                cout << "\nZasob zostal poprawnie zainicjalizowany i dodany do bazy.\n";
                break;
            }
                                 //wypozyczanie i zwracanie
            case 4: {
                cout << "\n\n--- PANEL WYPOZYCZEN I ZWROTOW (OBSLUGA WYJATKOW) ---\n\n";
                cout << "1. Wypozycz pozycje\n";
                cout << "2. Zwroc pozycje\n";
                int tryb = wczytajLiczbeCalkowita("Wybierz czynnosc (1-2): ", 1);

                string identyfikator;
                cout << "Podaj Tytul lub numer ISBN/ID pozycji: ";
                getline(cin, identyfikator);

                try {     //obsluguje wyjatki(try-catch) zeby nie crashowal przy blednych danych tylko wypisuje komunikat
                    if (tryb == 1) {
                        biblioteka.wypozycz(identyfikator);
                        cout << "Sukces: Pozycja \"" << identyfikator << "\" zostala pomyślnie wypozyczona.\n";
                    } else if (tryb == 2) {
                        biblioteka.zwroc(identyfikator);
                        cout << "Sukces: Pozycja \"" << identyfikator << "\" zostala pomyślnie zwrocona do bazy.\n";
                    } else {
                        cout << "Niepoprawny wybor operacji w panelu.\n";
                    }
                }
                catch (const out_of_range& e) { //nie istnieje w bazie
                    cout << "Blad krytyczny wyszukiwania (out_of_range): " << e.what() << "\n";
                }
                catch (const logic_error& e) { //pozycja isnieje ale operacja jest logicznie niemozliwa
                    cout << "Blad stanu logicznego obiektu (logic_error): " << e.what() << "\n";
                }
                break;
            }
        }
            //po kazdej operacji oprocz 5(wyjscie) czekamy az uzytkownik kliknie enter zanym wyswietli menu
        if (opcja != 5) {
            cout << "\nNacisnij ENTER aby kontynuowac...";
            czekaj_na_enter();
        }
    }
              //wyswietlenie raportu
    cout << "\n\n=== RAPORT KONCOWY ZAMKNIECIA SYSTEMU (SORTOWANIE PO CENIE DOBOWEJ) ===\n\n";
    biblioteka.sortujPoCenie();
    cout << biblioteka;
    return 0;
}
