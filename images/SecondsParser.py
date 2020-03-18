import csv

filename='MLXP2.csv'
#filename='datanobattery.csv'
result=[]

with open(filename) as csv_file:
    csv_reader=csv.reader(csv_file,delimiter=',')
    for row in csv_reader:
        result.append(row)

maxes=[]
count=0
maxtmp=0
for i in result:
    if count%4==0:
        maxes.append(maxtmp)
        maxtmp=0
    else:
        for j in i[2:]:
            if j!='':
                if float(j)>maxtmp:
                    maxtmp=float(j)
    count+=1




finals=[]
for i in range(len(maxes)):
    if maxes[i]>=100:
        finals.append(maxes[i])

#prunning the lower peaks
for i in range(len(maxes)):
    if maxes[i]<=150:
        maxes[i]=0   #set to zero


# AT THIS POINT MAXES HOLDS ALL MAXES FOR EACH SECOND (ALL ROWS IN CHUNKS OF 4))
# WHEREAS FINALS has those that are >= 100
with open('P2out.csv', 'w') as file:
    for line in maxes:
        file.write("%.2f\n" % line)
