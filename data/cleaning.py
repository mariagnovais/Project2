import pandas as pd

# ISSUES TO WORK ON FLIGHT:
# ----------------------------------------------------------------------------------------------------------
# DONE, NO FILE. IN MAJORS.CSV TAKE OUT BACHELOR'S DEGREE COLUMN
# DONE TESTED. IN ONLY MAJORSNAME.CSV, PUT THE LEADING ZEROS FROM CIPCODE
# CREATE A SCRIPT TO ADD MAJORS TO COLLEGES THAT DONT HAVE ONE
# RE-SEE NORMALIZE NAMES IN MERGING.PY
# START FRONT END

# NAMES CLEANING
def names_cleaning():
   csv_path = r"data\college_data_cleaned_100k.csv"

   # Read the CSV file
   data = pd.read_csv(csv_path)

   #Remove duplicates
   data = data.drop_duplicates(subset=['college_name'], keep='first')


   # Save only the institution names to a new CSV file 
   newdata = data
   output_csv_path = r"data\college_data.csv"
   newdata.to_csv(output_csv_path, index=False)


#MAJORS
def majors_file():
   # Cleaning up only bachelor's majors, their cip codes, and institutions
   csv_path = r"data\Most-Recent-Cohorts-Field-of-Study.csv"

   # Read the CSV file
   data = pd.read_csv(csv_path, usecols=['INSTNM','CIPCODE', 'CIPDESC', 'CREDDESC'], dtype={'CIPCODE': str})

   filtered_bachelors = data.loc[data['CREDDESC'] == "Bachelor's Degree"]
   filtered_bachelors = filtered_bachelors.drop(columns=['CREDDESC'])

   # print(filtered_bachelors.head())
   # Save only the filtered to a new CSV file 
   output_csv_path = r"data\majors.csv"
   filtered_bachelors.to_csv(output_csv_path, index=False)
   return


# Save majors
def save_majors():
   csv_path = r"data\majors.csv"

   # Read the CSV file
   majors = pd.read_csv(csv_path, usecols=['CIPCODE', 'CIPDESC'],  dtype={'CIPCODE': str})

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
   return


save_majors()