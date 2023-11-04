


def get_input(brand_mapper_func) -> tuple[bool, int, float, float, float, int]:
    print("Please enter some information about the phone: ")

    # guarantee
    guarantee = input('Guarantee (int: month): ')
    try: guarantee = int(guarantee)
    except: return __error("Guarantee must be an integer!")
    
    if guarantee < 0:
        return __error("Guarantee can't be a negative number!")

    # width
    width = input('Width (float: mm): ')
    try:  width = float(width)
    except: return __error("Width must be a float!")
    
    if width < 0:
        return __error("Width can't be a negative number!")

    # height
    height = input('Height (float: mm): ')
    try:  height = float(height)
    except: return __error("Height must be a float!")

    if height < 0:
        return __error("Height can't be a negative number!")
    
    # weight
    weight = input('Weight (float: gr): ')
    try:  weight = float(weight)
    except: return __error("Weight must be a float!")
        
    if weight < 0:
        return __error("Weight can't be a negative number!")
    
    # brand
    brand = input('Brand (str): ')
    brand = brand_mapper_func(brand)
    if brand == 1100:
        print("The brand must be one of these values: ")
        return __error("\tapple, general, honor, huawei, realme, samsung, tcl, tecno, xiaomi, vivo.")

    return True, guarantee, width, height, weight, brand



def __error(s: str) -> tuple[bool, int, float, float, float, int]:
    print(s)
    return False, -1, -1, -1, -1, -1



if __name__ == '__main__':
    pass


