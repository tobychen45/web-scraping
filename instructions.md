# Financial Data Scraper and Excel Report Generator

## Project Overview

This C++ program automates the extraction of product or service prices from a website and generates an Excel report listing the product names and their corresponding prices. The program uses **libcurl** to fetch the HTML content from a website, **regex** to extract the relevant product and price information, and **libxlsxwriter** to generate an Excel file that neatly organizes the scraped data.

### Features
- **Web Scraping**: Fetch HTML content from a specified website using **libcurl**.
- **Data Parsing**: Extract product names and prices from the HTML using **regex**.
- **Excel Report Generation**: Create an Excel file using **libxlsxwriter** to store the extracted data in a readable format.

---

## Prerequisites

To run this project, you need to install **libcurl** and **libxlsxwriter** libraries. Follow the instructions below to set up these dependencies.

### Installing Dependencies

#### **1. Installing libcurl**

**Linux (Ubuntu/Debian)**:
```bash
sudo apt update
sudo apt install libcurl4-openssl-dev
