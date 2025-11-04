import pandas as pd

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

    csv_path1 = r"data\college_data_cleaned_100k.csv"
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
    return merged

#merging_datasets()

# add majors to empty rows
def print_empty_majors(df: pd.DataFrame):
        empty_rows = df[df['CIPDESC'].isnull()]
        print("Rows with empty CIPDESC:")
        print(empty_rows)