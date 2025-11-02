import pandas as pd

csv_path = 'data\college_data_cleaned_100k.csv'

# Read the CSV file
inst_name = pd.read_csv(csv_path, usecols=['college_name'])

#Remove duplicates
inst_name = inst_name.drop_duplicates()

# Save only the institution names to a new CSV file 
names = inst_name
output_csv_path = 'data\only_names.csv'
names.to_csv(output_csv_path, index=False)