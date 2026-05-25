#!/usr/bin/env python3

import psycopg2
import matplotlib.pyplot as plt
import numpy as np

DB_CONFIG = {
    "dbname": "imdb_test",
    "user": "postgres",
    "password": "a",
    "host": "localhost",
    "port": 5432,
}

PATTERNS = {
    "year": r"\((\d{4}|\?{4})(?:/[^)]*)?\)",
    "type": r"\((?:VI|TV|VG|AF|UC|VO)\)",
    "series": r"\{(.*?)\}",
    "as_character": r"\(as character\)",
    "character_name": r"\[(.*?)\]",
    "credit": r"<(.*?)>",
}

def measure_access_time(conn, num_measurements=30):
    with conn.cursor() as cursor:
        print("Формирование представительной выборки (400 точек)...")

        query_sample = """
        (
            SELECT DISTINCT ON (pg_column_size(roles) / 20)
                id, pg_column_size(roles) as size
            FROM actors
            WHERE pg_column_size(roles) < 2048 
              AND roles IS NOT NULL AND jsonb_array_length(roles->'roles') > 0
            ORDER BY pg_column_size(roles) / 20, id
            LIMIT 100
        )
        UNION ALL
        (
            SELECT DISTINCT ON (pg_column_size(roles) / 100)
                id, pg_column_size(roles) as size
            FROM actors
            WHERE pg_column_size(roles) >= 2048 
              AND roles IS NOT NULL AND jsonb_array_length(roles->'roles') > 0
            ORDER BY pg_column_size(roles) / 100, id
            LIMIT 300
        )
        ORDER BY size;
        """

        cursor.execute(query_sample)
        samples = cursor.fetchall()
        print(f"Выборка готова. Всего точек: {len(samples)}")

        results = []
        for row_id, size in samples:
            times = []

            explain_query = f"""
                EXPLAIN (ANALYZE TRUE, TIMING FALSE, FORMAT JSON)
                SELECT roles->'roles'->0->>'year' FROM actors WHERE id = {row_id}
            """

            for _ in range(num_measurements):
                cursor.execute(explain_query)
                plan = cursor.fetchone()[0]
                # 'Actual Total Time' ms
                exec_time = plan[0]["Execution Time"]
                times.append(exec_time)

            mean_t = np.mean(times)
            std_t = np.std(times)
            filtered = [t for t in times if abs(t - mean_t) <= 2 * std_t]

            if filtered:
                avg_time = np.mean(filtered)
                results.append({"size": size, "time": avg_time})
                print(f"Size: {size:>6} bytes | Time: {avg_time:.4f} ms")

    return results


def plot_results(results):
    if not results:
        print("Нет данных для построения графика")
        return

    sizes = [r["size"] for r in results]
    times = [r["time"] for r in results]

    plt.figure(figsize=(12, 6))

    plt.plot(sizes, times, "o-", markersize=3, alpha=0.6, label="Время доступа")

    plt.axvline(x=2048, color="red", linestyle="--", label="Порог TOAST (2048 байт)")

    plt.title("Зависимость времени доступа к JSONB от размера объекта", fontsize=14)
    plt.xlabel("Длина jsonb в байтах (ось X)", fontsize=12)
    plt.ylabel("Время доступа (year) в мс (ось Y)", fontsize=12)

    plt.grid(True, which="both", ls="-", alpha=0.3)

    plt.legend()
    plt.show()


def main():
    print("1. Подключение к БД...")

    try:
        with psycopg2.connect(**DB_CONFIG) as conn:
            conn.autocommit = False
            print("Подключено успешно")

            print("\n2. Измерение времени доступа...")
            results = measure_access_time(conn, num_measurements=30)

            print("\n3. Построение графика...")
            plot_results(results)

    except Exception as e:
        print(f"Ошибка: {e}")
        import traceback

        traceback.print_exc()


if __name__ == "__main__":
    main()
