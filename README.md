## Stepik Course "Многопоточное программирование на С/С++"
В данном репозитории представлены все "письменные" задания предложенные в соответствующем курсе.

## Stepik Task №1
Посмотрим, насколько хорошо вы умеете обращаться с контейнерами!?

Задача простая - найти производную многочлена. Многочлен может быть большой, но только с неотрицательными целыми степенями и целыми коэффициентами. Подумайте, каким образом его лучше представить в памяти? Вывод должен быть без пробелов и в порядке убывания степеней, хотя ввод этому правилу может не удовлетворять. Также на ввод может быть подан многочлен с неприведенными слагаемыми.

## Stepik Task №2
Тут задача очень простая. Вам нужно создать умный указатель на объект класса std::string. Он должен иметь защиту от разыменования NULL - при попытке это сделать он должен конструировать внутри себя пустую строку.

## Stepik Task №3
Давайте напишем простой аллокатор.

N.B. Для получения баллов дефрагментацию делать не требуется, но очень рекомендуется сделать ее для самотренировки. Для того, чтобы ее сделать, придется реализовывать умные указатели, инвариантные относительно позиции блока памяти!

N.B. ReAlloc должен сохранять данные в блоке. То есть это не просто пара Free + Alloc

## Stepik Task №4
Напишите эхо-сервер (IPv4, TCP). Это сервер, который возвращает то, что ему прислано. Вопросы (с кодом и без) можно задавать в комментариях.

Тестировать сервер можно с помощью утилит telnet и nc - выбирайте по вкусу.
(Написан также клиент эхо-сервера для тестирования)
(Сервер и клиент написаны под Windows. Далее задания связанные с сокетами будут написаны под Linux)

## Stepik Task №5
Примитивный чат на основе сервера(IPv4, TCP). Проверка событий осуществляется при помощи select(). Для тестирования можно использовать утилиты telnet и nc - выбирайте по вкусу.

## Stepik Task №6
Примитивный чат на основе сервера(IPv4, TCP). Проверка событий осуществляется при помощи poll(). Для тестирования можно использовать утилиты telnet и nc - выбирайте по вкусу.

## Stepik Task №7
Примитивный чат на основе сервера(IPv4, TCP). Проверка событий осуществляется при помощи EPoll. Для тестирования можно использовать утилиты telnet и nc - выбирайте по вкусу.
