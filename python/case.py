switcher = {
    0: "L088005",
    1: "L088008",
    2: "L088010",
	3: "L088020",
	4: "L088030",
	5: "L088040",
	6: "L088050",
	7: "L088060",
	8: "L088070",
	9: "L088088",
	10: "R088005",
    11: "R088008",
    12: "R088010",
	13: "R088020",
	14: "R088030",
	15: "R088040",
	16: "R088050",
	17: "R088060",
	18: "R088070",
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
