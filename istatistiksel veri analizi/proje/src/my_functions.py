

import pandas as pd
from tabulate import tabulate
import seaborn as sns
import matplotlib.pyplot as plt

from math import floor, ceil, sqrt, erf



#######################  PUBLIC FUNCTIONS  #######################


def print_df(df: pd.DataFrame, head_count: int = -1) -> None:
    if head_count == -1: # if it's not entered, print the whole table
        head_count = df.shape[0]
    # tabulate package is for printing better looking tables in the terminal
    print(tabulate(df.head(head_count), headers='keys', tablefmt='psql'))



def open_plot_windows(df: pd.DataFrame) -> None:
    # Plot 1: Scatter plot for showing the relationship between Total and HP
    plt.figure(1)
    sns.scatterplot(x=df['Total'], y=df['HP'])
    plt.xlabel('Total')
    plt.ylabel('HP')

    # Plot 2: Bar plot for showing the count of Pokemons in each generation
    plt.figure(2)
    gen_counts = df['Generation'].value_counts()
    sns.barplot(x=gen_counts.index, y=gen_counts.values)
    plt.xlabel('Generation')
    plt.ylabel('Number of Pokemons')

    # Plot 3: Line plot for showing the Total attribute for each generation 
    plt.figure(3)
    gen_means = df.groupby('Generation')['Total'].mean()
    sns.lineplot(x=gen_means.index, y=gen_means.values)
    plt.xlabel('Generation')
    plt.ylabel('Mean Total')

    # Plot 4: Box plot for Total
    plt.figure(4)
    sns.boxplot(x=df['Total'])
    plt.xlabel('Total')

    # Calling plt.show() at the end so they all get opened in seperate windows
    plt.show()



def get_desc_stats_of(df: pd.DataFrame) -> pd.DataFrame:
    numeric_cols = [col for col in df.columns if df[col].dtype == 'int64']
    desc_stats = []
    for col in numeric_cols:
        terms = list(df[col])
        terms.sort()

        n = __get_count(terms)
        mmin, mmax = __get_min_max(terms)
        mean = __get_mean(terms)
        mode = __get_mode(terms)
        std = __get_std(terms)
        q1, q2, q3 = __get_quartiles(terms)
        iq_range = q3 - q1
        range = mmax - mmin
        coef_of_variance = std / mean
        
        desc_stats.append([n, mmin, mmax, range, mean, mode, std, coef_of_variance, q1, q2, q3, iq_range])

    col_names = ["Count", "Min", "Max", "Range", "Mean", "Mode", "Std", "CV", "Q1", "Q2", "Q3", "IQ"]
    return pd.DataFrame(data=desc_stats, index=numeric_cols, columns=col_names)



def do_t_test_for(series1: pd.Series, series2: pd.Series, before_text: str = "") -> None:
    terms1 = list(series1)
    terms2 = list(series2)

    mean_1 = __get_mean(terms1)
    mean_2 = __get_mean(terms2)
    std1 = __get_std(terms1)
    std2 = __get_std(terms2)
    n1 = float(len(terms1))
    n2 = float(len(terms2))

    t_stat = (mean_1 - mean_2) / sqrt((1/n1 + 1/n2) * ((n1-1)*std1**2 + (n2-1)*std2**2 ) / (n1+n2-2))
    p_value = 1 - erf(abs(t_stat) / sqrt(2))

    print(f'{before_text}The t-statistic is {t_stat:.4f} and the p-value is {p_value:.4f}')
    if p_value < 0.05:
        print(f'{before_text}The difference in means is statistically significant')
    else:
        print(f'{before_text}The difference in means is not statistically significant')
    print()



def calculate_pearson_corr_for(series1: pd.Series, series2: pd.Series, bf_text: str = "") -> None:
    terms1 = list(series1)
    terms2 = list(series2)
    mean1 = __get_mean(terms1)
    mean2 = __get_mean(terms2)
    std1 = __get_std(terms1)
    std2 = __get_std(terms2)

    # Pearson correlation coefficient
    corr = 0
    for a, d in zip(terms1, terms2):
        corr += (a - mean1) * (d - mean2)
    corr /= (len(terms1) - 1) * std1 * std2
    deg_of_free = len(terms1) - 2

    t = corr * (deg_of_free / ((1 - corr) * (1 + corr))) ** 0.5
    p = 1 - erf(abs(t) / sqrt(2))

    print(f'{bf_text}The Pearson correlation coefficient is {corr:} and the p-value is {p:}')
    
    if p < 0.05:
        print(f"{bf_text}The correlation is considered statistically significant.")
        print(f"{bf_text}Which suggests that there is a significant relationship between the two variables.")
    else:
        print(f"{bf_text}The correlation is not considered statistically significant")
        print(f"{bf_text}Which suggests that there is no significant relationship between the two variables.")



########################  PRIVATE FUNCTIONS  ########################


def __get_count(terms: list) -> float:
    return float(len(terms))


def __get_min_max(terms: list) -> tuple[float, float]:
    min_value = float('inf')
    max_value = float('-inf')
    for num in terms:
        if num > max_value:
            max_value = num
        if num < min_value:
            min_value = num
    
    # or simply return min(terms), max(terms)
    return min_value, max_value


def __get_mode(terms: list) -> float:
    counts = {}
    for element in terms:
        if element in counts:
            counts[element] += 1
        else:
            counts[element] = 1

    mode = max(counts, key=counts.get)
    return mode


def __get_mean(terms: list) -> float:
    return sum(terms) / len(terms)


def __get_std(terms: list, is_sample: bool = True) -> float:
    n = len(terms)
    mean = __get_mean(terms)

    std = 0.0
    for num in terms:
        std += (num - mean)**2
    
    if is_sample:
        std /= n - 1
    else:
        std /= n

    std = sqrt(std)
    return std


def __get_quartiles(terms: list) -> tuple[float,float,float]:
    n = len(terms)
    i1 = (n - 1) * 0.25
    i2 = (n - 1) * 0.5
    i3 = (n - 1) * 0.75

    q1 = terms[int(i1)]
    q2 = terms[int(i2)]
    q3 = terms[int(i3)]
    
    if i1 % 1 != 0:
        q1 += (terms[ceil(i1)] - terms[floor(i1)]) * (i1 % 1)
    if i2 % 1 != 0:
        q2 += (terms[ceil(i2)] - terms[floor(i2)]) * (i2 % 1)
    if i3 % 1 != 0:
        q3 += (terms[ceil(i3)] - terms[floor(i3)]) * (i3 % 1)

    return q1, q2, q3



#######################  MAIN  #######################


if __name__ == '__main__':
    pass  # empty since this file is a library
    


