switcher = {
    0: "R088005",
    1: "R088008",
    2: "R088010",
	3: "R088020",
	4: "R088030",
	5: "R088040",
	6: "R088050",
	7: "R088060",
	8: "R088070",
	9: "R088088",
	
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
