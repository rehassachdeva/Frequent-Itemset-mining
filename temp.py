import random

for i in range(1000000):
    num = random.randint(1,8)
    items = []
    for j in range(num):
        items.append(str(random.randint(0,4)))
    print ",".join(items)
