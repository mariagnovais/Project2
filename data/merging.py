import pandas as pd
import random
def merging_datasets():
    # Normalize institution names
    def normalize_names(s : pd.Series) -> pd.Series:
        s.str.lower()
        s.str.strip()
        s.str.replace(r"\s+", " ", regex=True)
        return s

    def nopoints_names(s : pd.Series) -> pd.Series:
        s = s.astype(str)
        s = s.str.replace('"', '', regex= False)
        s = s.str.replace(r"\.", "", regex=True)
        s =  s.str.replace(r"\s+", " ", regex=True)
        return s

    csv_path1 = r"data\college_data.csv"
    csv_path2 = r"data\majors.csv"

    # Read the CSV files
    data1 = pd.read_csv(csv_path1)
    data2 = pd.read_csv(csv_path2)

    # Apply normalization
    data1['inst_name'] = normalize_names(data1['college_name'])
    data2['inst_name'] = normalize_names(data2['INSTNM'])
    data2['CIPDESC'] = nopoints_names(data2['CIPDESC'])

    # merging
    merged = pd.merge(data1, data2[['inst_name', 'CIPCODE', 'CIPDESC', 'CREDDESC']], on='inst_name', how='left')

    # drop the normalized name column
    merged = merged.drop(columns=['inst_name'])

    # Save the merged data to a new CSV file
    output_csv_path = r"data\merged_data.csv"
    merged.to_csv(output_csv_path, index=False)
    return 

# merging_datasets()

# creating random data for colleges that don't have majors
def add_majors(df : pd.DataFrame) -> pd.DataFrame:

    csv_path_majors = r"data\only_majorsname.csv"
    majors = pd.read_csv(csv_path_majors)
    majors_list = majors['CIPDESC'].tolist()

    def assign_random_major(row):
        if pd.isnull(row['CIPDESC']):
            return random.choice(majors_list)
        else:
            return row['CIPDESC']

    df['CIPDESC'] = df.apply(assign_random_major, axis=1)

merged_data = pd.read_csv(r"data\merged_data.csv")
merged_data_filled = add_majors(merged_data)
output_csv_path_filled = r"data\merged_data_filled.csv"
merged_data_filled.to_csv(output_csv_path_filled, index=False)