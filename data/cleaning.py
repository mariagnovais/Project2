import pandas as pd

# NAMES CLEANING
# csv_path = 'data\college_data_cleaned_100k.csv'

# # Read the CSV file
# data = pd.read_csv(csv_path)

# #Remove duplicates
# data = data.drop_duplicates(subset=['college_name'], keep='first')


# # Save only the institution names to a new CSV file 
# newdata = data
# output_csv_path = 'data\college_data.csv'
# newdata.to_csv(output_csv_path, index=False)


#MAJORS
# # Cleaning up only bachelor's majors, their cip codes, and institutions
# csv_path = r"data\Most-Recent-Cohorts-Field-of-Study.csv"

# # Read the CSV file
# data = pd.read_csv(csv_path, usecols=['INSTNM','CIPCODE', 'CIPDESC', 'CREDDESC'], dtype={'CIPCODE': str})

# filtered_bachelors = data.loc[data['CREDDESC'] == "Bachelor's Degree"]

# # print(filtered_bachelors.head())
# # Save only the filtered to a new CSV file 
# output_csv_path = r"data\majors.csv"
# filtered_bachelors.to_csv(output_csv_path, index=False)

# NAMES 2
# csv_path = r"data\majors.csv"

# # Read the CSV file
# inst_name = pd.read_csv(csv_path, usecols=['INSTNM'])

# #Remove duplicates
# inst_name = inst_name.drop_duplicates()

# # Save only the institution names to a new CSV file 
# names = inst_name
# output_csv_path = r"data\only_namesmajorfile.csv"
# names.to_csv(output_csv_path, index=False)

# Compare differences in institution names between two files
# csv_path1 = r"data\only_names.csv"
# csv_path2 = r"data\only_namesmajorfile.csv"

# # Read the CSV files
# names1 = pd.read_csv(csv_path1)
# names2 = pd.read_csv(csv_path2)

# Find names in names2 that are not in names1
# differences = names1[~names1['college_name'].isin(names2['INSTNM'])] 

# print(differences)

# len
# len1 = len(names1)
# len2 = len(names2)
# print(f"Number of unique institution names in only_names.csv: {len1}")
# print(f"Number of unique institution names in only_namesmajorfile.csv: {len2}")

# # Save majors
csv_path = r"data\majors.csv"

# # Read the CSV file
majors = pd.read_csv(csv_path, usecols=['CIPCODE', 'CIPDESC'])

#Remove duplicates
majors = majors.drop_duplicates()

# Remove ' "" ' and "." from the major names
def nopoints_names(s : pd.Series) -> pd.Series:
   s = s.astype(str)
   s = s.str.replace('"', '', regex= False)
   s = s.str.replace(r"\.", "", regex=True)
   s =  s.str.replace(r"\s+", " ", regex=True)
   return s


majors['CIPDESC'] = nopoints_names(majors['CIPDESC'])

print(majors.head())
# Save only the filtered to a new CSV file
output_csv_path = r"data\only_majorsname.csv"
majors.to_csv(output_csv_path, index=False)