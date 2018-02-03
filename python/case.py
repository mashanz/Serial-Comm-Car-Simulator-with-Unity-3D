switcher = {
    0: "L005088",
    1: "L008088",
    2: "L010088",
	3: "L020088",
	4: "L030088",
	5: "L040088",
	6: "L050088",
	7: "L060088",
	8: "L070088",
	9: "L088088",
	10: "R005088",
    11: "R008088",
    12: "R010088",
	13: "R020088",
	14: "R030088",
	15: "R040088",
	16: "R050088",
	17: "R060088",
	18: "R070088",
	19: "R088088",
	
}


def cases(argument):
    return switcher.get(argument, "nothing")

def sending(loop):
	val = 0
	moveCase = 0
	while True:
		val += 1
		print(cases(moveCase))
		if val>loop:
			val = 0
			moveCase += 1
		if moveCase>2:
			break

if __name__=='__main__':
	sending(1000)
