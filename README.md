
# Отчёт по ИДЗ 2

## Об отчёте

**Выполнил:** Каверин Максим Вячеславович

**Группа:** БПИ217

**Вариант:** 32

**Задание:** Шайка пиратов под предводительством Джона Сильвера высадилась на берег Острова Сокровищ. Несмотря на добытую карту старого Флинта, местоположение сокровищ по-прежнему остается загадкой, поэтому искать клад приходится практически на ощупь. Так как Сильвер ходит на деревянной ноге, то самому бродить по джунглям ему не с руки. Джон Сильвер поделил остров на участки, а пиратов на небольшие группы. Каждой группе поручается искать клад на нескольких участках, а сам Сильвер ждет на берегу. Группа пиратов, обшарив один участок, переходит на следующий, еще необследованный. Закончив поиски, пираты возвращаются к Сильверу и докладывают о результатах. Если какая–то из групп находит клад на одном из своих участков, она возвращается к Сильверу, который шлет попугая, инициализирующего прекращение (прервыание) всех работ. Требуется создать приложение, моделирующее действия Сильвера и пиратов.

Данный отчёт разбит на блоки по оценкам для удобства проверки. Работа была выполнена с учетом требований до оценки 8 включительно.

## Схема работы

```mermaid
graph LR;
    A[Джон Сильвер] -->|Участки для поиска| B[Группы пиратов];
    B -->|Результаты поисков| A;
    A -->|Прерывание работ| B;
```

Джон Сильвер назначет участки для поиска группам пиратам. Когда группа пиратов находит клад, она оповщает Джона Сильвера об этом, а тот, в свою очередь, отправляет всем группам сигнал завершения работы.

## На оценку 4

> Все относящиеся к данному этапу файлы находятся в папке **4**.

Согласно требованиям была разработана программа, в котрой единый родительский процесс запускает требуемое число дочерних процессов. Процессы взаимодействуют с использованием именованных POSIX семафоров. Обмен данными ведется через разделяемую память в стандарте POSIX.

Пример результата работы программы:
```console
John Silver assigns section 0 to search
John Silver assigns section 1 to search
John Silver assigns section 2 to search
Pirate group 0 is searching section 0
Pirate group 2 is searching section 1
John Silver assigns section 3 to search
Pirate group 3 is searching section 2
John Silver assigns section 4 to search
Pirate group 1 is searching section 3
Pirate group 4 is searching section 4
John Silver assigns section 5 to search
Pirate group 2 is searching section 5
John Silver assigns section 6 to search
Pirate group 0 is searching section 6
John Silver assigns section 7 to search
Pirate group 3 is searching section 7
John Silver assigns section 8 to search
Pirate group 4 is searching section 8
John Silver assigns section 9 to search
Pirate group 1 is searching section 9
John Silver assigns section 10 to search
Pirate group 2 is searching section 10
John Silver assigns section 11 to search
Pirate group 0 is searching section 11
John Silver assigns section 12 to search
Pirate group 1 is searching section 12
John Silver assigns section 13 to search
Pirate group 4 found treasure in section 8!
John Silver was notified that the treasure has been found in section 8
Pirate group 1 is stopping it's work
Pirate group 0 is stopping it's work
Pirate group 2 is stopping it's work
Pirate group 4 is stopping it's work
Pirate group 3 is stopping it's work
```

## На оценку 5

> Все относящиеся к данному этапу файлы находятся в папке **5**.

Согласно требованиям была разработана программа, в котрой единый родительский процесс запускает требуемое число дочерних процессов. Процессы взаимодействуют с использованием неименованных POSIX семафоров расположенных в разделяемой памяти. Обмен данными ведется через разделяемую память в стандарте POSIX.

Пример результата работы программы:
```console
John Silver assigns section 0 to search
John Silver assigns section 1 to search
Pirate group 0 is searching section 0
John Silver assigns section 2 to search
Pirate group 1 is searching section 1
John Silver assigns section 3 to search
Pirate group 2 is searching section 2
John Silver assigns section 4 to search
John Silver assigns section 5 to search
Pirate group 3 is searching section 3
Pirate group 4 is searching section 4
Pirate group 4 is searching section 5
John Silver assigns section 6 to search
Pirate group 1 is searching section 6
John Silver assigns section 7 to search
Pirate group 2 is searching section 7
John Silver assigns section 8 to search
Pirate group 0 is searching section 8
John Silver assigns section 9 to search
Pirate group 3 is searching section 9
John Silver assigns section 10 to search
Pirate group 4 is searching section 10
John Silver assigns  section 11 to search
Pirate group 3 is searching section 11
John Silver assigns section 12 to search
Pirate group 0 is searching section 12
John Silver assigns section 13 to search
Pirate group 1 is searching section 13
John Silver assigns section 14 to search
Pirate group 2 is searching section 14
John Silver assigns section 15 to search
Pirate group 4 found treasure in section 10!
Pirate group 3 is searching section 15
John Silver assigns section 16 to search
John Silver was notified that the treasure has been found in section 10
Pirate group 0 is stopping it's work
Pirate group 1 is stopping it's work
Pirate group 4 is stopping it's work
Pirate group 2 is stopping it's work
Pirate group 3 is stopping it's work
```

## На оценку 6

> Все относящиеся к данному этапу файлы находятся в папке **6**.

Согласно требованиям была разработана программа, в котрой единый родительский процесс запускает требуемое число дочерних процессов. Процессы взаимодействуют с использованием семафоров в стандарте UNIX SYSTEM V. Обмен данными ведется через разделяемую память в стандарте UNIX SYSTEM V.

Пример результата работы программы:
```console
John Silver assigns section 0 to search
John Silver assigns section 1 to search
Pirate group 0 is searching section 0
John Silver assigns section 2 to search
Pirate group 1 is searching section 1
John Silver assigns section 3 to search
John Silver assigns section 4 to search
Pirate group 3 is searching section 3
Pirate group 2 is searching section 2
John Silver assigns section 5 to search
Pirate group 4 is searching section 4
Pirate group 0 is searching section 5
John Silver assigns section 6 to search
Pirate group 2 found treasure in section 2!
Pirate group 1 is searching section 6
John Silver assigns section 7 to search
John Silver was notified that the treasure has been found in section 2
Pirate group 3 is searching section 7
Pirate group 3 is stopping it's work
Pirate group 4 is stopping it's work
Pirate group 2 is stopping it's work
Pirate group 1 is stopping it's work
Pirate group 0 is stopping it's work
```

## На оценку 7

> Все относящиеся к данному этапу файлы находятся в папке **7**.

Согласно требованиям была разработана программа, состоящая из отдельных процессов, запускаемых независимо друг от друга. Процессы взаимодействуют с использованием именованных POSIX семафоров. Обмен данными ведется через разделяемую память в стандарте POSIX.

Пример вывода `main1.c`:

```console
John Silver assigns section 0 to search
John Silver assigns section 1 to search
John Silver assigns section 2 to search
John Silver assigns section 3 to search
John Silver assigns section 4 to search
John Silver assigns section 5 to search
John Silver assigns section 6 to search
John Silver assigns section 7 to search
John Silver assigns section 8 to search
John Silver assigns section 9 to search
John Silver was notified that the treasure has been found in section 7
```

Пример вывода нескольких запущенных `main2.c`:

```console
Pirate group 0 is searching section 0
Pirate group 0 is searching section 2
Pirate group 0 is searching section 6
Pirate group 0 is searching section 8
Pirate group 0 is stopping it's work
```

```console
Pirate group 1 is searching section 1
Pirate group 1 is searching section 3
Pirate group 1 is searching section 4
Pirate group 1 is searching section 5
Pirate group 1 is searching section 7
Pirate group 1 found treasure in section 7!
Pirate group 1 is stopping it's work
```

## На оценку 8

> Все относящиеся к данному этапу файлы находятся в папке **8**.

Согласно требованиям была разработана программа, состоящая из отдельных процессов, запускаемых независимо друг от друга. Процессы взаимодействуют с использованием семафоров в стандарте UNIX SYSTEM V. Обмен данными ведется через разделяемую память в стандарте UNIX SYSTEM V.

Пример вывода `main1.c`:

```console
John Silver assigns section 0 to search
John Silver assigns section 1 to search
John Silver assigns section 2 to search
John Silver assigns section 3 to search
John Silver assigns section 4 to search
John Silver assigns section 5 to search
John Silver assigns section 6 to search
John Silver assigns section 7 to search
John Silver assigns section 8 to search
John Silver assigns section 9 to search
John Silver assigns section 10 to search
John Silver assigns section 11 to search
John Silver assigns section 12 to search
John Silver assigns section 13 to search
John Silver assigns section 14 to search
John Silver assigns section 15 to search
John Silver assigns section 16 to search
John Silver assigns section 17 to search
John Silver assigns section 18 to search
John Silver assigns section 19 to search
John Silver was notified that the treasure has been found in section 17
```

Пример вывода нескольких запущенных `main2.c`:

```console
Pirate group 0 is searching section 0
Pirate group 0 is searching section 2
Pirate group 0 is searching section 3
Pirate group 0 is searching section 4
Pirate group 0 is searching section 6
Pirate group 0 is searching section 8
Pirate group 0 is searching section 10
Pirate group 0 is searching section 11
Pirate group 0 is searching section 12
Pirate group 0 is searching section 15
Pirate group 0 is searching section 17
Pirate group 0 found treasure in section 17!
Pirate group 0 is searching section 19
Pirate group 0 is stopping it's work
```

```console
Pirate group 1 is searching section 1
Pirate group 1 is searching section 5
Pirate group 1 is searching section 7
Pirate group 1 is searching section 9
Pirate group 1 is searching section 13
Pirate group 1 is searching section 14
Pirate group 1 is searching section 16
Pirate group 1 is searching section 18
Pirate group 1 is stopping it's work
```
