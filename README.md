# STC: CLI-app
## Тестовое задание для Syntacore на позицию Tools Intern Developer
## Установка
### Linux
Ввести в терминал, в директории с исходниками
    
    bash install.sh 

Необходимые инструменты: CMake
Данный скрипт запускает сборщик приложения. Результат сборки - stc и stc_test

stc - непосредственно приложение. Для запуска ввести в терминал:

    ./stc < *data_file_name* 

где data_file_name - файл с командами для выполнения

stc_test - unit-тесты

Сборка проверена на Ubuntu 22.04, g++ 11.3.0, cmake 3.22.1

### Windows
Ввести в bash-терминал, в директории с исходниками:

    bash install.sh

Необходимые инструменты: CMake, bash-оболочка для Windows.
Данный скрипт запускает сборщик приложения. Результат сборки - stc.exe и stc_test.exe

stc.exe - непосредственно приложение. Для запуска ввести в терминал:

    stc.exe < *data_file_name* 

где data_file_name - файл с командами для выполнения

stc_test - unit-тесты

Сборка проверена на Windows 10, MSVC от Visual Studio 2022, CMake 3.22.1