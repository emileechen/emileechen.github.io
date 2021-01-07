import csv
import json
from pprint import pprint
from calendar import monthrange

csv_file = 'data.csv'
json_file = 'data.json'

with open(csv_file) as f:
    reader = csv.DictReader(f)
    rows = list(reader)

drinks = [[{'Day': str(d+1), 'Month': str(m+1)} if d+1 <= monthrange(2020, m+1)[1] else {} for d in range(31)] for m in range(12)]
for drink in rows:
    drinks[int(drink['Month'])-1][int(drink['Day'])-1] = drink

with open(json_file, 'w') as f:
    f.write('drinks = ')
    json.dump(drinks, f, indent=4)