#include <iostream>
#include <curl/curl.h>
#include <fstream>
#include <string>
#include <regex>
#include <xlsxwriter.h>

using namespace std;

// Write callback function to store the data retrieved by curl in a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* data) {
    size_t totalSize = size * nmemb;
    data->append((char*)contents, totalSize);
    return totalSize;
}

// Function to download the HTML content of a webpage using curl
string downloadHTML(const string& url) {
    CURL* curl;
    CURLcode res;
    string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return readBuffer;
}

// Function to parse product and price information using regex
vector<pair<string, string>> extractProductPrices(const string& htmlContent) {
    vector<pair<string, string>> productPrices;
    
    // Example regex for extracting product names and prices
    regex productRegex("<h2 class=\"product-title\">(.*?)</h2>");
    regex priceRegex("<span class=\"price\">\\$([0-9\\.]+)</span>");

    smatch match;
    string::const_iterator searchStart(htmlContent.cbegin());

    while (regex_search(searchStart, htmlContent.cend(), match, productRegex)) {
        string productName = match[1];
        searchStart = match.suffix().first;

        if (regex_search(searchStart, htmlContent.cend(), match, priceRegex)) {
            string productPrice = match[1];
            productPrices.emplace_back(productName, productPrice);
        }
    }

    return productPrices;
}

// Function to generate an Excel report using libxlsxwriter
void generateExcelReport(const vector<pair<string, string>>& productPrices, const string& fileName) {
    lxw_workbook  *workbook  = workbook_new(fileName.c_str());
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    // Add headers
    worksheet_write_string(worksheet, 0, 0, "Product", NULL);
    worksheet_write_string(worksheet, 0, 1, "Price", NULL);

    // Write product and price data to the worksheet
    int row = 1;
    for (const auto& productPrice : productPrices) {
        worksheet_write_string(worksheet, row, 0, productPrice.first.c_str(), NULL);
        worksheet_write_string(worksheet, row, 1, productPrice.second.c_str(), NULL);
        row++;
    }

    workbook_close(workbook);
}

int main() {
    // Example URL of the website you want to scrape
    string url = "https://www.example.com/products";
    
    // Download the HTML content from the URL
    string htmlContent = downloadHTML(url);
    
    // Extract product names and prices from the HTML content
    vector<pair<string, string>> productPrices = extractProductPrices(htmlContent);
    
    // Generate an Excel report with the extracted data
    string excelFileName = "product_prices_report.xlsx";
    generateExcelReport(productPrices, excelFileName);

    cout << "Report generated: " << excelFileName << endl;

    return 0;
}
