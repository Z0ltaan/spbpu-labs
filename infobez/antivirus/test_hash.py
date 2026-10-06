import hashlib

# 1. Вычисляем хеш строки, которую записывает FC
text_to_write = 'tfrdsavuxkilqgalcyvsmhhbfkvzsurczvqenezvnhfqckgrjn'
string_hash = hashlib.sha256(text_to_write.encode('utf-8')).hexdigest()
print("Хеш строки из FC:  ", string_hash)

# 2. Выводим первоначальный хеш 6.txt из вашего report.txt для сравнения
report_hash = "3a83d6657f2d53fc11156870d8676c4cdf12ad41b0f9a3d77ff3524348a90d44"
print("Хеш из report.txt:", report_hash)

# 3. Автоматическое сравнение
print("Они совпадают?", string_hash == report_hash)

