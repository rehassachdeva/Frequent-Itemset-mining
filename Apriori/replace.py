from csv import reader, writer

def parse_config():
	confs = {}
	with open('config.csv') as fconf:
		content = reader(fconf)
		for row in content:
			if row[0] == 'flag':
				row[1] = int(row[1])
			elif row[0] == 'confidence' or row[0] == 'support':
				row[1] = float(row[1])
			confs[row[0]] = row[1]
	return confs

confs = parse_config()

with open("itemCodes.txt") as fcodes, open(confs["input"]) as finp:
	codes = {}
	for line in fcodes.readlines():
		l = line.split(",")
		codes[l[0]]=l[1][:-1]

	for line in finp.readlines():
		l = line.split(",")
		for i in range(len(l)):
			l[i] = codes[l[i]]
		print l

	
