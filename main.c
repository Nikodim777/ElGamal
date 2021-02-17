#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

int Pow(int num, int e, int n) //num - код символа,e - ключ,n - 2 открытый ключ
{
    int b = 1;
    while ( e > 1 ) { //Последовательное вычисление (num^e)%n
        if ( e % 2 ) {
            b *= num;
            b %= n;
            e--;
        } else {
            e /= 2;
            num *= num;
            num %= n;
        }
    }
    return ( num * b ) % n;
}

int getRoot(int p) //Получение первообразного корня
{
    int i, j = 0, k = 0, n = p - 1,
           fl, //Флаг нахождения корня
           emas[100]; //Массив делителей
    for ( i = 2; i * i <= n; i++ ) //Заполнение массива
        if ( n % i == 0 ) {
            emas[k++] = i;
            while ( n % i == 0 ) {
                n /= i;
            }
        }

    if( n > 1 ) {
        emas[k++] = n;
    }
    for ( i = 2; i <= p; i++ ) { //Нахождение корня
        fl = 1;
        for ( j = 0; j < k && fl; j++ ) {
            fl &= Pow ( i, ( p - 1 ) / emas[j], p ) != 1;
        }
        if ( fl ) {
            return i;
        }
    }
    return -1;
}

int main()
{
    SetConsoleOutputCP ( 1251 );
    srand ( time ( NULL ) );
    const int smas[10] = {919, 929, 937, 941, 947, 953, 967, 971, 977, 973}; //Массив простых чисел
    int i,
        p, //Простое число
        g, //Его первообразный корень
        e, //Открытый ключ
        d, //Закрытый ключ
        k, //Временный ключ
        num, //Число считанных элементов
        code[1024]; //Массив для хранения шифротекста
    char ch, filename[255], text[1024];
    FILE *fd;
    while ( 1 ) {
        printf ( "Выберите дейстивие: 0 - сгенерировать ключи, 1 - Выйти\n" );
        if ( getch() == '1' ) {
            break;
        }
        p = smas[rand() % 10];
        g = getRoot ( p );
        d = 1 + rand() % ( p - 2 );
        e = Pow ( g, d, p );
        printf ( "Простое число - %d\nПервообразный корень - %d\nОткрытый ключ - %d\nЗакрытый ключ - %d\n", p, g, e, d );
        while ( 1 ) {
            printf ( "Выберите дейстивие: 0 - зашифровать, 1 - расшифровать, 2 - Выйти\n" );
            ch = getch();
            if ( ch == '2' ) {
                break;
            }
            printf ( "Введите имя файла - " );
            scanf ( "%s", filename );
            if ( ch == '0' ) {
                fd = fopen ( filename, "r" );
                num = fread ( text, sizeof ( char ), 512, fd );
                fclose ( fd );
                for ( i = 0; i < num; i++ ) { //Преобразования символа в пару шифротекста
                    k = 1 + rand() % ( p - 2 );
                    code[2 * i] = Pow ( g, k, p );
                    code[2 * i + 1] = ( Pow ( e, k, p ) * ( text[i] + 128 ) ) % p; //+ 128 чтобы не было отрицательным значение
                }
                fd = fopen ( filename, "w" );
                fwrite ( code, sizeof ( int ), 2 * num, fd );
                fclose ( fd );
            } else {
                fd = fopen ( filename, "r" );
                num = fread ( code, sizeof ( int ), 1024, fd );
                fclose ( fd );
                for ( i = 0; i < num / 2; i++ ) { //Преобразование пары чисел в символ
                    text[i] = ( ( Pow ( code[2 * i], p - 1 - d, p ) ) * ( code[2 * i + 1] ) ) % p - 128;    //Отнимаем прибавленные ранее 128
                }
                text[num / 2] = '\0';
                fd = fopen ( filename, "w" );
                fwrite ( text, sizeof ( char ), num / 2, fd );
                fclose ( fd );
            }
        }
    }
    return 0;
}
