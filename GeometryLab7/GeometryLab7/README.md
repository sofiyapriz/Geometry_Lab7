# Лабораторна робота №7
## Обробка виключень. Тестування програми

**Варіант 16 — GeometryTask**

### 1. Перевірка RAII та Stack Unwinding

У програмі є клас `ScopeLogger`, який виводить повідомлення про створення та знищення об'єктів. Для перевірки було створено кілька вкладених функцій, у яких виникає виняток.

Очікуваний результат:

```text
Created: main
Created: level1
Created: level2
Created: level3
Destroyed: level3
[level2] diagnostics: angleBetween: zero vector
Destroyed: level2
Destroyed: level1
[main] degenerate: angleBetween: zero vector
Program continues
Destroyed: main
```

Фактичний результат відповідає очікуваному. Після виникнення винятку об'єкти знищуються у зворотному порядку створення. Функція `level2` виводить повідомлення про помилку та передає виняток далі за допомогою `throw;`. Після його обробки в `main` програма продовжує виконання.

### 2. Перевірка нестандартних винятків

Окремо перевірено, як програма обробляє винятки типів `int` та `std::string`.

З'ясовано, що `catch(const std::exception&)` не перехоплює їх, оскільки ці типи не є нащадками `std::exception`. Натомість `catch(...)` дозволяє перехопити виняток незалежно від його типу.

### 3. Результати тестування

Для перевірки роботи програми використано Google Test. Загалом створено 22 тести, які охоплюють геометричні обчислення, помилкові ситуації, граничні значення та збереження стану об'єкта після невдалих операцій.

**Результат тестування: усі 22 тести пройшли успішно.**

### 4. Збирання та запуск програми

Для збирання проєкту використовуються такі команди:

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

Запуск тестів:

```powershell
.\build\tests\Debug\geometry_tests.exe
```

Запуск основної програми:

```powershell
.\build\demo\Debug\demo.exe
```

Запуск прикладу з нестандартними винятками:

```powershell
.\build\bad_exceptions\Debug\bad_exceptions.exe
```

Перевірка тестів через CTest:

```powershell
ctest --test-dir build -C Debug --output-on-failure
```
``