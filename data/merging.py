import pandas as pd

# Normalize institution names to lowercase for comparison
# def normalize_names(df, column_name):
#     df[column_name] = df[column_name].str.lower().str.strip()
#     return df

csv_path1 = r"data\college_data_cleaned_100k.csv"
csv_path2 = r"data\majors.csv"

# Read the CSV files
names1 = pd.read_csv(csv_path1)
names2 = pd.read_csv(csv_path2)

# merging
