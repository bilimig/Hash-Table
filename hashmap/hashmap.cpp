#include <iostream>
#include <string>
#include <vector>

const int SIZE = 10;

struct KeyValuePair {
    std::string key;
    std::string value;
};

struct lista {
    std::string key;
    std::string value;
    lista* next;
};


class HashTable {
public:
    HashTable() : size_(SIZE), elementy(size_, NULL) {}


    std::string get(std::string const& key) {
        auto current = elementy[hashing(key)];
        while (current) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;

        }
        return "Brak Dopasowania";
    }

    void set(std::string const& key, std::string const& value) {
        std::cout << "set ( " << key << ", " << value << "): hash = " << hashing(key) << "\n";
        auto h = hashing(key);
        auto current = elementy[h];

        while (current) {
            if (current->key == key) {
                current->value = value; // Nadpisanie wartości dla istniejącego klucza
                return;
            }
            current = current->next;
        }

        elementy[h] = new lista{ key, value, elementy[h] };
    }

    bool del(std::string const& key) {
        lista** current = &elementy[hashing(key)];
        while (*current) {
            if ((*current)->key == key) {
                auto to_delete = *current;
                *current = (*current)->next;
                delete to_delete;
                return true;
            }
            current = &((*current)->next);
        }
        return false;
    }


private:

    size_t hashing(std::string const& key) {
        size_t sum = 0;
        for (char c : key) {
            sum += c;
        }
        return sum % size_;
    }

    size_t size_;
    std::vector<lista*> elementy;

};

class LinearProbingHashTable {
private:
    KeyValuePair table[SIZE];

public:
    LinearProbingHashTable() {
        clear();
    }

    void clear() {
        for (int i = 0; i < SIZE; i++) {
            table[i] = { "", "" }; // Puste wartości oznaczają wolne sloty
        }
    }

    int hashFunction(const std::string& key) {
        int hash = 0;
        for (char c : key) {
            hash += c;
        }
        return hash % SIZE;
    }

    int linearProbe(int index, int attempt) {
        return (index + attempt) % SIZE; // Rozwiązanie kolizji liniowej
    }

    void insert(const std::string& key, const std::string& value) {
        int index = hashFunction(key);
        int attempt = 0;

        while (!table[index].key.empty()) {
            index = linearProbe(index, attempt);
            attempt++;
        }

        table[index] = { key, value };
    }

    int search(const std::string& key) {
        int index = hashFunction(key);
        int attempt = 0;

        while (table[index].key != key && !table[index].key.empty()) {
            index = linearProbe(index, attempt);
            attempt++;

            if (attempt == SIZE)
                return -1; // Przeszukano całą tablicę, element nie znaleziony
        }

        return index;
    }

    std::string getValue(int index) {
        return table[index].value;
    }

    void printValueForKey(const std::string& key) {
        int index = search(key);
        if (index != -1) {
            std::cout << "Klucz: " << key << ", Wartość: " << getValue(index) << "Hash: " << index << std::endl;
        }
        else {
            std::cout << "Klucz '" << key << "' nie został znaleziony." << std::endl;
        }
    }
};

class QuadraticProbingHashTable {
private:
    KeyValuePair table[SIZE];

public:
    QuadraticProbingHashTable() {
        clear();
    }

    void clear() {
        for (int i = 0; i < SIZE; i++) {
            table[i] = { "", "" }; // Puste wartości oznaczają wolne sloty
        }
    }

    int hashFunction(const std::string& key) {
        int hash = 0;
        for (char c : key) {
            hash += c;
        }
        return hash % SIZE;
    }

    int quadraticProbe(int index, int attempt) {
        return (index + attempt * attempt) % SIZE; // Rozwiązanie kolizji kwadratowej
    }

    void insert(const std::string& key, const std::string& value) {
        int index = hashFunction(key);
        int attempt = 0;

        while (!table[index].key.empty()) {
            index = quadraticProbe(index, attempt);
            attempt++;
        }

        table[index] = { key, value };
    }

    int search(const std::string& key) {
        int index = hashFunction(key);
        int attempt = 0;

        while (table[index].key != key && !table[index].key.empty()) {
            index = quadraticProbe(index, attempt);
            attempt++;

            if (attempt == SIZE)
                return -1; // Przeszukano całą tablicę, element nie znaleziony
        }

        return index;
    }

    std::string getValue(int index) {
        return table[index].value;
    }

    void printValueForKey(const std::string& key) {
        int index = search(key);
        if (index != -1) {
            std::cout << "Klucz: " << key << ", Wartość: " << getValue(index) << "Hash: " << index << std::endl;
        }
        else {
            std::cout << "Klucz '" << key << "' nie został znaleziony." << std::endl;
        }
    }
};

class DoubleHashingHashTable {
private:
    KeyValuePair table[SIZE];

public:
    DoubleHashingHashTable() {
        clear();
    }

    void clear() {
        for (int i = 0; i < SIZE; i++) {
            table[i] = { "", "" }; // Puste wartości oznaczają wolne sloty
        }
    }

    int hashFunction(const std::string& key) {
        int hash = 0;
        for (char c : key) {
            hash += c;
        }
        return hash % SIZE;
    }

    int doubleHash(int index, int attempt, const std::string& key) {
        int hash1 = hashFunction(key);
        int hash2 = 7 - (std::stoi(key) % 7); // Wybór drugiej funkcji haszującej

        return (index + attempt * hash2) % SIZE; // Rozwiązanie kolizji podwójnym hashowaniem
    }

    void insert(const std::string& key, const std::string& value) {
        int index = hashFunction(key);
        int attempt = 0;

        while (!table[index].key.empty() && table[index].key != key) {
            index = doubleHash(index, attempt, key);
            attempt++;

            if (attempt == SIZE) {
                std::cout << "Przekroczono limit prób. Nie można dodać elementu." << std::endl;
                return;
            }
        }

        table[index] = { key, value };
    }

    int search(const std::string& key) {
        int index = hashFunction(key);
        int attempt = 0;

        while (table[index].key != key && !table[index].key.empty()) {
            index = doubleHash(index, attempt, key);
            attempt++;

            if (attempt == SIZE)
                return -1; // Przeszukano całą tablicę, element nie znaleziony
        }

        return index;
    }

    std::string getValue(int index) {
        return table[index].value;
    }

    void printValueForKey(const std::string& key) {
        int index = search(key);

        if (index != -1) {
            std::cout << "Klucz: " << key << ", Wartość: " << getValue(index) << "Hash: " << index<< std::endl;
        }
        else {
            std::cout << "Klucz '" << key << "' nie został znaleziony." << std::endl;
        }
    }

};
int main() {
    LinearProbingHashTable linearHashTable;
    linearHashTable.insert("key1", "value1");
    linearHashTable.insert("key2", "value2");
    linearHashTable.printValueForKey("key2");
    linearHashTable.printValueForKey("key1");
    linearHashTable.insert("key2", "value3");
    linearHashTable.printValueForKey("key2");

    QuadraticProbingHashTable quadraticHashTable;
    quadraticHashTable.insert("key1", "value1");
    quadraticHashTable.insert("key2", "value2");
    quadraticHashTable.printValueForKey("key2");
    quadraticHashTable.printValueForKey("key1");
    quadraticHashTable.insert("key2", "value3");
    quadraticHashTable.printValueForKey("key2");

    DoubleHashingHashTable doubleHashingHashTable;
    doubleHashingHashTable.insert("key1", "value1");
    doubleHashingHashTable.insert("key2", "value2");
    doubleHashingHashTable.printValueForKey("key2");
    doubleHashingHashTable.printValueForKey("key1");
    doubleHashingHashTable.insert("key2", "value3");
    doubleHashingHashTable.printValueForKey("key2");
    HashTable Tablica_hash;

    Tablica_hash.set("Kamil", "Owczarek");
    Tablica_hash.set("Maciek", "Krawczyk");
    Tablica_hash.set("Kuba", "Frydrych");
    Tablica_hash.set("Michal", "Rogacki");
    Tablica_hash.set("Damian", "Jakubowski");

    std::cout << Tablica_hash.get("Maciek") << "\n";
    std::cout << Tablica_hash.get("Damian") << "\n";

    Tablica_hash.set("Maciek", "Janiak");  

    std::cout << Tablica_hash.get("Maciek") << "\n";
    std::cout << Tablica_hash.get("Damian") << "\n";

    return 0;

    return 0;
}
