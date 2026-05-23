# System zarządzania biblioteką/wypożczalnią - Projekt C++

## Opis programu

Program konsolowy w C++ symulujący system wypożyczalni multimediów. Umożliwia zarządzanie zasobami trzech wbudowanych kategorii (książka, film, gra) oraz dowolnych kategorii definiowanych przez użytkownika.

## Struktura projektu

- main.cpp - logika menu i główna pętla programu
- functions.h - deklaracje klas i szablonów
- functions.cpp - definicje metod
- makefile - reguły kompilacji

## Funkcje programu

- Wyświetlanie wszystkich zasobów i statystyk
- Przegląd zasobów posortowanych według kategorii
- Wypożyczanie i zwracanie pozycji z obslugą wyjatków
- Raport końcowy(lista wszystkich zasobów posortowana po cenie dobowej)

## Wymagania 

- System operacyjny : Windows, Linux lub macOS
- Kompilator C++ który obsłguje standard **C++17**

## Jak uruchomić program

### 1. Pobranie z GitHub

Otwórz terminal i wpisz :
```bash
git clone https://github.com/Wiktor284/cpp_2026.git
cd cpp_2026
```
### 2. Kompilacja
```bash
# Linux / macOS
make

# Windows (MinGW)
g++ -std=c++17 functions.cpp main.cpp -o biblioteka.exe
```
### 3. Uruchamianie
```bash
# Linux / macOS
./biblioteka

# Windows (MinGW)
./biblioteka.exe
```
### 4. Czyszczenie plików tymczasowych
```bash
# Linux / macOS
make clean

# Windows (MinGW)
# (CMD)
del *.o biblioteka.exe
# (PowerShell)
Remove-Item *.o, biblioteka.exe
