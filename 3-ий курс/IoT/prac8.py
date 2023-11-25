import json
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from dateutil import parser
import statistics

def get_dataframe(file_path):
    with open(file_path) as file:
        json_dict = json.load(file)

        for row in json_dict:
            row['time'] = parser.parse(row['time']).strftime('%H:%M:%S')

        dataframe = pd.DataFrame(json_dict)

        return dataframe

dataframe = get_dataframe('/Users/magomeddaurbekov/Desktop/ВУЗ/3-ий курс/IoT/data.json')

minunte_distribution = sorted(list(set(f'{parser.parse(time).hour:02d}:{parser.parse(time).minute:02d}' for time in dataframe['time'])))
print(minunte_distribution)
voltage_disribution = [statistics.mean(dataframe.loc[dataframe['time'].str.contains(time)]['voltage']) for time in minunte_distribution]
explode = [0.05 if volts == max(voltage_disribution) else 0.01 for volts in voltage_disribution]

def func(pct, allvals):
    absolute = pct/100.*np.sum(allvals)
    return f"{pct:.1f}%\n({absolute:.1f} V)"

plt.title('Voltage')
plt.pie(voltage_disribution, labels=minunte_distribution, autopct=lambda pct: func(pct, voltage_disribution), explode=explode, startangle=180)

dataframe.plot(kind='line', x='time', y='illuminance')
plt.grid(axis='y')

dataframe.plot(kind='hist', x='time', y='sound', bins=[63, 66, 69, 72, 75, 78, 81, 84], width=1.5)
plt.grid(axis='y')

plt.show()