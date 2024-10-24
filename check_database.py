import pandas as pd
import sqlite3 as sql

# Check student table
conn = sql.connect('res/db/pbl.db')

df = pd.read_sql_query("SELECT * FROM STUDENT", conn)

print(df)