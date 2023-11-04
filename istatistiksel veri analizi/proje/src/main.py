
import pandas as pd
import os # for getting the right Pokemon.csv file path

from my_functions import *



########  Section 0 - Project Information  ########
print("\nStatistical Data Analysis Project - Oğuzhan Topaloğlu, Ç19052025\n")



########  Section 1 - Reading the Dataset  ########
main_path = os.path.dirname(__file__)
db_path = os.path.join(main_path, 'Pokemon.csv')
df = pd.read_csv(db_path)               # import the Pokemon dataset
df = df.rename(columns={'#' : 'ID'})    # rename badly named column to 'ID'
df = df.dropna()                        # remove all rows that have null values in any column



########  Section 2 - Plotting  ########
open_plot_windows(df)



########  Section 3 - Calculating Descriptive Stats  ########
desc_stats_df = get_desc_stats_of(df)
print_df(desc_stats_df)



########  Section 4 - T Tests  ########

# TEST 1 - Testing whether the mean Total stat of gen1 and gen3 pokemon are 
# significantly different from each other:
df_sub_1 = df[df['Generation'] == 1]
df_sub_2 = df[df['Generation'] == 3]

print("\nComparing the mean Total stat of Generation 1 and Generation 3 Pokemons:")
do_t_test_for(df_sub_1['Total'], df_sub_2['Total'], before_text='  - ')

# TEST 2 - Testing whether the mean Defense stat of Pokemon with Type 1 equal to "Fire" is 
# significantly different from the mean Defense stat of Pokemon with Type 1 equal to "Water":
df_sub_1 = df[df['Type 1'] == 'Fire']
df_sub_2 = df[df['Type 1'] == 'Water']

print("\nComparing the mean Defense stat of Fire-type and Water-type Pokemons:")
do_t_test_for(df_sub_1['Defense'], df_sub_2['Defense'], before_text='  - ')

# TEST 3 - Testing whether the mean Attack stat of gen1 and gen2 pokemon are 
# significantly different from each other:
df_sub_1 = df[df['Generation'] == 1]
df_sub_2 = df[df['Generation'] == 2]

print("\nComparing the mean Attack stat of Generation 1 and Generation 2 Pokemons:")
do_t_test_for(df_sub_1['Attack'], df_sub_2['Attack'], before_text='  - ')

print("-----------------------------------------------")



########  Section 5 - Calculating Correlation and Testing It  ########
# Sidenote: I used sklearn to check my results, p values in the following tests are
#   very close to 0, sklearn gives results like "2.07E-20" which is very small
#   that's why my functions just return 0, it has nothing to do with the .4f formatting

print("\nCalculating Pearson correlation coefficient for Total stat and Generation:")
calculate_pearson_corr_for(df['Total'], df['Generation'], bf_text='  - ')

print("\nCalculating Pearson correlation coefficient for Sp. Atk and Sp. Def stats:")
calculate_pearson_corr_for(df['Sp. Atk'], df['Sp. Def'], bf_text='  - ')

print("\nCalculating Pearson correlation coefficient for Attack and Defense stats:")
calculate_pearson_corr_for(df['Attack'], df['Defense'], bf_text='  - ')

print("\n-----------------------------------------------\n")


