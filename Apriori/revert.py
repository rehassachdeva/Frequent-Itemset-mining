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

def parse_input(inp_file):
	i = 1
	code = {}
	with open(inp_file) as finp, open("converted.csv", 'wb') as fconv:
		out_writer = writer(fconv)
		content = reader(finp)
		for record in content:
			for j in range(len(record)):
				if record[j] not in code.keys():
					code[record[j]] = i
					i += 1
				record[j] = code[record[j]]
			out_writer.writerow(record)
	with open("itemCodes.csv", 'wb') as fcodes:
		out_writer = writer(fcodes)
		for key,val in code.items():
			out_writer.writerow([key,val]);

if __name__ == "__main__":
	confs = parse_config()
	parse_input(confs['input'])