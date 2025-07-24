import requests
import yfinance as yf
import pandas as pd

APCA_KEY = 'PKF1N57C1HHP6NJ5ZTBE'
APCA_SECRET = 'slUbRcJRfWST4jS7NfLI6Uf1NqCpeHQD3feeE190'
NINJA_KEY = 'rF3YYejMbxj2SgahJ2htug==tV6YYkYxCSeUf3gv'

headers = {
    'APCA-API-KEY-ID': APCA_KEY,
    'APCA-API-SECRET-KEY': APCA_SECRET
}

symbol = 'AAPL'
bars_url = f'https://data.alpaca.markets/v2/stocks/{symbol}/bars'
params = {'timeframe': '1Day', 'limit': 1}

ticker = yf.Ticker("AAPL")
hist = ticker.history(period="1y")['Close']
volatility = hist.pct_change().std() * (252 ** 0.5)  # annualized
print(f"Annualized historical volatility: {volatility:.2%}")

response = requests.get(bars_url, headers=headers, params=params)
initial_stock_price = response.json()['bars'][0]['c']  # closing price
print(f"{symbol} latest close price: ${initial_stock_price}")


url = "https://api.fiscaldata.treasury.gov/services/api/fiscal_service/v2/accounting/od/avg_interest_rates"
params = {
    "filter": "record_date:gte:2025-01-01,record_date:lte:2025-07-23,security_desc:eq:Treasury Bills",
    "fields": "record_date,security_desc,avg_interest_rate_amt",
    "page[size]": "100"
}
resp = requests.get(url, params=params)
rf_interest_rate = resp.json()["data"][-1].get('avg_interest_rate_amt')
print(rf_interest_rate)

