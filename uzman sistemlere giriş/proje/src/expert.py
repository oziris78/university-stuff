

import pandas as pd
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import confusion_matrix
from sklearn.metrics import accuracy_score, classification_report
from sklearn.model_selection import train_test_split

# benim sınıflar
import scrape



def print_success_rate(y_true, y_pred) -> None:
    print("Confusion matrix: ")
    print(confusion_matrix(y_true, y_pred))
    print()
    print(f"Accuracy is {accuracy_score(y_true, y_pred)}")
    print("Classification report is: ")
    print(classification_report(y_true, y_pred))



def get_model() -> RandomForestClassifier:
    # Web scraping part
    all_phone_urls = scrape.get_all_phone_urls()
    data_as_list = scrape.get_data_as_list(all_phone_urls)
    # Data preprocessing part
    df = __preprocess_data(data_as_list)
    model_df, gui_df = __split_df(df, 25)
    # Modeling part
    model, score = __create_model(model_df)
    return model, gui_df, score



def predict_price(model: RandomForestClassifier, values: list) -> int:
    df_val = pd.DataFrame(columns=["Guarantee","Width","Height","Weight","Brand"])
    df_val.loc[len(df_val)] = values
    y_predicted = model.predict(df_val)
    y_predicted = pd.Series(y_predicted)
    return y_predicted[0]



def __create_model(df: pd.DataFrame):
    X = df.drop(['Price', 'Name', 'Origin'], axis='columns')
    Y = df['Price']

    X_train, X_test, y_train, y_test = train_test_split(X, Y, test_size = 0.2)
    model = RandomForestClassifier(n_estimators=120)
    model.fit(X_train, y_train)
    score = model.score(X_test, y_test)
    return model, score



def __preprocess_data(data_as_list) -> pd.DataFrame:
    df = pd.DataFrame(data_as_list)
    
    # Türkçe sütunların ismini İngilizce yapıyorum 
    df = df.rename(columns={ "Product Name" : "Name", "Menşei" : "Origin", 
        "Garanti" : "Guarantee", "Genişlik" : "Width", 
        "Yükseklik" : "Height", "Ağırlık" : "Weight" })
    
    # Önceden yapılan (manüel) null değer testlerine göre belirlediğim az sayıda null içeren
    # sütunları ayırıyorum ve daha sonrasında null satırları minimal telefon kaybıyla atıyorum
    df = df[ ["Name", "Price", "Origin", "Guarantee", "Width", "Height", "Weight"] ]
    df = df.dropna()

    # gr, gram, mm gibi string kısımlarını kaldırma
    df['Width'] = df['Width'].map(__wh_mapper_func)
    df['Height'] = df['Height'].map(__wh_mapper_func)
    df['Weight'] = df['Weight'].map(__weight_mapper_func)
    df['Origin'] = df['Origin'].map(lambda x: x.lower())

    # veri türlerini düzeltme
    df["Guarantee"] = pd.to_numeric(df["Guarantee"])
    df["Price"] = pd.to_numeric(df["Price"])
    df["Width"] = pd.to_numeric(df["Width"])
    df["Height"] = pd.to_numeric(df["Height"])
    df["Weight"] = pd.to_numeric(df["Weight"])

    # marka bilgisinin oluşturulması
    df["Brand"] = df["Name"].copy().apply(brand_mapper_func)

    return df



# içine verilen df'yi birinde size satır olacak şekilde ikiye ayırıp döner
def __split_df(df: pd.DataFrame, size: int) -> tuple[pd.DataFrame, pd.DataFrame]:
    df = df.sample(frac=1).reset_index(drop=True)  # her satırı shuffle et
    gui_df = df.iloc[:size, :]   # ilk size satırı al
    model_df = df.iloc[size:, :]   # son n-size satırı al
    return model_df.copy(), gui_df.copy()



def brand_mapper_func(s: str) -> int:
    s = s.lower()
    # Vatan bilgisayarda bulunan her marka bunlardır
    if s.count('apple') > 0 or s.count('iphone'):    return 100
    elif s.count('general') > 0:                     return 200
    elif s.count('honor') > 0:                       return 300
    elif s.count('huawei') > 0:                      return 400
    elif s.count('realme') > 0:                      return 500
    elif s.count('samsung') > 0:                     return 600
    elif s.count('tcl') > 0:                         return 700
    elif s.count('tecno') > 0:                       return 800
    elif s.count('xiaomi') > 0:                      return 900
    elif s.count('vivo') > 0:                        return 1000
    else:                                            return 1100



def __weight_mapper_func(s: str) -> str:
    s = s.lower()
    if s.count('gram') > 0:
        return s.replace('gram', '').replace(' ', '')
    elif s.count('gr') > 0:
        return s.replace('gr', '').replace(' ', '')
    elif s.count('g') > 0:
        return s.replace('g', '').replace(' ', '')
    else:
        return s if s.isdigit() else "-1"



def __wh_mapper_func(s: str) -> str:
    s = s.lower()
    if s.count('cm') > 0:
        s = s.replace('cm', '').replace(',', '.').replace(' ', '')
        s = str(float(s) * 10)  # mm yap
        return s 
    elif s.count('mm') > 0:
        return s.replace('mm', '').replace(',', '.').replace(' ', '')



if __name__ == '__main__':
    pass


