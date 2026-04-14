CREATE EXTENSION IF NOT EXISTS plpython3u;

CREATE EXTENSION IF NOT EXISTS plpython3u;

CREATE OR REPLACE PROCEDURE generate_data()
AS $$
import random

word_list =[f"word_{i}" for i in range(1001)]

batch_size = 5000

# Сначала очистить таблицы
plpy.execute("TRUNCATE main_table CASCADE")
plpy.execute("TRUNCATE child_table")

# ===== Заполняем main_table (ID от 1 до 1,000,000) =====
plpy.notice("Starting main_table (1M rows)...")
for start in range(1, 1_000_001, batch_size):
    # Исправлен диапазон в range для корректной генерации всех ID
    end_val = min(start + batch_size, 1_000_001)
    values_str = ','.join([
        f"({i}, 'value_{i}', '2024-01-01')" 
        for i in range(start, end_val)
    ])
    plpy.execute(f"INSERT INTO main_table VALUES {values_str}")
    if start % 100000 == 1:
        plpy.notice(f"main_table progress: {start-1}/1,000,000")

# ===== Заполняем child_table, используя ТОЛЬКО существующие ID =====
plpy.notice("Starting child_table (10M rows)...")
for start in range(0, 10_000_000, batch_size):
    values = []
    end = min(start + batch_size, 10_000_000)
    for j in range(start, end):
        # Генерируем "человекочитаемую" фразу из 10 слов
        comment_text = ' '.join(random.choices(word_list, k=10)).replace("'", "''")
        # Гарантированно существующий ID (от 1 до 1,000,000)
        main_id = random.randint(1, 1_000_000)
        values.append(f"({j}, {main_id}, '{comment_text}', '2024-01-01')")
    
    plpy.execute(f"INSERT INTO child_table VALUES {','.join(values)}")
    if start % 500000 == 0:
        plpy.notice(f"child_table progress: {end}/10,000,000")

$$ LANGUAGE plpython3u;


-- CREATE OR REPLACE PROCEDURE generate_data()
-- AS $$
-- import random
-- import string
--
-- word_list = [''.join(random.choices(string.ascii_lowercase, k=random.randint(3,10))) 
--              for _ in range(1200)]
--
-- batch_size = 5000
--
-- # Сначала очистить таблицы
-- plpy.execute("TRUNCATE main_table CASCADE")
-- plpy.execute("TRUNCATE child_table")
--
-- # ===== Заполняем main_table (ID от 1 до 1,000,000) =====
-- plpy.notice("Starting main_table (1M rows)...")
-- for start in range(1, 1_000_001, batch_size):
--     values_str = ','.join([
--         f"({i}, 'value_{i}', '2024-01-01')" 
--         for i in range(start, min(start+batch_size, 1_000_001))
--     ])
--     plpy.execute(f"INSERT INTO main_table VALUES {values_str}")
--     plpy.notice(f"main_table: {min(start+batch_size-1, 1_000_000)}/1,000,000")
--
-- # ===== Заполняем child_table, используя ТОЛЬКО существующие ID =====
-- plpy.notice("Starting child_table (10M rows)...")
-- for start in range(0, 10_000_000, batch_size):
--     values = []
--     end = min(start+batch_size, 10_000_000)
--     for j in range(start, end):
--         comment_text = ' '.join(random.choices(word_list, k=10)).replace("'", "''")
--         # Гарантированно существующий ID (от 1 до 1,000,000)
--         main_id = random.randint(1, 1_000_000)
--         values.append(f"({j}, {main_id}, '{comment_text}', '2024-01-01')")
--
--     plpy.execute(f"INSERT INTO child_table VALUES {','.join(values)}")
--     plpy.notice(f"child_table: {end}/10,000,000")
--
-- $$ LANGUAGE plpython3u;
