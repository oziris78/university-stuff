

from bs4 import BeautifulSoup
import requests



def get_all_phone_urls() -> list[str]:
    MAIN_PAGE_DIV = "div.product-list.product-list--list-page"
    MAIN_PAGE_A = "a.product-list__image-safe-link.sld"
    all_phone_urls = []

    i = 1
    while True:
        url = f"https://www.vatanbilgisayar.com/cep-telefonu-modelleri/?page={i}"
        page = requests.get(url)
        soup = BeautifulSoup(page.content, "html.parser")

        divs = soup.select(MAIN_PAGE_DIV)

        # eğer sayfada hiçbir ürün yoksa son sayfadayızdır
        if len(divs) == 0:
            break
        
        # Ana sayfadaki div'lerin içinde bulunan a etiketlerinin href'lerini depola
        for link in soup.select(f"{MAIN_PAGE_DIV} {MAIN_PAGE_A}"):
            href = link.get('href')
            all_phone_urls.append(f"https://www.vatanbilgisayar.com{href}")

        i += 1
    
    return all_phone_urls



def get_data_as_list(all_phone_urls) -> list[dict[str:str]]:
    data = []

    for url in all_phone_urls:
        prod_dict = dict()  # her telefon için böyle bir dict yapılıp data listesine eklenecek

        soup = BeautifulSoup(requests.get(url).content, "html.parser")
        
        product_name = soup.select('h1.product-list__product-name')[0].text.replace('\n', '')
        prod_dict["Product Name"] = product_name

        product_price = soup.select("span.product-list__price")[0].text \
            .replace(".", "").replace("\n", "").replace(",", "")
        prod_dict["Price"] = product_price

        feature_divs = soup.select("div.product-feature")
        for div in feature_divs:
            table = div.select("table.product-table")[0]
            rows = table.select("tr")
            
            for row in rows:
                title = row.select("td")[0].text.replace('\n', '')
                value = row.select("td")[1].text.replace('\n', '')
                prod_dict[title] = value
        
        data.append(prod_dict)   # data = [{}, {}, bu_ürün, {}, {}, ...]
    
    return data



if __name__ == '__main__':
    pass


