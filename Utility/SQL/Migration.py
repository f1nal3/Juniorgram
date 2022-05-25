#!/usr/bin/python
# coding: utf-8

import argparse
from subprocess import Popen, PIPE
import psycopg2
import os.path

PRODUCT_DATABASE_NAME = os.getenv('DATABASE_NAME')

kill_db_processes_sql = """SELECT    pg_terminate_backend (pid)
                        FROM    pg_stat_activity
                        WHERE   datname = '{}';
                    """.format(PRODUCT_DATABASE_NAME)

new_database_name_sql = """ALTER DATABASE {0} RENAME TO _{0};
                """.format(PRODUCT_DATABASE_NAME)


def connection_parameters(db_name_env='POSTGRES_DB') -> str:
    _args = "password=%s " % os.getenv('POSTGRES_PASSWORD') + \
           "host=%s " % os.getenv('POSTGRES_HOST') + \
           "port=%s " % os.getenv('POSTGRES_PORT') + \
           "dbname=%s " % os.getenv(db_name_env) + \
           "user=%s " % os.getenv('POSTGRES_USER')
    return _args


def grab_sql_request() -> str:
    path = "./"
    if os.path.isfile('Database_Scheme.sql'):
        path += 'Database_Scheme.sql'
    else:
        raise RuntimeError('Failed to open Database_Scheme.sql')
    request = None
    with open(path, "r") as source:
        request = source.read()
    return request


def make_table_copy() -> None:
    conn = None
    try:
        conn = psycopg2.connect(connection_parameters())
        cur = conn.cursor()
        cur.execute(kill_db_processes_sql)
        cur.execute(new_database_name_sql)
        conn.commit()
        cur.close()
    except (Exception, psycopg2.DatabaseError) as error:
        print(error)
    finally:
        if conn is not None:
            conn.close()


def delete_and_create() -> None:
    conn = None
    try:
        conn = psycopg2.connect(connection_parameters())
        conn.autocommit = True
        cur = conn.cursor()
        cur.execute('DROP DATABASE IF EXISTS {};'.format(PRODUCT_DATABASE_NAME))
        conn.commit()
        cur.execute('CREATE DATABASE {};'.format(PRODUCT_DATABASE_NAME))
        conn.commit()
        cur.close()
    except (Exception, psycopg2.DatabaseError) as error:
        print(error)
    finally:
        if conn is not None:
            conn.close()


def create_tables() -> None:
    conn = None
    try:
        conn = psycopg2.connect(connection_parameters('DATABASE_NAME'))
        conn.autocommit = True
        cur = conn.cursor()
        cur.execute(grab_sql_request())
        conn.commit()
        cur.close()
    except (Exception, psycopg2.DatabaseError) as error:
        print(error)
    finally:
        if conn is not None:
            conn.close()


def migrate_data() -> None:
    extract_cmd = ["pg_dump.exe", "-a", "_{}".format(PRODUCT_DATABASE_NAME)]
    inject_cmd = ["psql.exe", "-d", PRODUCT_DATABASE_NAME]
    my_env = os.environ.copy()
    my_env['PGPASSWORD'] = os.getenv('POSTGRES_PASSWORD')
    p1 = Popen(extract_cmd, stdout=PIPE, stdin=PIPE, text=True, shell=True, env=my_env)
    Popen(inject_cmd, stdin=p1.stdout, text=True, shell=True, env=my_env)


def drop_old_db() -> None:
    conn = None
    try:
        conn = psycopg2.connect(connection_parameters())
        conn.autocommit = True
        cur = conn.cursor()
        cur.execute(kill_db_processes_sql)
        cur.execute('DROP DATABASE IF EXISTS _{};'.format(PRODUCT_DATABASE_NAME))
        conn.commit()
        cur.close()
    except (Exception, psycopg2.DatabaseError) as error:
        print(error)
    finally:
        if conn is not None:
            conn.close()


def parse_arguments():
    # Create argument parser
    parser = argparse.ArgumentParser()

    # Positional mandatory arguments
    parser.add_argument("-c", "--clean", help="Remove old copy of database", type=bool, default=False)
    # Print version
    parser.add_argument("--version", action="version", version='%(prog)s - Version 1.0')

    # Parse arguments
    _args = parser.parse_args()

    return _args


if __name__ == '__main__':
    args = parse_arguments()
    functions = [make_table_copy, delete_and_create, create_tables, migrate_data]
    for i in range(len(functions)):
        print("Step ", functions[i].__name__)
        functions[i]()
        print("Done!")

    if args.clean:
        print("Step ", drop_old_db.__name__)
        drop_old_db()
        print("Done!")
