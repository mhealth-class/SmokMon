import csv

filename='smokingOut.csv'
result=[]

with open(filename) as csv_file:
    csv_reader=csv.reader(csv_file,delimiter=',')
    for row in csv_reader:
        result.append(row)

means=[]
for i in result:
    count=0
    sum=0
    for j in i[2:]:
        if j!='':
            sum+=float(j)
            count+=1
    avg=sum/count
    t=(i[0],avg)
    means.append(t)

#print the means
for i in means:
    print(*i)

with open( "smokingMeansOut.csv", "w",newline='') as csv_file:
    writer = csv.writer(csv_file, delimiter=',')
    for line in means:
        writer.writerow(line)