import csv

#filename='correct.csv'
filename='datanobattery.csv'
result=[]

with open(filename) as csv_file:
    csv_reader=csv.reader(csv_file,delimiter=',')
    for row in csv_reader:
        result.append(row)

#the new list to hold the filtered results
filtered=[]

for i in result:
    count = 0
    for j in i[2:]:
        if j!='':
            if float(j)>=35:
                count+=1
    if count >=5:
        filtered.append(i)

for i in filtered:
    print(i)


