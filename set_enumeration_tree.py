from math import ceil

class treeNode:
	def __init__(self, val, count):
		self.val = val
		self.count = count
		self.children = []

	def update_children(self, children):
		for child in sorted(children.iterkeys()):
			self.children.append(treeNode(child, children[child]))
		

class setEnumerationTree:
	def __init__(self, records, support):
		self.records = records
		self.size_records = len(records)
		self.mincount = ceil(support*self.size_records)
		self.root = treeNode("", self.size_records)
		freq_one_itemsets = self.get_freq_one_itemsets()
		self.root.update_children(freq_one_itemsets)

	def get_freq_one_itemsets(self):
		candidates = {}
		for record in records:
			for item in record:
				if item in candidates.keys():
					candidates[item] += 1
				else:
					candidates[item] = 1
		for candidate in candidates.keys():
			if candidates[candidate] < self.mincount:
				del candidates[candidate]
		return candidates

if __name__ == "__main__":
	records = [ 
		['d','e','f','l','m','k','p'],
		['m','p','f','a','l','c'],
		['c','p','m','j','f','a'],
		['j','k','o','g','b'],
		['l','c','f','e','n','p','m']
		]
	tree = setEnumerationTree(records, 0.6)

