#include "hangman.h"
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <locale.h>
#include <iostream>
#include <limits>

// Инициализация глобального списка слов
std::vector<std::string> word_list = { "DOOR", "BROWSER", "VIRUS", "DRIVER", "LOADING", "INTERNET", "LOGIN" };

std::string get_word(const std::vector<std::string>& lst, int seed) {
    if (seed == 0) {
        srand(static_cast<unsigned int>(time(0))); // Инициализация генератора случайных чисел
    }
    else {
        srand(seed); // Установка зерна генератора для тестирования
    }
    int index = rand() % static_cast<int>(lst.size()); // Получение случайного индекса
    std::string word = lst[index]; // Выбор слова по индексу
    std::transform(word.begin(), word.end(), word.begin(), ::toupper); // Преобразование слова в верхний регистр
    return word; // Возврат выбранного слова
}

std::string display_hangman(int tries) {
    // Список возможных состояний виселицы
    std::vector<std::string> stages = {
        "   --------\n   |      |\n   |      O\n   |     \\|/\n   |      |\n   |     / \\\n   -\n",
        "   --------\n   |      |\n   |      O\n   |     \\|/\n   |      |\n   |     / \n   -\n",
        "   --------\n   |      |\n   |      O\n   |     \\|/\n   |      |\n   |      \n   -\n",
        "   --------\n   |      |\n   |      O\n   |     \\|\n   |      |\n   |      \n   -\n",
        "   --------\n   |      |\n   |      O\n   |      |\n   |      |\n   |      \n   -\n",
        "   --------\n   |      |\n   |      O\n   |      \n   |      \n   |      \n   -\n",
        "   --------\n   |      |\n   |      \n   |      \n   |      \n   |      \n   -\n"
    };
    if (tries < 0 || tries >= static_cast<int>(stages.size())) {
        return "Неверное количество попыток";
    }
    return stages[tries]; // Возврат текущего состояния виселицы
}

void play(const std::string& word) {
    std::string word_completion(word.length(), '_'); // Инициализация строки для отображения прогресса игрока
    bool guessed = false; // Флаг, указывающий, угадал ли игрок слово
    std::vector<char> guessed_letters; // Список уже названных букв
    std::vector<std::string> guessed_words; // Список уже названных слов
    int tries = 6; // Максимальное количество попыток
    int curr_tries = 0; // Текущее количество использованных попыток

    std::cout << "Давайте играть в угадайку слов!\n"; // Вывод приветственного сообщения
    std::cout << display_hangman(curr_tries) << "\n"; // Вывод начального состояния виселицы
    std::cout << word_completion << "\n"; // Вывод начального состояния слова

    // Цикл игры
    while (!guessed && curr_tries < tries) {
        std::string choose_state; // Переменная для выбора ввода буквы или слова
        while (true) {
            std::cout << "Для ввода буквы нажмите 1, для слова - 2: ";
            std::cin >> choose_state;
            if (choose_state == "1" || choose_state == "2") {
                break;
            }
            else {
                std::cout << "Неверный ввод!\n";
                // Очистка буфера ввода
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        // Если игрок выбрал ввод буквы
        if (choose_state == "1") {
            char letter; // Переменная для введенной буквы
            while (true) {
                std::cout << "Введите букву: ";
                std::cin >> letter;
                letter = static_cast<char>(toupper(letter)); // Преобразование буквы в верхний регистр
                if (isalpha(letter)) {
                    break;
                }
                else {
                    std::cout << "Неверный ввод!\n";
                }
            }

            // Если буква уже была названа
            if (std::find(guessed_letters.begin(), guessed_letters.end(), letter) != guessed_letters.end()) {
                std::cout << "Такая буква уже была названа.\n";
            }
            else {
                guessed_letters.push_back(letter); // Добавление буквы в список названных
                // Если буквы нет в слове
                if (word.find(letter) == std::string::npos) {
                    curr_tries++; // Увеличение количества использованных попыток
                }
                else {
                    // Замена соответствующих символов в строке прогресса
                    for (size_t i = 0; i < word.length(); ++i) {
                        if (letter == word[i]) {
                            word_completion[i] = letter;
                        }
                    }
                }
            }
        }

        // Если игрок выбрал ввод слова
        if (choose_state == "2") {
            std::string wd; // Переменная для введенного слова
            while (true) {
                std::cout << "Введите слово: ";
                std::cin >> wd;
                std::transform(wd.begin(), wd.end(), wd.begin(), ::toupper); // Преобразование слова в верхний регистр
                if (wd.length() == word.length()) {
                    break;
                }
                else {
                    std::cout << "Неверный ввод! Длина слова: " << word.length() << "\n";
                }
            }

            // Если слово уже было названо
            if (std::find(guessed_words.begin(), guessed_words.end(), wd) != guessed_words.end()) {
                std::cout << "Такое слово уже было названо.\n";
            }
            else {
                guessed_words.push_back(wd); // Добавление слова в список названных
                // Если слово угадано
                if (wd == word) {
                    guessed = true; // Установка флага угаданного слова
                    std::cout << "Поздравляем, вы угадали слово! Вы победили!\nЗагаданное слово: " << word << "\n";
                }
                else {
                    curr_tries++; // Увеличение количества использованных попыток
                }
            }
        }

        // Обновление состояния виселицы и прогресса слова, если слово не угадано
        if (!guessed) {
            std::cout << display_hangman(curr_tries) << "\n" << word_completion << "\nОсталось " << (tries - curr_tries) << " попыток\n";
        }

        // Если слово полностью угадано
        if (word_completion == word) {
            guessed = true; // Установка флага угаданного слова
            std::cout << "Поздравляем, вы угадали слово! Вы победили!\nЗагаданное слово: " << word << "\n";
        }
    }

    // Если слово не угадано
    if (!guessed) {
        std::cout << "Вы проиграли. Загаданное слово: " << word << "\n";
    }
}
