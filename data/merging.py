import pandas as pd
import numpy as np
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

    # treat empty strings as NaN
    merged['CIPDESC'] = merged['CIPDESC'].replace(r'^s*$', np.nan, regex=True)

    # Save the merged data to a new CSV file
    output_csv_path = r"data\merged_data.csv"
    merged.to_csv(output_csv_path, index=False)
    return 

# merging_datasets()


# creating random data for colleges that don't have majors
def add_majors():

    # paths
    merged_path = r"data\merged_data.csv"
    csv_path_majors = r"data\only_majorsname.csv"
    output_path = r"data\merged_data_100k.csv"

    # load data
    merged = pd.read_csv(merged_path)
    majors = pd.read_csv(csv_path_majors)
    majors_list = majors['CIPDESC'].dropna().astype(str).tolist()

    # separate missing and not missing
    with_majors = merged[merged['CIPDESC'].notna()].copy()
    without_majors = merged[merged['CIPDESC'].isna()].copy()

    expanded_rows = []

    # for each college without majors, create 13 rows with random majors
    for idx, row in without_majors.iterrows():
        for _ in range(13):
            new_row = row.copy()
            new_row['CIPDESC'] = random.choice(majors_list)
            expanded_rows.append(new_row)

    # combine all
    expanded_df = pd.DataFrame(expanded_rows)
    final = pd.concat([with_majors, expanded_df], ignore_index=True)

    # remove rows without majors
    final = final.dropna(subset=['CIPDESC']).reset_index(drop=True)

    # save the final 
    final.to_csv(output_path, index=False)
    return final

final_df = add_majors()