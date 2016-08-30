from csv import reader, writer
from math import ceil
from itertools import combinations
from time import time
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
	records = []
	i = 0
	with open(inp_file) as finp:
		content = reader(finp)
		for record in content:
			records.append(frozenset(record[:-1]))
	return records

def gen_output(freq_itemsets, association_rules, out_file):
	with open(out_file, 'wb') as fout:
		out_writer = writer(fout)
		out_writer.writerow([len(freq_itemsets)])
		for item in freq_itemsets:
			out_writer.writerow(list(item))
		if association_rules:
			out_writer.writerow([len(association_rules)])
			for rule in association_rules:
				out_writer.writerow(list(rule[0]) + ['=>'] + list(rule[1]))

def get_one_itemsets(records):
	candidates = set()
	for record in records:
		for item in record:
			temp = frozenset([item])
			if temp in candidates:
				continue
			else:
				candidates.add(temp)
	return candidates

def filter_freq_itemsets(records, candidates, mincount, counts):
	itemsets = {}
	freq_itemsets = set()
	for candidate in candidates:
		for record in records:
			if record.issuperset(candidate):
				if candidate in itemsets.keys():
					itemsets[candidate] += 1
				else:
					itemsets[candidate] = 1
	for candidate in itemsets.keys():
		val = itemsets[candidate]
		if val >= mincount:
			counts[candidate] = val
			freq_itemsets.add(candidate)
	return freq_itemsets

def apriori_gen(cur_freq_itemsets):
	candidates = set()
	for freq_itemset1 in cur_freq_itemsets:
		for freq_itemset2 in cur_freq_itemsets:
			cnt = 0
			for item in freq_itemset1:
				if item not in freq_itemset2:
					cnt += 1
			if cnt == 1:
				new_item_set = freq_itemset2.union(freq_itemset1)
				flag = 0
				x = len(new_item_set)
				for immediate_subset in combinations(new_item_set, x-1):
					immediate_subset_set = frozenset(list(immediate_subset))
					if immediate_subset_set not in cur_freq_itemsets:
						flag = 1
						break
				if flag == 1:
					continue
				else:
					candidates.add(new_item_set)
	return candidates

def apriori(records, support):
	size = len(records)
	counts = {}
	mincount = ceil(support*size)
	candidates = get_one_itemsets(records)
	freq_itemsets = set()

	while candidates:
		cur_freq_itemsets = filter_freq_itemsets(records, candidates, mincount, counts)
		freq_itemsets = freq_itemsets.union(cur_freq_itemsets)
		candidates = apriori_gen(cur_freq_itemsets)

	return counts, freq_itemsets	

def mine_rules(counts, freq_itemsets, minconf):
	association_rules = set()
	for itemset in freq_itemsets:
		for i in xrange(1, len(itemset)):
			for lhs_candidate in combinations(itemset, i):
				lhs_candidate_set = frozenset(list(lhs_candidate))
				if counts[lhs_candidate_set]*minconf <= counts[itemset]:
					rhs_candidate = itemset.difference(lhs_candidate_set)
					association_rules.add((lhs_candidate_set, rhs_candidate))
	return association_rules

if __name__ == "__main__":
	confs = parse_config()
	records = parse_input(confs['input'])
        start = time()
	counts, freq_itemsets = apriori(records, confs['support'])
	if confs['flag'] == 1:
		association_rules = mine_rules(counts, freq_itemsets, confs['confidence'])
	else:
		association_rules = None
	gen_output(freq_itemsets, association_rules, confs['output'])
        end = time()
        print end-start
