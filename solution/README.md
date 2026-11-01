# Решение

В этой папке лежит зашифрованное решение.
Получите от преподавателя пароль[^1], расшифруйте решение и примените[^2]:

```shell
openssl enc -d -aes-256-cbc -pbkdf2 -in solution/solution.diff.enc -out solution/solution.diff
git applu solution/solution.diff
```

[^1]: Подсказка для преподавателя: `gopass show cppdevcourse/homeworks/multicast`
[^2]: Команды нужно выполнять из корня проекта без изменений в рабочей директории
