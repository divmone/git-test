#include <iostream>
#include <math.h>

using namespace std;

int main()
{
    double circleRadius, sideSquare;
    bool isCorrect;

    setlocale(LC_ALL, "Russian");

    cout << "\tДанная программа проверяет, входит ли квадрат в круг\n"
         << "Введите радиус круга (натуральное число).\n"
         << "Диапазон значений для ввода значений радиуса окружности 1...100 \n";
   
    do {

        isCorrect = false;
        
        cin >> circleRadius;
        if (circleRadius < 1 ) {
            isCorrect = true; 
        }

        if (circleRadius > 100 ) {
            isCorrect = true;
        }

        if (cin.get() != '\n' ) {
            isCorrect = true;
            cin.clear();
            while (cin.get() != '\n') {}
        }
        
        if (isCorrect) {
            cout << "Повторите ввод\n"
                 << "Диапазон значений для ввода значений радиуса окружности 1...100 \n";
        }
    } while(isCorrect);
    
    cout << "Введите длину стороны квадрата(натуральное число)\n"
         << "Диапазон значений для ввода значений длины квадрата 1...100 \n";
    
    do {

        isCorrect = false;

        cin  >> sideSquare;
        if ( sideSquare < 1) {
            isCorrect = true;
        }

        if (
            sideSquare > 100) {
            isCorrect = true;
        }

        if (cin.get() != '\n') {
            isCorrect = true;
            cin.clear();
            while (cin.get() != '\n') {}
        }

        if (isCorrect) {
            cout << "Повторите ввод\n"
                 << "Диапазон значений для ввода значений радиуса окружности 1...100 \n";
        }
    } while(isCorrect);
        
    if (circleRadius > (sideSquare * sqrt(2)) / 2) {
        cout << "Квадрат проходит в круг";
    }
    else {
        cout << "Квадрат не проходит в круг";
    } 

    return 0;

}


    





