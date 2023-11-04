

import expert
import menu


print("Uzman Sistemlere Giriş Projesi - Oğuzhan Topaloğlu, Ç19052025")
print("The model is currently being trained, please wait...")
model, gui_df, score = expert.get_model()
print("Model is done training!")
print(f"The model's score was {score}")


while True:
    print("\n---------------------  MENU  ---------------------")
    print("1. Predict price by using custom information")
    print("2. Predict price for random 25 phones")
    selection = input("Your selection: ")

    if not selection.isdigit():
        print("Please enter a number.")
        continue

    selection = int(selection)

    if selection == 1:
        success, guarantee, width, height, weight, brand = menu.get_input(expert.brand_mapper_func)
        if not success:
            continue

        price = expert.predict_price(model, [guarantee, width, height, weight, brand])
        print(f"The predicted price is {price} TL")

    if selection == 2:
        y_true = []
        y_pred = []
        for index, row in gui_df.iterrows():
            phone = row['Name']
            price = row['Price']
            guarantee = row['Guarantee']
            width = row['Width']
            height = row['Height']
            weight = row['Weight']
            brand = row['Brand']
            print(f"Phone is: {phone}")
            print(f"Actual price was {price}")
            y_true.append(price)
            pred_price = expert.predict_price(model, [guarantee, width, height, weight, brand])
            print(f"Predicted price is {pred_price}\n")
            y_pred.append(pred_price)

        expert.print_success_rate(y_true, y_pred)


