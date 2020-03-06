import csv

filename='MLXP0.csv'
#filename='datanobattery.csv'
result=[]

with open(filename) as csv_file:
    csv_reader=csv.reader(csv_file,delimiter=',')
    for row in csv_reader:
        result.append(row)


#the new list to hold the filtered results
filtered=[]
maxes=[]

for i in result:
    count = 0
    for j in i[2:]:
        if j!='':
            if float(j)>=90:   #we decided that our lower bound should be 40 degrees Celsius
                count+=1
    if count >=2:               #if we have seen at least 5 cells with the above temperature, keep the line
        filtered.append(i)

for i in filtered:  #for every row
    max=0           #init a max
    for j in i[2:]:     #for every element in the row except the first two
        if j!='' and max<float(j):       #if it's not empty and greater than max
            max=float(j)                 #set new max equal to it
    maxes.append([i[0],max])                   #means we have finished the row so append the max


#for output redirection
#or i in filtered:
 #   print(*i)

with open( "P0maxesUnfilteredcount.csv", "w",newline='') as csv_file:
    writer = csv.writer(csv_file, delimiter=',')
    for line in maxes:
        writer.writerow(line)


