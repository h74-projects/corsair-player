from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.by import By
import sys

def scrape_youtube(query):
    chrome_options = Options()
    chrome_options.add_argument("--headless")  # Run in headless mode
    driver = webdriver.Chrome(options=chrome_options)

    driver.get(f"https://www.youtube.com/results?search_query={query}")

    
    driver.implicitly_wait(2) 

    first_result_element = driver.find_element(By.CSS_SELECTOR, '#contents a#thumbnail')
    first_result_url = first_result_element.get_attribute("href")

    driver.quit() 

    return first_result_url

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 seleniumScript.py <query>")
        sys.exit(1)

    query = sys.argv[1]
    first_result_url = scrape_youtube(query)

    if first_result_url:
        print(first_result_url)
    else:
        print("No search result URLs found.")
