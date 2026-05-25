#!/usr/bin/env python3

import psycopg2
import numpy as np

DB_CONFIG = {
    "dbname": "imdb_test",
    "user": "postgres",
    "password": "a",
    "host": "localhost",
    "port": 5432,
}


def get_size_metrics(cursor):
    cursor.execute("SELECT pg_total_relation_size('actors')")
    table_size = cursor.fetchone()[0]
    cursor.execute("SELECT pg_current_wal_lsn() - '0/0'::pg_lsn")
    wal_lsn = int(cursor.fetchone()[0])
    return table_size, wal_lsn


def run_update_experiment(conn):
    with conn.cursor() as cursor:
        cursor.execute("""
            (SELECT id, pg_column_size(roles) FROM actors WHERE pg_column_size(roles) < 2000 LIMIT 1)
            UNION ALL
            (SELECT id, pg_column_size(roles) FROM actors WHERE pg_column_size(roles) > 10000 LIMIT 1)
        """)
        candidates = cursor.fetchall()

        results_table = []

        for row_id, json_len in candidates:
            size_before, wal_before = get_size_metrics(cursor)

            new_year = str(np.random.randint(1900, 2024))
            cursor.execute(
                """
                UPDATE actors 
                SET roles = jsonb_set(roles, '{roles,0,year}', %s) 
                WHERE id = %s
            """,
                (f'"{new_year}"', row_id),
            )
            conn.commit()

            size_after, wal_after = get_size_metrics(cursor)

            results_table.append(
                {
                    "длина jsonb": json_len,
                    "размер табл. до": size_before,
                    "размер табл. после": size_after,
                    "прирост табл.": size_after - size_before,
                    "WAL до": wal_before,
                    "WAL после": wal_after,
                    "прирост WAL": wal_after - wal_before,
                }
            )

        print(
            f"{'Длина':<10} | {'Табл До':<12} | {'Табл После':<12} | {'Прирост':<10} | {'WAL Прирост':<12}"
        )
        print("-" * 75)
        for r in results_table:
            print(
                f"{r['длина jsonb']:<10} | {r['размер табл. до']:<12} | {r['размер табл. после']:<12} | "
                f"{r['прирост табл.']:<10} | {r['прирост WAL']:<12}"
            )


def main():
    print("1. Подключение к БД...")

    try:
        with psycopg2.connect(**DB_CONFIG) as conn:
            conn.autocommit = False

            run_update_experiment(conn)

    except Exception as e:
        print(f"Ошибка: {e}")
        import traceback

        traceback.print_exc()


if __name__ == "__main__":
    main()
